
#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>
#include <QHeaderView>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_model(new TodoModel(this))
    , m_currentTask(nullptr)
    , m_hasUnsavedChanges(false)
{
    setWindowTitle("KIRA - Gestionnaire de Tâches");
    resize(1200, 700);

    createActions();
    createMenus();
    createToolBar();
    createMainWidget();

    statusBar()->showMessage("Prêt", 3000);

    // Ajouter quelques tâches de démonstration
    TodoItem* task1 = new TodoItem("Bienvenue dans KIRA", "Gestionnaire de tâches hiérarchiques");
    m_model->addRootTask(task1);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    // Actions Fichier
    m_newProjectAction = new QAction(tr("&Nouveau Projet"), this);
    m_newProjectAction->setShortcut(QKeySequence::New);
    m_newProjectAction->setStatusTip(tr("Créer un nouveau projet"));
    connect(m_newProjectAction, &QAction::triggered, this, &MainWindow::onNewProject);

    m_openProjectAction = new QAction(tr("&Ouvrir..."), this);
    m_openProjectAction->setShortcut(QKeySequence::Open);
    m_openProjectAction->setStatusTip(tr("Ouvrir un projet existant"));
    connect(m_openProjectAction, &QAction::triggered, this, &MainWindow::onOpenProject);

    m_saveProjectAction = new QAction(tr("&Enregistrer"), this);
    m_saveProjectAction->setShortcut(QKeySequence::Save);
    m_saveProjectAction->setStatusTip(tr("Enregistrer le projet"));
    connect(m_saveProjectAction, &QAction::triggered, this, &MainWindow::onSaveProject);

    m_saveProjectAsAction = new QAction(tr("Enregistrer &sous..."), this);
    m_saveProjectAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveProjectAsAction->setStatusTip(tr("Enregistrer le projet sous un nouveau nom"));
    connect(m_saveProjectAsAction, &QAction::triggered, this, &MainWindow::onSaveProjectAs);

    m_quitAction = new QAction(tr("&Quitter"), this);
    m_quitAction->setShortcut(QKeySequence::Quit);
    m_quitAction->setStatusTip(tr("Quitter l'application"));
    connect(m_quitAction, &QAction::triggered, this, &MainWindow::onQuit);

    // Actions Édition
    m_addRootTaskAction = new QAction(tr("Ajouter une &Tâche"), this);
    m_addRootTaskAction->setShortcut(Qt::CTRL | Qt::Key_T);
    m_addRootTaskAction->setStatusTip(tr("Ajouter une nouvelle tâche racine"));
    connect(m_addRootTaskAction, &QAction::triggered, this, &MainWindow::onAddRootTask);

    m_addChildTaskAction = new QAction(tr("Ajouter une &Sous-tâche"), this);
    m_addChildTaskAction->setShortcut(Qt::CTRL | Qt::Key_S);
    m_addChildTaskAction->setStatusTip(tr("Ajouter une sous-tâche à la tâche sélectionnée"));
    connect(m_addChildTaskAction, &QAction::triggered, this, &MainWindow::onAddChildTask);

    m_addCompositeAction = new QAction(tr("Ajouter un &Projet"), this);
    m_addCompositeAction->setShortcut(Qt::CTRL | Qt::Key_P);
    m_addCompositeAction->setStatusTip(tr("Ajouter un nouveau projet composite"));
    connect(m_addCompositeAction, &QAction::triggered, this, &MainWindow::onAddComposite);

    m_deleteTaskAction = new QAction(tr("&Supprimer"), this);
    m_deleteTaskAction->setShortcut(QKeySequence::Delete);
    m_deleteTaskAction->setStatusTip(tr("Supprimer la tâche sélectionnée"));
    connect(m_deleteTaskAction, &QAction::triggered, this, &MainWindow::onDeleteTask);

    // Actions Affichage
    m_expandAllAction = new QAction(tr("Tout &Développer"), this);
    m_expandAllAction->setStatusTip(tr("Développer toutes les tâches"));
    connect(m_expandAllAction, &QAction::triggered, this, &MainWindow::expandAll);

    m_collapseAllAction = new QAction(tr("Tout &Réduire"), this);
    m_collapseAllAction->setStatusTip(tr("Réduire toutes les tâches"));
    connect(m_collapseAllAction, &QAction::triggered, this, &MainWindow::collapseAll);

    // Actions Aide
    m_aboutAction = new QAction(tr("À &propos"), this);
    m_aboutAction->setStatusTip(tr("À propos de KIRA"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::createMenus()
{
    // Menu Fichier
    QMenu* fileMenu = menuBar()->addMenu(tr("&Fichier"));
    fileMenu->addAction(m_newProjectAction);
    fileMenu->addAction(m_openProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_saveProjectAction);
    fileMenu->addAction(m_saveProjectAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_quitAction);

    // Menu Édition
    QMenu* editMenu = menuBar()->addMenu(tr("&Édition"));
    editMenu->addAction(m_addRootTaskAction);
    editMenu->addAction(m_addChildTaskAction);
    editMenu->addAction(m_addCompositeAction);
    editMenu->addSeparator();
    editMenu->addAction(m_deleteTaskAction);

    // Menu Affichage
    QMenu* viewMenu = menuBar()->addMenu(tr("&Affichage"));
    viewMenu->addAction(m_expandAllAction);
    viewMenu->addAction(m_collapseAllAction);

    // Menu Aide
    QMenu* helpMenu = menuBar()->addMenu(tr("&Aide"));
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBar()
{
    QToolBar* toolbar = addToolBar(tr("Barre d'outils"));
    toolbar->setMovable(false);

    toolbar->addAction(m_newProjectAction);
    toolbar->addAction(m_openProjectAction);
    toolbar->addAction(m_saveProjectAction);
    toolbar->addSeparator();
    toolbar->addAction(m_addRootTaskAction);
    toolbar->addAction(m_addChildTaskAction);
    toolbar->addAction(m_addCompositeAction);
    toolbar->addAction(m_deleteTaskAction);
    toolbar->addSeparator();
    toolbar->addAction(m_expandAllAction);
    toolbar->addAction(m_collapseAllAction);
}

void MainWindow::createMainWidget()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Barre de recherche
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* searchLabel = new QLabel(tr("Rechercher:"), this);
    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText(tr("Entrez un mot-clé..."));
    connect(m_searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchBox);
    mainLayout->addLayout(searchLayout);

    // Splitter pour diviser TreeView et Panneau de détails
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // TreeView (gauche)
    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_model);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    m_treeView->setAlternatingRowColors(true);
    m_treeView->header()->setStretchLastSection(true);
    m_treeView->setColumnWidth(0, 250);
    m_treeView->setColumnWidth(1, 150);
    m_treeView->setColumnWidth(2, 100);

    connect(m_treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onTaskSelectionChanged);

    splitter->addWidget(m_treeView);

    // Panneau de détails (droite)
    createDetailPanel();
    splitter->addWidget(m_detailPanel);

    // Ratio 60/40
    splitter->setStretchFactor(0, 6);
    splitter->setStretchFactor(1, 4);

    mainLayout->addWidget(splitter);

    setCentralWidget(centralWidget);

    // Désactiver le panneau de détails au démarrage
    updateDetailPanelState(false);
}

void MainWindow::createDetailPanel()
{
    m_detailPanel = new QWidget(this);
    QVBoxLayout* detailLayout = new QVBoxLayout(m_detailPanel);

    // Titre du panneau
    QLabel* titleLabel = new QLabel(tr("<h3>Détails de la Tâche</h3>"), this);
    detailLayout->addWidget(titleLabel);

    // Formulaire de détails
    QGridLayout* formLayout = new QGridLayout();

    // ID (lecture seule)
    formLayout->addWidget(new QLabel(tr("ID:"), this), 0, 0);
    m_taskIdLabel = new QLabel(tr("Aucune tâche sélectionnée"), this);
    m_taskIdLabel->setWordWrap(true);
    m_taskIdLabel->setStyleSheet("QLabel { color: gray; font-size: 9pt; }");
    formLayout->addWidget(m_taskIdLabel, 0, 1);

    // Titre
    formLayout->addWidget(new QLabel(tr("Titre:"), this), 1, 0);
    m_titleEdit = new QLineEdit(this);
    formLayout->addWidget(m_titleEdit, 1, 1);

    // Date limite
    formLayout->addWidget(new QLabel(tr("Date limite:"), this), 2, 0);
    m_dueDateEdit = new QDateEdit(this);
    m_dueDateEdit->setCalendarPopup(true);
    m_dueDateEdit->setDate(QDate::currentDate().addDays(7));
    formLayout->addWidget(m_dueDateEdit, 2, 1);

    // État
    formLayout->addWidget(new QLabel(tr("État:"), this), 3, 0);
    m_stateCombo = new QComboBox(this);
    m_stateCombo->addItem(tr("Pas Prêt"), static_cast<int>(TodoState::Not_Ready));
    m_stateCombo->addItem(tr("Prêt"), static_cast<int>(TodoState::Ready_Todo));
    m_stateCombo->addItem(tr("En Cours"), static_cast<int>(TodoState::In_Progress));
    m_stateCombo->addItem(tr("Terminé"), static_cast<int>(TodoState::Done));
    connect(m_stateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStateChanged);
    formLayout->addWidget(m_stateCombo, 3, 1);

    detailLayout->addLayout(formLayout);

    // Description
    QLabel* descLabel = new QLabel(tr("Description:"), this);
    detailLayout->addWidget(descLabel);
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setMaximumHeight(150);
    detailLayout->addWidget(m_descriptionEdit);

    // Dépendances
    QGroupBox* depsGroup = new QGroupBox(tr("Dépendances"), this);
    QHBoxLayout* depsLayout = new QHBoxLayout(depsGroup);

    QVBoxLayout* prevsLayout = new QVBoxLayout();
    prevsLayout->addWidget(new QLabel(tr("Prédécesseurs:"), this));
    m_prevsListWidget = new QListWidget(this);
    m_prevsListWidget->setMaximumHeight(100);
    prevsLayout->addWidget(m_prevsListWidget);
    depsLayout->addLayout(prevsLayout);

    QVBoxLayout* nextsLayout = new QVBoxLayout();
    nextsLayout->addWidget(new QLabel(tr("Successeurs:"), this));
    m_nextsListWidget = new QListWidget(this);
    m_nextsListWidget->setMaximumHeight(100);
    nextsLayout->addWidget(m_nextsListWidget);
    depsLayout->addLayout(nextsLayout);

    detailLayout->addWidget(depsGroup);

    // Boutons d'action
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_saveButton = new QPushButton(tr("Sauvegarder"), this);
    m_saveButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; }");
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::onSaveDetails);
    buttonLayout->addWidget(m_saveButton);

    m_cancelButton = new QPushButton(tr("Annuler"), this);
    m_cancelButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 5px 15px; }");
    connect(m_cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelDetails);
    buttonLayout->addWidget(m_cancelButton);

    detailLayout->addLayout(buttonLayout);

    detailLayout->addStretch();
}


