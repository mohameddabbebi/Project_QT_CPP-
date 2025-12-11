#include "ModelHead/todoitem.h"
#include <QUuid>

TodoItem::TodoItem(const QString& title,
                   const QString& desc,
                   const QDate& dat,
                   const QList<TodoItem*>& prevs,
                   const QList<TodoItem*>& nexts,
                   QObject *parent)
    : QObject(parent)
    ,id(QUuid::createUuid().toString())
    , m_title(title)
    , m_description(desc)
    , m_state(TodoState::Not_Ready)
    , m_dueDate(dat)
    , m_countPrec(0)
    , m_prevs(prevs)
    , m_nexts(nexts)
{
    // Initialisation du nombre de prédécesseurs non terminés
    m_countPrec = m_prevs.size();

    for (TodoItem* prev : m_prevs) {
        if (prev && prev->getState() == TodoState::Done) {
            m_countPrec--;
        }

        if(prev){
        prev->addNext(this);
        }
    }
    for (TodoItem* next : m_nexts) {
        if (next) {
            next->addPrev(this);
        }
    }

    // Définir l’état initial
    if (m_countPrec == 0) {
        m_state = TodoState::Ready_Todo;
    } else {
        m_state = TodoState::Not_Ready;
    }
}

TodoItem::TodoItem(const QString& idp,const QString& title,
                   const QString& desc,
                   const QDate& dat,
                   const QList<TodoItem*>& prevs,
                   const QList<TodoItem*>& nexts,
                   QObject *parent)
    : QObject(parent)
    ,id(idp)
    , m_title(title)
    , m_description(desc)
    , m_state(TodoState::Not_Ready)
    , m_dueDate(dat)
    , m_countPrec(0)
    , m_prevs(prevs)
    , m_nexts(nexts)
{
    // Initialisation du nombre de prédécesseurs non terminés
    m_countPrec = m_prevs.size();

    for (TodoItem* prev : m_prevs) {
        if (prev && prev->getState() == TodoState::Done) {
            m_countPrec--;
        }

        if(prev){
            prev->addNext(this);
        }
    }
    for (TodoItem* next : m_nexts) {
        if (next) {
            next->addPrev(this);
        }
    }

    // Définir l’état initial
    if (m_countPrec == 0) {
        m_state = TodoState::Ready_Todo;
    } else {
        m_state = TodoState::Not_Ready;
    }
}

TodoItem::~TodoItem()
{
    // Nettoyer les relations bidirectionnelles
    for (TodoItem* prev : m_prevs) {
        if (prev) {  // ✅ CORRECTION : vérifier avant d'utiliser
            prev->removeNext(this);
        }
    }
    for (TodoItem* next : m_nexts) {
        if (next) {  // ✅ CORRECTION : vérifier avant d'utiliser
            next->removePrev(this);
        }
    }
}

void TodoItem::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit dataChanged();
    }
}

void TodoItem::setDescription(const QString& description)
{
    if (m_description != description) {
        m_description = description;
        emit dataChanged();
    }
}

void TodoItem::setState(TodoState state)
{
    if (m_state != state) {
        m_state = state;

        emit stateChanged(state);
        emit dataChanged();

        // Si la tâche passe à Done → prévenir les suivantes
        if (state == TodoState::Done) {
            for (TodoItem* next : m_nexts) {
                next->m_countPrec--;
                next->updateStateFromDependencies();
            }
        }
    }
}

void TodoItem::setDueDate(const QDate& date)
{
    if (m_dueDate != date) {
        m_dueDate = date;
        emit dataChanged();
    }
}

void TodoItem::addPrev(TodoItem* prev)
{
    if (prev && !m_prevs.contains(prev)) {
        m_prevs.append(prev);
        if (!prev->getNexts().contains(this)) {
            prev->addNext(this);
        }

        if (prev->getState() != TodoState::Done) {
            m_countPrec++;
        }

        updateStateFromDependencies();
    }
}

void TodoItem::addNext(TodoItem* next)
{
    if (next && !m_nexts.contains(next)) {
        m_nexts.append(next);
        if (!next->getPrevs().contains(this)) {
            next->addPrev(this);
        }
    }
}

void TodoItem::removePrev(TodoItem* prev)
{
    if (m_prevs.removeOne(prev)) {
        if (prev && prev->getState() != TodoState::Done) {
            m_countPrec--;
        }
        updateStateFromDependencies();
    }
}

void TodoItem::removeNext(TodoItem* next)
{
    m_nexts.removeOne(next);
}

bool TodoItem::isReady() const
{
    return m_countPrec == 0;
}

void TodoItem::updateStateFromDependencies()
{
    if (m_state == TodoState::Done)
        return;

    if (m_countPrec > 0) {
        setState(TodoState::Not_Ready);
    } else {
        setState(TodoState::Ready_Todo);
    }
}
