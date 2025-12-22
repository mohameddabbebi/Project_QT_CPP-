
#include "ModelHead/todomodel.h"
#include <QColor>
#include <QBrush>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
/**
 * @brief Constructeur du modèle TodoModel.
 * @param parent Objet parent
 *
 * Initialise le modèle avec une racine composite invisible.
 */
TodoModel::TodoModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(new Composite("Root", "Racine invisible", this))
{

}
/**
 * @brief Destructeur du modèle.
 *
 * Supprime tous les enfants et la racine.
 */
TodoModel::~TodoModel()
{
    clear();
    delete m_root;
}


/**
 * @brief Crée un index de modèle pour la vue.
 * @param row Ligne de l'élément
 * @param column Colonne de l'élément
 * @param parent Index parent
 * @return QModelIndex correspondant à la position demandée
 *
 * Si l'élément demandé est un enfant d'un composite, retourne l'index correspondant.
 */
QModelIndex TodoModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Composite* parentItem = nullptr;

    if (!parent.isValid()) {
        // Si pas de parent → on cherche dans la racine
        parentItem = m_root;
    } else {
        // Sinon, récupérer le parent depuis l'index
        TodoItem* item = static_cast<TodoItem*>(parent.internalPointer());
        if (item && item->isComposite()) {
            parentItem = static_cast<Composite*>(item);
        }
    }

    if (!parentItem) {
        return QModelIndex();
    }

    TodoItem* childItem = parentItem->getChild(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}
/**
 * @brief Récupère l'index parent d'un élément.
 * @param child Index enfant
 * @return QModelIndex parent
 *
 * Retourne un index invalide si l'élément est directement sous la racine.
 */
QModelIndex TodoModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    TodoItem* childItem = static_cast<TodoItem*>(child.internalPointer());
    if (!childItem) {
        return QModelIndex();
    }

    Composite* parentItem = findParentOf(childItem);

    // Si le parent est la racine (ou null) → pas de parent visible
    if (!parentItem || parentItem == m_root) {
        return QModelIndex();
    }

    // Trouver la position du parent dans son propre parent
    Composite* grandParent = findParentOf(parentItem);
    if (!grandParent) {
        return QModelIndex();
    }

    int row = grandParent->indexOf(parentItem);
    return createIndex(row, 0, parentItem);
}
/**
 * @brief Retourne le nombre de lignes enfants pour un index donné.
 */
int TodoModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0; // Pas d'enfants dans les colonnes > 0
    }

    Composite* parentItem = nullptr;

    if (!parent.isValid()) {
        // Pas de parent → compter les enfants de la racine
        parentItem = m_root;
    } else {
        TodoItem* item = static_cast<TodoItem*>(parent.internalPointer());
        if (item && item->isComposite()) {
            parentItem = static_cast<Composite*>(item);
        }
    }

    return parentItem ? parentItem->getChildrenCount() : 0;
}
/**
 * @brief Retourne le nombre de colonnes.
 */
int TodoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}
/**
 * @brief Retourne les données pour un index et un rôle donné.
 * @param index Index de l'élément
 * @param role Rôle Qt
 * @return QVariant correspondant à la donnée demandée
 *
 * Gère l'affichage, la couleur selon l'état, et le pourcentage pour les composites.
 */