void MainWindow::onNewProject()
{
    if (m_hasUnsavedChanges) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, tr("Nouveau Projet"),
            tr("Voulez-vous sauvegarder les modifications?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            onSaveProject();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    m_model->clear();
    m_currentFilePath.clear();
    m_hasUnsavedChanges = false;
    clearDetailPanel();
    statusBar()->showMessage(tr("Nouveau projet créé"), 3000);
}

void MainWindow::onOpenProject()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Ouvrir un projet"),
        QString(), tr("Fichiers KIRA (*.kira *.json *.xml)"));

    if (!fileName.isEmpty()) {
        // TODO: Implémenter le chargement depuis fichier
        statusBar()->showMessage(tr("Ouverture de: %1").arg(fileName), 3000);
        m_currentFilePath = fileName;
    }
}

void MainWindow::onSaveProject()
{
    if (m_currentFilePath.isEmpty()) {
        onSaveProjectAs();
    } else {
        // TODO: Implémenter la sauvegarde
        m_hasUnsavedChanges = false;
        statusBar()->showMessage(tr("Projet sauvegardé: %1").arg(m_currentFilePath), 3000);
    }
}

void MainWindow::onSaveProjectAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Enregistrer le projet"),
        QString(), tr("Fichiers KIRA (*.kira);;JSON (*.json);;XML (*.xml)"));

    if (!fileName.isEmpty()) {
        m_currentFilePath = fileName;
        onSaveProject();
    }
}

