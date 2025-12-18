
#include "ModelHead/todomodel.h"
#include <QColor>
#include <QBrush>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TodoModel::TodoModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(new Composite("Root", "Racine invisible", this))
{

}

TodoModel::~TodoModel()
{
    clear();
    delete m_root;
}



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

int TodoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

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

    // Couleur de fond selon l'état
    if (role == Qt::BackgroundRole && index.column() == StateColumn) {
        switch (item->getState()) {
        case TodoState::Not_Ready:
            return QBrush(QColor(255, 200, 200)); // Rouge clair
        case TodoState::Ready_Todo:
            return QBrush(QColor(255, 255, 200)); // Jaune clair
        case TodoState::In_Progress:
            return QBrush(QColor(200, 220, 255)); // Bleu clair
        case TodoState::Done:
            return QBrush(QColor(200, 255, 200)); // Vert clair
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

Qt::ItemFlags TodoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    // Toutes les colonnes sauf l'état sont éditables
    if (index.column() != StateColumn) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

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

TodoItem* TodoModel::getTask(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    return static_cast<TodoItem*>(index.internalPointer());
}

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

void TodoModel::unregisterTask(TodoItem* task)
{
    if (!task) {
        return;
    }

    disconnect(task, &TodoItem::dataChanged,
               this, &TodoModel::onTaskDataChanged);

    m_tasks.removeOne(task);
}

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
void Remplissage_du_Model(QJsonArray arr , Composite* CT , TodoState x , Composite* Root_System);
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

    QJsonObject root = doc.object();
    this->Project_Name = root["Name"].toString();
    QJsonObject rot = root["root"].toObject();
    Composite* Root = new Composite(rot["title"].toString(),rot["description"].toString(),nullptr);
    Root->setState(TodoState::Ready_Todo);
    Root->setDueDate(QDate::fromString(rot["dueDate"].toString()));
    Root->setcountDoneChild(rot["count_childs"].toInt());
    QJsonArray arr = rot["children"].toArray();
    Remplissage_du_Model(arr,Root,TodoState::Ready_Todo,Root);
    AddTask(Root);
    return true;
}
void TodoModel::AddTask(TodoItem* x){
    this->m_tasks.append(x);
    return ;
}



void TodoModel::Remplissage_nexts(Composite * Root_System , TodoItem * p , QJsonArray arr2){
    for(QJsonValueRef voisin : arr2){
        QJsonObject elt = voisin.toObject();
        if(elt["isComposite"].toBool()){
            Composite* eltchild = new Composite(elt["title"].toString(),elt["description"].toString(),nullptr);
            eltchild->setState(TodoState::Not_Ready);
            eltchild->setDueDate(QDate::fromString(elt["dueDate"].toString()));
            eltchild->setcountDoneChild(elt["count_childs"].toInt());
            QJsonArray arr1 = elt["children"].toArray();
            Remplissage_du_Model(arr1,eltchild,TodoState::Not_Ready,Root_System);
            QJsonArray arrr1 = elt["nexts"].toArray();
            eltchild->addPrev(p);
            Remplissage_nexts(Root_System,eltchild,arrr1);
            p->addNext(eltchild);
            AddTask(eltchild);
        }
        else {
            TodoItem* eltt = new TodoItem(elt["title"].toString(),elt["description"].toString(),nullptr);
            eltt->setState(TodoState::Not_Ready);
            eltt->setDueDate(QDate::fromString(elt["dueDate"].toString()));
            QJsonArray arrr = elt["nexts"].toArray();
            eltt->addPrev(p);
            Remplissage_nexts(Root_System,eltt,arrr);
            p->addNext(eltt);
            AddTask(eltt);
        }
    }
    return ;
}
void TodoModel::Remplissage_du_Model(QJsonArray arr , Composite* CT , TodoState x , Composite* Root_System){
    for(QJsonValueRef inter : arr){
        QJsonObject rot = inter.toObject();
        if(rot["isComposite"].toBool()){
            Composite* Root = new Composite(rot["title"].toString(),rot["description"].toString(),CT);
            Root->setState(x);
            Root->setDueDate(QDate::fromString(rot["dueDate"].toString()));
            Root->setcountDoneChild(rot["count_childs"].toInt());
            QJsonArray arr1 = rot["children"].toArray();
            QJsonArray arr2 =  rot["nexts"].toArray();
            Remplissage_nexts(Root_System,Root,arr2);
            Remplissage_du_Model(arr1 , Root , x , Root_System);

            CT->addChild(Root);
            AddTask(Root);

        }
        else {
            TodoItem* Root = new TodoItem(rot["title"].toString(),rot["description"].toString(),nullptr);
            Root->setState(x);
            Root->setDueDate(QDate::fromString(rot["dueDate"].toString()));
            QJsonArray arr2 =  rot["nexts"].toArray();
            Remplissage_nexts(Root_System,Root,arr2);
            CT->addChild(Root);
            AddTask(Root);
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

    // 🔁 NEXTS (petits JSON imbriqués)
    QJsonArray nexts;
    for (TodoItem* n : item->getNexts()) {
        nexts.append(serializeItem(n));
    }
    obj["nexts"] = nexts;

    // 🌳 COMPOSITE → enfants récursifs
    if (item->isComposite()) {
        Composite* comp = static_cast<Composite*>(item);

        obj["count_childs"] = comp->getChildren().size();

        QJsonArray children;
        for (TodoItem* c : comp->getChildren()) {
            children.append(serializeItem(c)); // 🔁 récursif
        }
        obj["children"] = children;
    }

    return obj;
}


bool TodoModel::exportToJson(const QString& filePath) const
{
    if (m_tasks.isEmpty())
        return false;

    Composite* root = dynamic_cast<Composite*>(m_tasks.first());
    if (!root)
        return false;

    QJsonObject json;
    json["Name"] = Project_Name;
    json["root"] = serializeItem(root); // 🌳 récursif

    QJsonDocument doc(json);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

