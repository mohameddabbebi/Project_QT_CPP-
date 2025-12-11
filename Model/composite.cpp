#include "ModelHead/composite.h"

Composite::Composite(const QString& title,
                     const QString& desc,
                     QObject *parent)
    : TodoItem(title,
               desc,
               QDate::currentDate().addDays(7),
               {},
               {},
               parent)
{

}
Composite::Composite(const QString& idp ,const QString& title,
                     const QString& desc,
                     QObject *parent)
    : TodoItem(idp,title,
               desc,
               QDate::currentDate().addDays(7),
               {},
               {},
               parent)
{

}

Composite::~Composite()
{
    // Les enfants seront détruits automatiquement par Qt (parent/child)
    qDeleteAll(m_children);
    m_children.clear();
}

void Composite::addChild(TodoItem* child)
{
    if (child && !m_children.contains(child)) {
        m_children.append(child);
        child->setParent(this); // Gestion mémoire Qt

        // Connecter le signal de changement d'état
        connect(child, &TodoItem::stateChanged,
                this, &Composite::onChildStateChanged);

        updateStateFromChildren();
        emit dataChanged();
    }
}
void Composite::setcountDoneChild(int n){this->countDoneChild=n;return ;};
void Composite::removeChild(TodoItem* child)
{
    if (m_children.removeOne(child)) {
        disconnect(child, &TodoItem::stateChanged,
                   this, &Composite::onChildStateChanged);
        child->setParent(nullptr);
        updateStateFromChildren();
        emit dataChanged();
    }
}

TodoItem* Composite::getChild(int index) const
{
    if (index >= 0 && index < m_children.size()) {
        return m_children.at(index);
    }
    return nullptr;
}

int Composite::indexOf(TodoItem* child) const
{
    return m_children.indexOf(child);
}

bool Composite::areAllChildrenDone() const
{
    if (m_children.isEmpty()) {
        return false;
    }

    for (const TodoItem* child : m_children) {
        if (child->getState() != TodoState::Done) {
            return false;
        }
    }
    return true;
}

void Composite::updateStateFromChildren()
{
    if (m_children.isEmpty()) {
        return;
    }

    // Toutes les sous-tâches sont terminées → Composite Done
    if (areAllChildrenDone()) {
        setState(TodoState::Done);
        return;
    }

    // Analyser l'état des enfants
    bool hasInProgress = false;
    bool hasReady = false;
    bool allNotReady = true;

    for (const TodoItem* child : m_children) {
        TodoState childState = child->getState();

        if (childState == TodoState::In_Progress) {
            hasInProgress = true;
            allNotReady = false;
        }
        else if (childState == TodoState::Ready_Todo) {
            hasReady = true;
            allNotReady = false;
        }
        else if (childState == TodoState::Done) {
            allNotReady = false;
        }
    }


    if (hasInProgress) {
        // Au moins un enfant en cours → Composite en cours
        setState(TodoState::In_Progress);
    }
    else if (hasReady) {
        // Au moins un enfant prêt → Composite prêt
        setState(TodoState::Ready_Todo);
    }
    else if (allNotReady) {
        // Tous les enfants pas prêts → Composite pas prêt
        setState(TodoState::Not_Ready);
    }
    else if (m_state == TodoState::Done) {
        // Si on était Done mais plus maintenant
        setState(TodoState::Ready_Todo);
    }
}
int Composite::getCompletionPercentage() const
{
    if (m_children.isEmpty()) {
        return m_state == TodoState::Done ? 100 : 0;
    }

    int doneCount = 0;
    for (const TodoItem* child : m_children) {
        if (child->getState() == TodoState::Done) {
            doneCount++;
        }
    }

    return (doneCount * 100) / m_children.size();
}

void Composite::onChildStateChanged(TodoState newState)
{
    Q_UNUSED(newState)
    updateStateFromChildren();
}