void MainWindow::onQuit()
{
    if (m_hasUnsavedChanges) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, tr("Quitter"),
            tr("Voulez-vous sauvegarder avant de quitter?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            onSaveProject();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    close();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("À propos de KIRA"),
                       tr("<h2>KIRA - Gestionnaire de Tâches</h2>"
                          "<p>Version 1.0</p>"
                          "<p>Gestionnaire de tâches hiérarchiques avec dépendances</p>"
                          "<p><b>Auteurs:</b> DABBEBI MOHAMED, DJEBBI Yesmine</p>"
                          "<p>Projet LAO - 2025</p>"));
}


void MainWindow::onAddRootTask()
{
    TodoItem* task = new TodoItem(tr("Nouvelle Tâche"), tr("Description de la tâche"));
    m_model->addRootTask(task);
    m_hasUnsavedChanges = true;

    // Sélectionner la nouvelle tâche
    QModelIndex index = m_model->getIndexFromTask(task);
    m_treeView->setCurrentIndex(index);

    statusBar()->showMessage(tr("Tâche ajoutée"), 2000);
}

void MainWindow::onAddChildTask()
{
    QModelIndex current = m_treeView->currentIndex();
    if (!current.isValid()) {
        QMessageBox::warning(this, tr("Ajouter une sous-tâche"),
                             tr("Veuillez sélectionner une tâche composite."));
        return;
    }

    TodoItem* parent = m_model->getTask(current);
    if (!parent || !parent->isComposite()) {
        QMessageBox::warning(this, tr("Ajouter une sous-tâche"),
                             tr("La tâche sélectionnée n'est pas un composite."));
        return;
    }

    TodoItem* child = new TodoItem(tr("Nouvelle Sous-tâche"), tr("Description"));
    m_model->addChildTask(current, child);
    m_hasUnsavedChanges = true;

    m_treeView->expand(current);
    statusBar()->showMessage(tr("Sous-tâche ajoutée"), 2000);
}

