#include "../ControllerHead/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include "todofilterproxymodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model(new TodoModel(this))
    , m_proxyModel(new TodoFilterProxyModel(this))  // ✅ CRÉER le proxy
    , m_currentTask(nullptr)
    , m_hasUnsavedChanges(false)
    , m_editPrevsMode(false)
{
    ui->setupUi(this);

    // Configurer le proxy
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1);


    setupTreeView();
    setupConnections();
    setupStateComboBox();


    updateDetailPanelState(false);



    ui->statusBar->showMessage(tr("Ready - Welcome to KIRA!"), 3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::setupTreeView()
{

   ui->treeView->setModel(m_proxyModel);


    ui->treeView->setColumnWidth(0, 300);  // Title
    ui->treeView->setColumnWidth(1, 120);  // State
    ui->treeView->setColumnWidth(2, 100);  // Date
    ui->treeView->setColumnWidth(3, 200);  // Description


    ui->treeView->header()->setStretchLastSection(true);
}

void MainWindow::setupConnections()
{

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onTaskSelectionChanged);
    connect(ui->searchBox, &QLineEdit::textChanged,
            this, &MainWindow::on_searchBox_textChanged);
}

void MainWindow::setupStateComboBox()
{

    ui->stateCombo->clear();


    ui->stateCombo->addItem(tr("Not_Ready"),
                            static_cast<int>(TodoState::Not_Ready));
    ui->stateCombo->addItem(tr("Ready_Todo"),
                            static_cast<int>(TodoState::Ready_Todo));
    ui->stateCombo->addItem(tr("In_Progress"),
                            static_cast<int>(TodoState::In_Progress));
    ui->stateCombo->addItem(tr("Done"),
                            static_cast<int>(TodoState::Done));
}


void MainWindow::on_actionNewProject_triggered()
{
    if (m_hasUnsavedChanges) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("New Project"),
            tr("Do you want to save changes?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Yes);

        if (reply == QMessageBox::Yes) {
            on_actionSave_triggered();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }


    m_model->clear();
    m_currentFilePath.clear();
    m_hasUnsavedChanges = false;
    clearDetailPanel();

    ui->statusBar->showMessage(tr("New project created"), 3000);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Project"),
        QDir::homePath(),
        tr("JSON Files (*.json);;XML Files (*.xml);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }
    on_actionNewProject_triggered();
    if (m_model->importFromJson(fileName)) {
        m_currentFilePath = fileName;
        m_hasUnsavedChanges = false;
        ui->treeView->expandAll();
        ui->statusBar->showMessage(tr("Project loaded successfully"), 3000);
        ui->treeView->expandAll();
    } else {
        QMessageBox::warning(this, tr("Error"),
                             tr("Failed to load project from: %1").arg(fileName));
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_currentFilePath.isEmpty()) {
        on_actionSaveAs_triggered();
        return;
    }


    if (m_model->exportToJson(m_currentFilePath)) {
        m_hasUnsavedChanges = false;
        ui->statusBar->showMessage(
            tr("Project saved: %1").arg(m_currentFilePath),
            3000);
    } else {
        QMessageBox::warning(this, tr("Error"),
                             tr("Failed to save project to: %1").arg(m_currentFilePath));
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Project As"),
        QDir::homePath() + "/project.json",
        tr("JSON Files (*.json);;XML Files (*.xml)"));

    if (fileName.isEmpty()) {
        return;
    }


    if (!fileName.endsWith(".json") && !fileName.endsWith(".xml")) {
        fileName += ".json";
    }

    m_currentFilePath = fileName;
    on_actionSave_triggered();
}

void MainWindow::on_actionImporter_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Import Tasks"),
        QDir::homePath(),
        tr("JSON Files (*.json);;XML Files (*.xml)"));

    if (fileName.isEmpty()) {
        return;
    }

    if (m_model->importFromJson(fileName)) {
        ui->treeView->expandAll();
        ui->statusBar->showMessage(tr("Tasks imported successfully"), 3000);
    } else {
        QMessageBox::warning(this, tr("Error"),
                             tr("Failed to import from: %1").arg(fileName));
    }
}

void MainWindow::on_actionExporter_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Export Tasks"),
        QDir::homePath() + "/export.json",
        tr("JSON Files (*.json);;XML Files (*.xml)"));

    if (fileName.isEmpty()) {
        return;
    }

    if (m_model->exportToJson(fileName)) {
        ui->statusBar->showMessage(tr("Tasks exported successfully"), 3000);
    } else {
        QMessageBox::warning(this, tr("Error"),
                             tr("Failed to export to: %1").arg(fileName));
    }
}