QVariant TodoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TodoItem* item = static_cast<TodoItem*>(index.internalPointer());
    if (!item) {
        return QVariant();
    }

    // Rôle d'affichage
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case TitleColumn:
            return item->getTitle();
        case StateColumn:
            return todoStateToString(item->getState());
        case DueDateColumn:
            return item->getDueDate().toString("dd/MM/yyyy");
        case DescriptionColumn:
            return item->getDescription();
        default:
            return QVariant();
        }
    }

    if (role == Qt::ForegroundRole && index.column() == StateColumn) {
        switch (item->getState()) {
        case TodoState::Not_Ready:
            return QBrush(QColor("#c0392b")); // rouge foncé
        case TodoState::Ready_Todo:
            return QBrush(QColor("#f39c12")); // orange
        case TodoState::In_Progress:
            return QBrush(QColor("#2980b9")); // bleu
        case TodoState::Done:
            return QBrush(QColor("#27ae60")); // vert
        }
    }


    // Affichage du pourcentage pour les composites
    if (role == Qt::DisplayRole && index.column() == StateColumn && item->isComposite()) {
        Composite* comp = static_cast<Composite*>(item);
        return QString("%1 (%2%)")
            .arg(todoStateToString(item->getState()))
            .arg(comp->getCompletionPercentage());
    }


    return QVariant();
}

/**
 * @brief Retourne les en-têtes pour les colonnes.
 */
QVariant TodoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case TitleColumn:
            return tr("Titre");
        case StateColumn:
            return tr("État");
        case DueDateColumn:
            return tr("Date limite");
        case DescriptionColumn:
            return tr("Description");
        default:
            return QVariant();
        }
    }
    return QVariant();
}
/**
 * @brief Retourne les flags pour un index donné (éditable, sélectionnable, etc.).
 */
Qt::ItemFlags TodoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    TodoItem* item = static_cast<TodoItem*>(index.internalPointer());
    if (!item)
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;


    if (index.column() != StateColumn) {
        flags |= Qt::ItemIsEditable;
        return flags;
    }


    if (item->getCountPrec() > 0) {
        return flags;
    }


    flags |= Qt::ItemIsEditable;
    return flags;
}


/**
 * @brief Modifie la donnée pour un index donné.
 * @param index Index de la cellule
 * @param value Nouvelle valeur
 * @param role Rôle Qt
 * @return true si la modification a été effectuée
 */
bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    TodoItem* item = static_cast<TodoItem*>(index.internalPointer());
    if (!item) {
        return false;
    }

    bool success = false;

    switch (index.column()) {
    case TitleColumn:
        item->setTitle(value.toString());
        success = true;
        break;
    case DueDateColumn:
        item->setDueDate(QDate::fromString(value.toString(), "dd/MM/yyyy"));
        success = true;
        break;
    case DescriptionColumn:
        item->setDescription(value.toString());
        success = true;
        break;
    default:
        break;
    }

    if (success) {
        emit dataChanged(index, index);
    }

    return success;
}

/**
 * @brief Ajoute une tâche directement sous la racine.
 */

void TodoModel::addRootTask(TodoItem* task)
{
    if (!task) {
        return;
    }

    int row = m_root->getChildrenCount();

    beginInsertRows(QModelIndex(), row, row);
    m_root->addChild(task);
    registerTask(task);
    endInsertRows();
}
/**
 * @brief Ajoute un enfant sous un composite donné.
 */
void TodoModel::addChildTask(const QModelIndex &parent, TodoItem* child)
{
    if (!child || !parent.isValid()) {
        return;
    }

    TodoItem* parentItem = static_cast<TodoItem*>(parent.internalPointer());
    if (!parentItem || !parentItem->isComposite()) {
        return;
    }

    Composite* parentComposite = static_cast<Composite*>(parentItem);
    int row = parentComposite->getChildrenCount();

    beginInsertRows(parent, row, row);
    parentComposite->addChild(child);
    registerTask(child);
    endInsertRows();
}
/**
 * @brief Supprime une tâche et met à jour la vue.
 */
bool TodoModel::removeTask(const QModelIndex &index)
{
    if (!index.isValid()) {
        return false;
    }

    TodoItem* item = static_cast<TodoItem*>(index.internalPointer());
    if (!item) {
        return false;
    }

    Composite* parent = findParentOf(item);
    if (!parent) {
        return false;
    }

    int row = parent->indexOf(item);
    QModelIndex parentIndex = getIndexFromTask(parent);

    beginRemoveRows(parentIndex, row, row);
    parent->removeChild(item);
    unregisterTask(item);
    endRemoveRows();

    delete item;
    return true;
}
/**
 * @brief Retourne le TodoItem correspondant à un index.
 */