void MainWindow::onAddComposite()
{
    Composite* composite = new Composite(tr("Nouveau Projet"), tr("Description du projet"));
    m_model->addRootTask(composite);
    m_hasUnsavedChanges = true;

    QModelIndex index = m_model->getIndexFromTask(composite);
    m_treeView->setCurrentIndex(index);

    statusBar()->showMessage(tr("Projet composite ajouté"), 2000);
}

void MainWindow::onDeleteTask()
{
    QModelIndex current = m_treeView->currentIndex();
    if (!current.isValid()) {
        QMessageBox::warning(this, tr("Supprimer"),
                             tr("Veuillez sélectionner une tâche à supprimer."));
        return;
    }

    TodoItem* task = m_model->getTask(current);
    if (!task) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Supprimer"),
        tr("Êtes-vous sûr de vouloir supprimer '%1'?").arg(task->getTitle()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_model->removeTask(current);
        m_hasUnsavedChanges = true;
        clearDetailPanel();
        statusBar()->showMessage(tr("Tâche supprimée"), 2000);
    }
}


void MainWindow::onTaskSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!current.isValid()) {
        clearDetailPanel();
        updateDetailPanelState(false);
        return;
    }

    TodoItem* task = m_model->getTask(current);
    if (task) {
        loadTaskDetails(task);
        updateDetailPanelState(true);
    }
}