void MainWindow::on_actionQuit_triggered()
{
    if (m_hasUnsavedChanges) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("Quit"),
            tr("Do you want to save before quitting?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Yes);  // ✅ Fix: add default button parameter

        if (reply == QMessageBox::Yes) {
            on_actionSave_triggered();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(
        this,
        tr("About KIRA"),
        tr("<h2>KIRA - Task Manager</h2>"
           "<p><b>Version:</b> 1.0</p>"
           "<p>Hierarchical task manager with dependencies</p>"
           "<hr>"
           "<p><b>Authors:</b></p>"
           "<ul>"
           "<li>DABBEBI MOHAMED</li>"
           "<li>DJEBBI Yesmine</li>"
           "</ul>"
           "<p><b>Project:</b> LAOA - 2025</p>"
           "<p><b>Technology:</b> Qt 6 / C++17</p>"));
}


void MainWindow::on_actionAddTask_triggered()
{
QString title = tr("Task %1").arg(m_taskCounter++);
    TodoItem* task = new TodoItem("",
                                  title,
                                  tr("Task description"));

    m_model->addRootTask(task);

    m_hasUnsavedChanges = true;
    ui->treeView->expandAll();

    ui->statusBar->showMessage(tr("Root task added: %1").arg(title), 2000);
}

void MainWindow::on_actionAddChild_triggered()
{
    QModelIndex current = ui->treeView->currentIndex();

    if (!current.isValid()) {
        QMessageBox::warning(
            this,
            tr("Add Subtask"),
            tr("Please select a composite task to add a subtask."));
        return;
    }


    QModelIndex sourceIndex = mapToSource(current);
    TodoItem* parent = m_model->getTask(sourceIndex);

    if (!parent || !parent->isComposite()) {
        QMessageBox::warning(
            this,
            tr("Add Subtask"),
            tr("Selected task is not a composite.\n\n"
               "Only composites can contain subtasks.\n"
               "Use 'Add Composite' to create a composite project."));
        return;
    }

  QString title = tr("SubTask %1").arg(m_subtaskCounter++);
    TodoItem* child = new TodoItem("",
                                   title,
                                   tr("Subtask description"));


    m_model->addChildTask(sourceIndex, child);

    m_hasUnsavedChanges = true;
    ui->treeView->expand(current);

    ui->statusBar->showMessage(tr("Subtask added"), 2000);
}
void MainWindow::on_actionAdd_SubTaskComposite_triggered()
{
    QModelIndex current = ui->treeView->currentIndex();

    if (!current.isValid()) {
        QMessageBox::warning(
            this,
            tr("Add Subtask"),
            tr("Please select a composite task to add a subtask."));
        return;
    }


    QModelIndex sourceIndex = mapToSource(current);
    TodoItem* parent = m_model->getTask(sourceIndex);

    if (!parent || !parent->isComposite()) {
        QMessageBox::warning(
            this,
            tr("Add Subtask"),
            tr("Selected task is not a composite.\n\n"
               "Only composites can contain subtasks.\n"
               "Use 'Add Composite' to create a composite project."));
        return;
    }

  QString title = tr("SubTask %1").arg(m_subtaskCounter++);
    Composite* child = new Composite("",
                                     title,
                                     tr("Subtask description"));


    m_model->addChildTask(sourceIndex, child);

    m_hasUnsavedChanges = true;
    ui->treeView->expand(current);

    ui->statusBar->showMessage(tr("Subtask added"), 2000);
}
void MainWindow::on_actionAddComposite_triggered()
{

    QString title = tr("Composite %1").arg(m_compositeCounter++);
    Composite* composite = new Composite("",
                                         title,
                                         tr("Composite description"));

    m_model->addRootTask(composite);

    m_hasUnsavedChanges = true;
    ui->treeView->expandAll();

    ui->statusBar->showMessage(tr("Composite project added"), 2000);
}

void MainWindow::on_actionDelete_triggered()
{
    QModelIndex current = ui->treeView->currentIndex();

    if (!current.isValid()) {
        QMessageBox::warning(
            this,
            tr("Delete"),
            tr("Please select a task to delete."));
        return;
    }

    QModelIndex sourceIndex = mapToSource(current);
    TodoItem* task = m_model->getTask(sourceIndex);
    if (!task) {
        return;
    }


    QString message = tr("Are you sure you want to delete '%1'?")
                          .arg(task->getTitle());

    if (task->isComposite()) {
        Composite* comp = static_cast<Composite*>(task);
        if (comp->getChildrenCount() > 0) {
            message += tr("\n\nThis task contains %1 subtask(s) "
                          "which will also be deleted.")
                           .arg(comp->getChildrenCount());
        }
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Delete"),
        message,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_model->removeTask(sourceIndex);
        m_hasUnsavedChanges = true;
        clearDetailPanel();
        ui->statusBar->showMessage(tr("Task deleted"), 2000);
    }
}


QModelIndex MainWindow::mapToSource(const QModelIndex& proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();

    return m_proxyModel->mapToSource(proxyIndex);
}
void MainWindow::onTaskSelectionChanged(
    const QModelIndex &current,
    const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!current.isValid()) {
        clearDetailPanel();
        updateDetailPanelState(false);
        return;
    }

    try {
        QModelIndex sourceIndex = mapToSource(current);

        qDebug() << "=== Task Selection Changed ===";
        qDebug() << "Proxy index valid:" << current.isValid();
        qDebug() << "Source index valid:" << sourceIndex.isValid();

        TodoItem* task = m_model->getTask(sourceIndex);

        if (task) {
            qDebug() << "Task found:" << task->getTitle();
            qDebug() << "Is composite:" << task->isComposite();

            updateDetailPanelState(true);
            loadTaskDetails(task);

            qDebug() << "Details loaded successfully";
        } else {
            qDebug() << "ERROR: getTask returned nullptr!";
            clearDetailPanel();
            updateDetailPanelState(false);
        }
    } catch (const std::exception& e) {
        qDebug() << "EXCEPTION in onTaskSelectionChanged:" << e.what();

    } catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in onTaskSelectionChanged";

    }
}
void MainWindow::on_saveButton_clicked()
{
    if (!m_currentTask) {
        return;
    }


    m_currentTask->setTitle(ui->titleEdit->text());
    m_currentTask->setDescription(ui->descriptionEdit->toPlainText());
    m_currentTask->setDueDate(ui->dueDateEdit->date());

    m_hasUnsavedChanges = true;


    QModelIndex proxyIndex = ui->treeView->currentIndex();
    QModelIndex sourceIndex = mapToSource(proxyIndex);
    emit m_model->dataChanged(sourceIndex, sourceIndex);

    ui->statusBar->showMessage(tr("Changes saved"), 2000);
}

