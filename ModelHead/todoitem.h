#ifndef TODOITEM_H
#define TODOITEM_H

#include <QString>
#include <QDate>
#include <QList>
#include <QObject>
#include "TodoState.h"

class TodoItem : public QObject
{
    Q_OBJECT

public:
     TodoItem(const QString& title = "",
                 const QString& desc = "",
                 const QDate& dat = QDate::currentDate().addDays(7),
                 const QList<TodoItem*>& prevs = {},
                 const QList<TodoItem*>& nexts = {},
                 QObject *parent = nullptr);
    TodoItem(const QString& idp ,const QString& title = "",
             const QString& desc = "",
             const QDate& dat = QDate::currentDate().addDays(7),
             const QList<TodoItem*>& prevs = {},
             const QList<TodoItem*>& nexts = {},
             QObject *parent = nullptr);

    /**
     * @brief Destructeur virtuel
     */
    virtual ~TodoItem();

    // Getters
    QString getId() const { return id; }
    QString getTitle() const { return m_title; }
    QString getDescription() const { return m_description; }
    TodoState getState() const { return m_state; }
    QDate getDueDate() const { return m_dueDate; }
    int getCountPrec() const { return m_countPrec; }

    const QList<TodoItem*>& getPrevs() const { return m_prevs; }
    const QList<TodoItem*>& getNexts() const { return m_nexts; }

    // Setters
    void setTitle(const QString& title);
    void setDescription(const QString& description);
    void setState(TodoState state);
    void setDueDate(const QDate& date);

    // Gestion des dépendances
    /**
     *Ajoute une tâche précédente (dépendance)
     */
    void addPrev(TodoItem* prev);

    /**
     * Ajoute une tâche suivante
     */
    void addNext(TodoItem* next);

    /**
     * Retire une tâche précédente
     */
    void removePrev(TodoItem* prev);

    /**
     * Retire une tâche suivante
     */
    void removeNext(TodoItem* next);

    /**
     * Vérifie si la tâche est prête à être exécutée
     */
    bool isReady() const;

    /**
     *  Met à jour automatiquement l'état selon les dépendances
     */
    void updateStateFromDependencies();

    // Méthodes virtuelles pour le pattern Composite
    virtual bool isComposite() const { return false; }
    virtual bool isLeaf() const { return true; }

signals:
    /**
     *  Signal émis lorsque les données de la tâche changent
     */
    void dataChanged();

    /**
     * Signal émis lorsque l'état change
     */
    void stateChanged(TodoState newState);

protected:
    QString id ;
    QString m_title;
    QString m_description;
    TodoState m_state;
    QDate m_dueDate;
    int m_countPrec;
    QList<TodoItem*> m_prevs;
    QList<TodoItem*> m_nexts;
};

#endif
