#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include "todoitem.h"
#include "composite.h"

class TodoModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum Column {
        TitleColumn = 0,
        StateColumn,
        DueDateColumn,
        DescriptionColumn,
        ColumnCount
    };

    explicit TodoModel(QObject *parent = nullptr);

    ~TodoModel() override;


    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;


    Qt::ItemFlags flags(const QModelIndex &index) const override;


    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;


    void addRootTask(TodoItem* task);


    void addChildTask(const QModelIndex &parent, TodoItem* child);


    bool removeTask(const QModelIndex &index);

    TodoItem* getTask(const QModelIndex &index) const;


    QModelIndex getIndexFromTask(TodoItem* task) const;

    const QList<TodoItem*>& getAllTasks() const { return m_tasks; }

    void AddTask(TodoItem* x);
    Composite* getRoot() const { return m_root; }

    bool importFromJson(const QString& filePath);
    bool exportToJson(const QString& filePath) const;

    void clear();

private slots:

    void onTaskDataChanged();

private:

    void registerTask(TodoItem* task);

    void unregisterTask(TodoItem* task);


    Composite* findParentOf(TodoItem* task) const;


    Composite* findParentRecursive(TodoItem* task, Composite* parent) const;

private:
    QString Project_Name;
    Composite* m_root;
    QList<TodoItem*> m_tasks;
};

#endif