TodoItem* TodoModel::getTask(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    return static_cast<TodoItem*>(index.internalPointer());
}
/**
 * @brief Retourne l'index d'un TodoItem donné.
 */
QModelIndex TodoModel::getIndexFromTask(TodoItem* task) const
{
    if (!task || task == m_root) {
        return QModelIndex();
    }

    Composite* parent = findParentOf(task);
    if (!parent) {
        return QModelIndex();
    }

    int row = parent->indexOf(task);
    if (row < 0) {
        return QModelIndex();
    }

    return createIndex(row, 0, task);
}
/**
 * @brief Réinitialise le modèle et supprime toutes les tâches.
 */
void TodoModel::clear()
{
    beginResetModel();

    // Détruire tous les enfants de la racine
    while (m_root->getChildrenCount() > 0) {
        TodoItem* child = m_root->getChild(0);
        m_root->removeChild(child);
        delete child;
    }

    m_tasks.clear();

    endResetModel();
}
/**
 * @brief Slot appelé quand une tâche émet un signal dataChanged.
 */
void TodoModel::onTaskDataChanged()
{
    TodoItem* task = qobject_cast<TodoItem*>(sender());
    if (!task) {
        return;
    }

    QModelIndex taskIndex = getIndexFromTask(task);
    if (taskIndex.isValid()) {
        // Notifier que toute la ligne a changé
        QModelIndex left = index(taskIndex.row(), 0, taskIndex.parent());
        QModelIndex right = index(taskIndex.row(), ColumnCount - 1, taskIndex.parent());
        emit dataChanged(left, right);
    }
}

/**
 * @brief Enregistre une tâche pour le suivi et connecte son signal.
 */

void TodoModel::registerTask(TodoItem* task)
{
    if (!task || m_tasks.contains(task)) {
        return;
    }

    m_tasks.append(task);

    // Connecter les signaux pour mettre à jour l'affichage
    connect(task, &TodoItem::dataChanged,
            this, &TodoModel::onTaskDataChanged);
}

/**
 * @brief Déconnecte et supprime une tâche du suivi.
 */
void TodoModel::unregisterTask(TodoItem* task)
{
    if (!task) {
        return;
    }

    disconnect(task, &TodoItem::dataChanged,
               this, &TodoModel::onTaskDataChanged);

    m_tasks.removeOne(task);
}
/**
 * @brief Trouve le composite parent d'une tâche donnée.
 */
Composite* TodoModel::findParentOf(TodoItem* task) const
{
    if (!task) {
        return nullptr;
    }

    // Vérifier d'abord dans la racine
    if (m_root->getChildren().contains(task)) {
        return m_root;
    }

    // Recherche récursive
    return findParentRecursive(task, m_root);
}
/**
 * @brief Recherche récursive du parent dans un composite.
 */
Composite* TodoModel::findParentRecursive(TodoItem* task, Composite* parent) const
{
    if (!parent || !task) {
        return nullptr;
    }

    // Vérifier les enfants directs
    const QList<TodoItem*>& children = parent->getChildren();
    for (TodoItem* child : children) {
        if (child == task) {
            return parent;
        }

        // Si l'enfant est un composite, chercher récursivement
        if (child->isComposite()) {
            Composite* found = findParentRecursive(task, static_cast<Composite*>(child));
            if (found) {
                return found;
            }
        }
    }

    return nullptr;
}
/**
 * @brief Importe un projet depuis un fichier JSON.
 * @param filePath Chemin vers le fichier JSON
 * @return true si l'import réussi
 */