void MainWindow::on_cancelButton_clicked()
{
    if (m_currentTask) {

        loadTaskDetails(m_currentTask);
        ui->statusBar->showMessage(tr("Changes cancelled"), 2000);
    }
}

void MainWindow::on_stateCombo_currentIndexChanged(int index)
{
    if (!m_currentTask) {
        return;
    }

    if (m_currentTask->getCountPrec() > 0) {
        ui->stateCombo->blockSignals(true);
        loadTaskDetails(m_currentTask); // restaurer l'état réel
        ui->stateCombo->blockSignals(false);


        return;
    }

    TodoState newState = static_cast<TodoState>(
        ui->stateCombo->itemData(index).toInt());

    m_currentTask->setState(newState);

    m_hasUnsavedChanges = true;



    QModelIndex proxyIndex = ui->treeView->currentIndex();
    QModelIndex sourceIndex = mapToSource(proxyIndex);
    emit m_model->dataChanged(sourceIndex, sourceIndex);


    ui->statusBar->showMessage(
        tr("State changed: %1").arg(todoStateToString(newState)),
        2000);
}

void MainWindow::on_searchBox_textChanged(const QString &text)
{

    m_proxyModel->setFilterFixedString(text);

    if (text.isEmpty()) {
        ui->statusBar->clearMessage();
    } else {
        ui->statusBar->showMessage(tr("Search: %1").arg(text), 2000);
    }
}



