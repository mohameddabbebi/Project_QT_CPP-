#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QSplitter>
#include "todomodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots pour les actions de menu
    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onSaveProjectAs();
    void onQuit();
    void onAbout();

    // Slots pour les boutons d'action
    void onAddRootTask();
    void onAddChildTask();
    void onAddComposite();
    void onDeleteTask();

    // Slots pour la gestion des détails
    void onTaskSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSaveDetails();
    void onCancelDetails();

    // Slot pour la recherche
    void onSearchTextChanged(const QString &text);

    // Slot pour changer l'état d'une tâche
    void onStateChanged(int index);

private:
    // Méthodes de création de l'interface
    void createActions();
    void createMenus();
    void createToolBar();
    void createMainWidget();
    void createDetailPanel();

    // Méthodes utilitaires
    void loadTaskDetails(TodoItem* task);
    void clearDetailPanel();
    void updateDetailPanelState(bool enabled);
    TodoItem* getSelectedTask() const;
    void expandAll();
    void collapseAll();

    // Widgets principaux
    QTreeView* m_treeView;
    TodoModel* m_model;
    QLineEdit* m_searchBox;

    // Panneau de détails
    QWidget* m_detailPanel;
    QLineEdit* m_titleEdit;
    QTextEdit* m_descriptionEdit;
    QDateEdit* m_dueDateEdit;
    QComboBox* m_stateCombo;
    QListWidget* m_prevsListWidget;
    QListWidget* m_nextsListWidget;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
    QLabel* m_taskIdLabel;

    // Actions de menu
    QAction* m_newProjectAction;
    QAction* m_openProjectAction;
    QAction* m_saveProjectAction;
    QAction* m_saveProjectAsAction;
    QAction* m_quitAction;
    QAction* m_aboutAction;
    QAction* m_addRootTaskAction;
    QAction* m_addChildTaskAction;
    QAction* m_addCompositeAction;
    QAction* m_deleteTaskAction;
    QAction* m_expandAllAction;
    QAction* m_collapseAllAction;

    // Variables d'état
    TodoItem* m_currentTask;
    QString m_currentFilePath;
    bool m_hasUnsavedChanges;
};

#endif