bool TodoModel::importFromJson(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

    if (error.error != QJsonParseError::NoError)
        return false;
 clear();
    beginResetModel();


    QJsonObject root = doc.object();
    this->Project_Name = root["Name"].toString();
    QJsonObject rot = root["root"].toObject();
   m_root->setTitle(Project_Name);
    m_root->setDescription(rot["description"].toString());
    m_root->setState(stringToTodoState(rot["state"].toString()));
    m_root->setDueDate(QDate::fromString(rot["dueDate"].toString(),Qt::ISODate));
    m_root->setcountDoneChild(rot["count_childs"].toInt());
    QJsonArray arr = rot["children"].toArray();
    registerTask(m_root);
    Remplissage_du_Model(arr,m_root,m_root);
    for (auto& pair : pendingNexts) {
        Remplissage_nexts(m_root, pair.first, pair.second);
    }
    pendingNexts.clear();




    endResetModel();
    return true;
}





void TodoModel::Remplissage_nexts(
    Composite* Root_System,
    TodoItem* p,
    QJsonArray arr2)
{
    for (const QJsonValue& voisin : arr2) {


        QString nextTitle = voisin.toString();

        TodoItem* nextTask = nullptr;
        for (TodoItem* t : m_tasks) {
            if (t->getTitle() == nextTitle) {
                nextTask = t;
                break;
            }
        }

        if (nextTask) {
            p->addNext(nextTask);
            nextTask->addPrev(p);
        }
        else {
            qWarning() << "Next task not found:" << nextTitle;
        }
    }
}
void TodoModel::Remplissage_du_Model(QJsonArray arr , Composite* CT , Composite* Root_System){
    for(QJsonValueRef inter : arr){
        QJsonObject rot = inter.toObject();
        if(rot["isComposite"].toBool()){
            Composite* Root = new Composite(rot["title"].toString(),rot["description"].toString(),CT);
            Root->setState(stringToTodoState(rot["state"].toString()));
            Root->setDueDate(QDate::fromString(rot["dueDate"].toString(),Qt::ISODate));
            Root->setcountDoneChild(rot["count_childs"].toInt());
            QJsonArray arr1 = rot["children"].toArray();
            QJsonArray arr2 =  rot["nexts"].toArray();
            pendingNexts.append({ Root, arr2 });

            Remplissage_du_Model(arr1 , Root  , Root_System);


            CT->addChild(Root);
            registerTask(Root);

        }
        else {
            TodoItem* Root = new TodoItem(rot["title"].toString(), rot["description"].toString(), QDate(), {}, {}, CT);
            Root->setState(stringToTodoState(rot["state"].toString()));
            Root->setDueDate(QDate::fromString(rot["dueDate"].toString(),Qt::ISODate));
            QJsonArray arr2 =  rot["nexts"].toArray();
            pendingNexts.append({ Root, arr2 });
            CT->addChild(Root);
            registerTask(Root);
        }
    }
}




QJsonObject serializeItem(TodoItem* item)
{
    QJsonObject obj;

    obj["title"] = item->getTitle();
    obj["description"] = item->getDescription();
    obj["state"] = todoStateToString(item->getState());
    obj["dueDate"] = item->getDueDate().toString(Qt::ISODate);
    obj["isComposite"] = item->isComposite();
    obj["count_prec"] = item->getPrevs().size();
    QJsonArray nexts;
    for (TodoItem* n : item->getNexts()) {
        nexts.append(n->getTitle());
    }
    obj["nexts"] = nexts;
    if (item->isComposite()) {
        Composite* comp = static_cast<Composite*>(item);

        obj["count_childs"] = comp->getChildren().size();

        QJsonArray children;
        for (TodoItem* c : comp->getChildren()) {
            children.append(serializeItem(c));
        }
        obj["children"] = children;
    }

    return obj;
}


bool TodoModel::exportToJson(const QString& filePath) const
{
    if (m_tasks.isEmpty())
        return false;
    QJsonObject json;
    json["Name"] = Project_Name;
    json["root"] = serializeItem(m_root);

    QJsonDocument doc(json);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}


