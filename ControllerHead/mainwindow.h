#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


#include "../ModelHead/todomodel.h"
#include "../ModelHead/todoitem.h"
#include "../ModelHead/composite.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    // File Menu
    void on_actionNewProject_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionQuit_triggered();
    void on_actionImporter_triggered();
    void on_actionExporter_triggered();


    void on_actionAddTask_triggered();
    void on_actionAddChild_triggered();
    void on_actionAddComposite_triggered();
    void on_actionDelete_triggered();


    void on_actionAbout_triggered();


    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void on_stateCombo_currentIndexChanged(int index);
    void on_searchBox_textChanged(const QString &text);


    void onTaskSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actionAdd_SubTaskComposite_triggered();

private:

    Ui::MainWindow *ui;


    TodoModel* m_model;


    TodoItem* m_currentTask;
    QString m_currentFilePath;
    bool m_hasUnsavedChanges;


    void setupConnections();
    void setupStateComboBox();
    void setupTreeView();
    void loadTaskDetails(TodoItem* task);
    void clearDetailPanel();
    void updateDetailPanelState(bool enabled);
};

#endif
