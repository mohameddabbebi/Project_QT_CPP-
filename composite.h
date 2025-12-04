/**
 * @file Composite.h
 * @brief Classe implémentant le pattern Composite pour les tâches hiérarchiques
 * @author [Votre Nom]
 */

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "todoitem.h"

/**
 * @class Composite
 * @brief Tâche composite pouvant contenir des sous-tâches
 */
class Composite : public TodoItem
{
    Q_OBJECT

public:
    Composite(const QString& title = "", QObject *parent = nullptr);
    ~Composite() override;

    // Gestion de la hiérarchie
    void addChild(TodoItem* child);
    void removeChild(TodoItem* child);
    TodoItem* getChild(int index) const;
    int getChildrenCount() const { return m_children.size(); }
    const QList<TodoItem*>& getChildren() const { return m_children; }

    int indexOf(TodoItem* child) const;

    // Override du pattern Composite
    bool isComposite() const override { return true; }
    bool isLeaf() const override { return false; }

    /**
     * @brief Vérifie si toutes les sous-tâches sont terminées
     */
    bool areAllChildrenDone() const;

    /**
     * @brief Met à jour l'état selon les sous-tâches
     */
    void updateStateFromChildren();

    /**
     * @brief Calcule le pourcentage de complétion
     * @return Valeur entre 0 et 100
     */
    int getCompletionPercentage() const;

private slots:
    void onChildStateChanged(TodoState newState);

private:
    QList<TodoItem*> m_children;
    int countDoneChild;


};

#endif // COMPOSITE_H
