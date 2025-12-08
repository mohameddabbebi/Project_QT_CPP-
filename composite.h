
#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "todoitem.h"


class Composite : public TodoItem
{
    Q_OBJECT

public:
    Composite(const QString& title = "",const QString& desc = "", QObject *parent = nullptr);
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
     * Vérifie si toutes les sous-tâches sont terminées
     */
    bool areAllChildrenDone() const;

    /**
     *  Met à jour l'état selon les sous-tâches
     */
    void updateStateFromChildren();

    /**
     * Calcule le pourcentage de complétion
     */
    int getCompletionPercentage() const;

private slots:
    void onChildStateChanged(TodoState newState);

private:
    QList<TodoItem*> m_children;
    int countDoneChild;


};

#endif
