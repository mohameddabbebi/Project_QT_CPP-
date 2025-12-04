/**
 * @file Composite.cpp
 * @brief Implémentation de Composite
 * @author [Votre Nom]
 */

#include "composite.h"

Composite::Composite(const QString& title, QObject *parent)
    : TodoItem()
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

    if (areAllChildrenDone()) {
        setState(TodoState::Done);
    } else {
        // Vérifier si au moins une sous-tâche est en cours
        bool hasInProgress = false;
        for (const TodoItem* child : m_children) {
            if (child->getState() == TodoState::In_Progress) {
                hasInProgress = true;
                break;
            }
        }

        if (hasInProgress) {
            setState(TodoState::In_Progress);
        } else if (m_state == TodoState::Done) {
            // Si on était Done mais plus maintenant
            setState(TodoState::Ready_Todo);
        }
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
