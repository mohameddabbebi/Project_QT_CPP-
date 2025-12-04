/**
 * @file TodoItem.h
 * @brief Classe représentant une tâche
 * @author [Votre Nom]
 */

#ifndef TODOITEM_H
#define TODOITEM_H

#include <QString>
#include <QDate>
#include <QList>
#include <QObject>
#include "TodoState.h"

/**
 * @class TodoItem
 * @brief Représente une tâche avec ses propriétés et dépendances
 */
class TodoItem : public QObject
{
    Q_OBJECT

public:
     const static int distrubuteur=0;
    /**
     * @brief Constructeur
     * @param title Titre de la tâche
     * @param parent Objet parent Qt (pour la gestion mémoire)
     */
     TodoItem(const QString& title = "",
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
     * @brief Ajoute une tâche précédente (dépendance)
     * @param prev Pointeur vers la tâche précédente
     */
    void addPrev(TodoItem* prev);

    /**
     * @brief Ajoute une tâche suivante
     * @param next Pointeur vers la tâche suivante
     */
    void addNext(TodoItem* next);

    /**
     * @brief Retire une tâche précédente
     */
    void removePrev(TodoItem* prev);

    /**
     * @brief Retire une tâche suivante
     */
    void removeNext(TodoItem* next);

    /**
     * @brief Vérifie si la tâche est prête à être exécutée
     * @return true si toutes les dépendances sont satisfaites
     */
    bool isReady() const;

    /**
     * @brief Met à jour automatiquement l'état selon les dépendances
     */
    void updateStateFromDependencies();

    // Méthodes virtuelles pour le pattern Composite
    virtual bool isComposite() const { return false; }
    virtual bool isLeaf() const { return true; }
    TodoState getState();

signals:
    /**
     * @brief Signal émis lorsque les données de la tâche changent
     */
    void dataChanged();

    /**
     * @brief Signal émis lorsque l'état change
     */
    void stateChanged(TodoState newState);

protected:
    QString id ;
    QString m_title;              ///< Titre de la tâche
    QString m_description;        ///< Description détaillée
    TodoState m_state;            ///< État actuel
    QDate m_dueDate;              ///< Date de fin prévue
    int m_countPrec;              ///< Nombre de prédécesseurs non terminés
    QList<TodoItem*> m_prevs;     ///< Liste des tâches précédentes
    QList<TodoItem*> m_nexts;     ///< Liste des tâches suivantes
};

#endif // TODOITEM_H