void MainWindow::loadTaskDetails(TodoItem* task)
{
    qDebug() << ">>> loadTaskDetails START";

    if (!task) {
        qDebug() << "Task is nullptr, clearing panel";
        clearDetailPanel();
        return;
    }

    qDebug() << "Setting m_currentTask";
    m_currentTask = task;

    qDebug() << "Setting taskIdLabel...";
    ui->taskIdLabel->setText(task->getTitle());

    qDebug() << "Setting titleEdit...";
    ui->titleEdit->setText(task->getTitle());

    qDebug() << "Setting descriptionEdit...";
    ui->descriptionEdit->setPlainText(task->getDescription());

    qDebug() << "Setting dueDateEdit...";
    ui->dueDateEdit->setDate(task->getDueDate());

    qDebug() << "Checking canChangeState...";
    qDebug() << "getCountPrec() =" << task->getCountPrec();
    bool canChangeState = (task->getCountPrec() == 0);
    ui->stateCombo->setEnabled(canChangeState);

    if (!canChangeState) {
        ui->stateCombo->setToolTip(
            tr("Impossible de modifier l'état :\n"
               "Tous les prédécesseurs ne sont pas terminés."));
    } else {
        ui->stateCombo->setToolTip(tr("Modifier l'état de la tâche"));
    }

    qDebug() << "Updating stateCombo...";
    ui->stateCombo->blockSignals(true);
    for (int i = 0; i < ui->stateCombo->count(); ++i) {
        TodoState comboState = static_cast<TodoState>(
            ui->stateCombo->itemData(i).toInt());

        if (comboState == task->getState()) {
            ui->stateCombo->setCurrentIndex(i);
            break;
        }
    }
    ui->stateCombo->blockSignals(false);

    qDebug() << "Clearing prevsListWidget...";
    ui->prevsListWidget->clear();

    qDebug() << "Getting prevs list...";
    const QList<TodoItem*>& prevs = task->getPrevs();
    qDebug() << "Number of prevs:" << prevs.size();

    qDebug() << "Iterating through prevs...";
    for (int i = 0; i < prevs.size(); ++i) {
        TodoItem* prev = prevs[i];
        qDebug() << "  Prev" << i << ":" << (prev ? "valid" : "NULL");

        if (prev) {
            qDebug() << "    Getting title...";
            QString title = prev->getTitle();
            qDebug() << "    Title:" << title;

            qDebug() << "    Adding to list widget...";
            ui->prevsListWidget->addItem(QString("← %1").arg(title));
            qDebug() << "    Added successfully";
        }
    }

    qDebug() << "Checking if composite...";
    if (task->isComposite()) {
        qDebug() << "Task is composite, getting info...";
        Composite* comp = static_cast<Composite*>(task);
        QString info = tr("Composite: %1 subtask(s), %2% completed")
                           .arg(comp->getChildrenCount())
                           .arg(comp->getCompletionPercentage());
        ui->statusBar->showMessage(info, 3000);
    }

    qDebug() << "<<< loadTaskDetails END";
}

void MainWindow::clearDetailPanel()
{
    m_currentTask = nullptr;

    ui->taskIdLabel->setText(tr("No task selected"));
    ui->titleEdit->clear();
    ui->descriptionEdit->clear();
    ui->dueDateEdit->setDate(QDate::currentDate().addDays(7));
    ui->stateCombo->setCurrentIndex(0);
    ui->prevsListWidget->clear();

}

void MainWindow::updateDetailPanelState(bool enabled)
{
    ui->titleEdit->setEnabled(enabled);
    ui->descriptionEdit->setEnabled(enabled);
    ui->dueDateEdit->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);
    ui->cancelButton->setEnabled(enabled);
    ui->prevsListWidget->setEnabled(enabled);

}




void MainWindow::on_prevsListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);

    if (!current)
        return;

    int id = current->data(Qt::UserRole).toInt();
    qDebug() << "Prev sélectionné ID =" << id;
}

void MainWindow::addPrevFromAllItems()
{
    ui->prevsListWidget->clear();

    if (!m_currentTask)
        return;

    const QList<TodoItem*>& allTasks = m_model->getAllTasks();
    const QList<TodoItem*>& currentPrevs = m_currentTask->getPrevs();

    for (TodoItem* task : allTasks) {

        if (task == m_currentTask)
            continue;

        QListWidgetItem* item =
            new QListWidgetItem(task->getTitle());

        item->setData(Qt::UserRole,
                      QVariant::fromValue<void*>(task));

        if (m_editPrevsMode) {
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(
                currentPrevs.contains(task)
                    ? Qt::Checked
                    : Qt::Unchecked
                );
        }

        ui->prevsListWidget->addItem(item);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!m_currentTask) {
        QMessageBox::warning(
            this,
            tr("No task selected"),
            tr("Please select a task first.")
            );
        return;
    }

    m_editPrevsMode = true;
    addPrevFromAllItems();
}


void MainWindow::on_pushButton_clicked()
{

    if (!m_currentTask)
        return;
    QList<TodoItem*> newPrevs;

    for (int i = 0; i < ui->prevsListWidget->count(); ++i) {
        QListWidgetItem* item = ui->prevsListWidget->item(i);

        if (item->checkState() == Qt::Checked) {
            TodoItem* prev =
                static_cast<TodoItem*>(
                    item->data(Qt::UserRole).value<void*>()
                    );
            if (prev)
                newPrevs.append(prev);
        }
    }
    m_currentTask->setPrev(newPrevs);

    m_editPrevsMode = false;
    addPrevFromAllItems();

    m_hasUnsavedChanges = true;
}