void MainWindow::onSaveDetails()
{
    if (!m_currentTask) return;

    m_currentTask->setTitle(m_titleEdit->text());
    m_currentTask->setDescription(m_descriptionEdit->toPlainText());
    m_currentTask->setDueDate(m_dueDateEdit->date());

    // L'état est déjà sauvegardé via onStateChanged

    m_hasUnsavedChanges = true;
    statusBar()->showMessage(tr("Modifications sauvegardées"), 2000);
}

void MainWindow::onCancelDetails()
{
    if (m_currentTask) {
        loadTaskDetails(m_currentTask);
        statusBar()->showMessage(tr("Modifications annulées"), 2000);
    }
}

void MainWindow::onStateChanged(int index)
{
    if (!m_currentTask) return;

    TodoState newState = static_cast<TodoState>(m_stateCombo->itemData(index).toInt());
    m_currentTask->setState(newState);
    m_hasUnsavedChanges = true;
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    // TODO: Implémenter la recherche/filtre
    if (text.isEmpty()) {
        statusBar()->clearMessage();
    } else {
        statusBar()->showMessage(tr("Recherche: %1").arg(text));
    }
}


void MainWindow::loadTaskDetails(TodoItem* task)
{
    m_currentTask = task;

    m_taskIdLabel->setText(task->getId());
    m_titleEdit->setText(task->getTitle());
    m_descriptionEdit->setPlainText(task->getDescription());
    m_dueDateEdit->setDate(task->getDueDate());

    // Sélectionner l'état correct
    for (int i = 0; i < m_stateCombo->count(); ++i) {
        if (static_cast<TodoState>(m_stateCombo->itemData(i).toInt()) == task->getState()) {
            m_stateCombo->setCurrentIndex(i);
            break;
        }
    }

    // Afficher les dépendances
    m_prevsListWidget->clear();
    for (TodoItem* prev : task->getPrevs()) {
        m_prevsListWidget->addItem(prev->getTitle());
    }

    m_nextsListWidget->clear();
    for (TodoItem* next : task->getNexts()) {
        m_nextsListWidget->addItem(next->getTitle());
    }
}

void MainWindow::clearDetailPanel()
{
    m_currentTask = nullptr;
    m_taskIdLabel->setText(tr("Aucune tâche sélectionnée"));
    m_titleEdit->clear();
    m_descriptionEdit->clear();
    m_dueDateEdit->setDate(QDate::currentDate().addDays(7));
    m_stateCombo->setCurrentIndex(0);
    m_prevsListWidget->clear();
    m_nextsListWidget->clear();
}

void MainWindow::updateDetailPanelState(bool enabled)
{
    m_titleEdit->setEnabled(enabled);
    m_descriptionEdit->setEnabled(enabled);
    m_dueDateEdit->setEnabled(enabled);
    m_stateCombo->setEnabled(enabled);
    m_saveButton->setEnabled(enabled);
    m_cancelButton->setEnabled(enabled);
}

TodoItem* MainWindow::getSelectedTask() const
{
    QModelIndex current = m_treeView->currentIndex();
    if (!current.isValid()) {
        return nullptr;
    }
    return m_model->getTask(current);
}

void MainWindow::expandAll()
{
    m_treeView->expandAll();
    statusBar()->showMessage(tr("Toutes les tâches développées"), 2000);
}

void MainWindow::collapseAll()
{
    m_treeView->collapseAll();
    statusBar()->showMessage(tr("Toutes les tâches réduites"), 2000);
}
