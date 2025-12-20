/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNewProject;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionImporter;
    QAction *actionExporter;
    QAction *actionQuit;
    QAction *actionAddTask;
    QAction *actionDelete;
    QAction *actionAbout;
    QAction *actionAddChild;
    QAction *actionAddComposite;
    QAction *actionAdd_SubTaskComposite;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *Search;
    QLineEdit *searchBox;
    QHBoxLayout *horizontalLayout_7;
    QTreeView *treeView;
    QWidget *detailPanel;
    QVBoxLayout *verticalLayout_8;
    QLabel *TaskDetails;
    QGridLayout *gridLayout;
    QLineEdit *titleEdit;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *taskIdLabel;
    QLabel *label;
    QLabel *label_2;
    QDateEdit *dueDateEdit;
    QComboBox *stateCombo;
    QLabel *label_7;
    QTextEdit *descriptionEdit;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout;
    QLabel *label_8;
    QListWidget *prevsListWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_9;
    QListWidget *nextsListWidget;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QMenu *menuEdition;
    QMenu *menuAbout;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 700);
        actionNewProject = new QAction(MainWindow);
        actionNewProject->setObjectName("actionNewProject");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        actionNewProject->setIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen));
        actionOpen->setIcon(icon1);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
        actionSave->setIcon(icon2);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName("actionSaveAs");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs));
        actionSaveAs->setIcon(icon3);
        actionImporter = new QAction(MainWindow);
        actionImporter->setObjectName("actionImporter");
        actionExporter = new QAction(MainWindow);
        actionExporter->setObjectName("actionExporter");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        actionAddTask = new QAction(MainWindow);
        actionAddTask->setObjectName("actionAddTask");
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName("actionDelete");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionAddChild = new QAction(MainWindow);
        actionAddChild->setObjectName("actionAddChild");
        actionAddComposite = new QAction(MainWindow);
        actionAddComposite->setObjectName("actionAddComposite");
        actionAdd_SubTaskComposite = new QAction(MainWindow);
        actionAdd_SubTaskComposite->setObjectName("actionAdd_SubTaskComposite");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(11, -1, -1, -1);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        Search = new QLabel(centralwidget);
        Search->setObjectName("Search");

        horizontalLayout_5->addWidget(Search, 0, Qt::AlignmentFlag::AlignTop);

        searchBox = new QLineEdit(centralwidget);
        searchBox->setObjectName("searchBox");

        horizontalLayout_5->addWidget(searchBox, 0, Qt::AlignmentFlag::AlignTop);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        treeView = new QTreeView(centralwidget);
        treeView->setObjectName("treeView");
        treeView->setAlternatingRowColors(true);

        horizontalLayout_7->addWidget(treeView);

        detailPanel = new QWidget(centralwidget);
        detailPanel->setObjectName("detailPanel");
        verticalLayout_8 = new QVBoxLayout(detailPanel);
        verticalLayout_8->setObjectName("verticalLayout_8");
        TaskDetails = new QLabel(detailPanel);
        TaskDetails->setObjectName("TaskDetails");
        TaskDetails->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_8->addWidget(TaskDetails, 0, Qt::AlignmentFlag::AlignTop);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        titleEdit = new QLineEdit(detailPanel);
        titleEdit->setObjectName("titleEdit");

        gridLayout->addWidget(titleEdit, 1, 1, 1, 1);

        label_5 = new QLabel(detailPanel);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        label_6 = new QLabel(detailPanel);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        taskIdLabel = new QLabel(detailPanel);
        taskIdLabel->setObjectName("taskIdLabel");
        taskIdLabel->setStyleSheet(QString::fromUtf8("color: gray; font-size: 9pt;"));
        taskIdLabel->setWordWrap(true);

        gridLayout->addWidget(taskIdLabel, 0, 1, 1, 1);

        label = new QLabel(detailPanel);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(detailPanel);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        dueDateEdit = new QDateEdit(detailPanel);
        dueDateEdit->setObjectName("dueDateEdit");
        dueDateEdit->setCalendarPopup(true);

        gridLayout->addWidget(dueDateEdit, 2, 1, 1, 1);

        stateCombo = new QComboBox(detailPanel);
        stateCombo->addItem(QString());
        stateCombo->addItem(QString());
        stateCombo->addItem(QString());
        stateCombo->addItem(QString());
        stateCombo->setObjectName("stateCombo");

        gridLayout->addWidget(stateCombo, 3, 1, 1, 1);


        verticalLayout_8->addLayout(gridLayout);

        label_7 = new QLabel(detailPanel);
        label_7->setObjectName("label_7");

        verticalLayout_8->addWidget(label_7);

        descriptionEdit = new QTextEdit(detailPanel);
        descriptionEdit->setObjectName("descriptionEdit");
        descriptionEdit->setMaximumSize(QSize(16777215, 100));

        verticalLayout_8->addWidget(descriptionEdit);

        groupBox = new QGroupBox(detailPanel);
        groupBox->setObjectName("groupBox");
        groupBox->setMaximumSize(QSize(16777215, 200));
        horizontalLayout_9 = new QHBoxLayout(groupBox);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");

        verticalLayout->addWidget(label_8);

        prevsListWidget = new QListWidget(groupBox);
        prevsListWidget->setObjectName("prevsListWidget");
        prevsListWidget->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(prevsListWidget);


        horizontalLayout_9->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");

        verticalLayout_3->addWidget(label_9);

        nextsListWidget = new QListWidget(groupBox);
        nextsListWidget->setObjectName("nextsListWidget");
        nextsListWidget->setMaximumSize(QSize(16777215, 100));

        verticalLayout_3->addWidget(nextsListWidget);


        horizontalLayout_9->addLayout(verticalLayout_3);


        verticalLayout_8->addWidget(groupBox);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer);

        saveButton = new QPushButton(detailPanel);
        saveButton->setObjectName("saveButton");
        saveButton->setStyleSheet(QString::fromUtf8("background-color: #4CAF50; color: white; padding: 5px 15px;"));

        horizontalLayout_10->addWidget(saveButton);

        cancelButton = new QPushButton(detailPanel);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setStyleSheet(QString::fromUtf8("background-color: #f44336; color: white; padding: 5px 15px;"));

        horizontalLayout_10->addWidget(cancelButton);


        verticalLayout_8->addLayout(horizontalLayout_10);


        horizontalLayout_7->addWidget(detailPanel);


        verticalLayout_2->addLayout(horizontalLayout_7);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 25));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName("menuFichier");
        menuEdition = new QMenu(menubar);
        menuEdition->setObjectName("menuEdition");
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName("menuAbout");
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setMovable(false);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menuFichier->menuAction());
        menubar->addAction(menuEdition->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuFichier->addAction(actionNewProject);
        menuFichier->addAction(actionOpen);
        menuFichier->addAction(actionSave);
        menuFichier->addAction(actionSaveAs);
        menuFichier->addAction(actionImporter);
        menuFichier->addAction(actionExporter);
        menuFichier->addAction(actionQuit);
        menuEdition->addAction(actionAddTask);
        menuEdition->addAction(actionAddChild);
        menuEdition->addAction(actionAdd_SubTaskComposite);
        menuEdition->addAction(actionDelete);
        menuEdition->addAction(actionAddComposite);
        menuAbout->addAction(actionAbout);
        toolBar->addAction(actionNewProject);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionSaveAs);
        toolBar->addAction(actionImporter);
        toolBar->addAction(actionExporter);
        toolBar->addAction(actionAddChild);
        toolBar->addAction(actionAdd_SubTaskComposite);
        toolBar->addAction(actionAddTask);
        toolBar->addAction(actionAddComposite);
        toolBar->addAction(actionDelete);
        toolBar->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "KIRA - Gestionnaire de T\303\242ches", nullptr));
        actionNewProject->setText(QCoreApplication::translate("MainWindow", "New Project", nullptr));
#if QT_CONFIG(shortcut)
        actionNewProject->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open..", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveAs->setText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
#if QT_CONFIG(shortcut)
        actionSaveAs->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionImporter->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
        actionExporter->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAddTask->setText(QCoreApplication::translate("MainWindow", "Add Task", nullptr));
        actionDelete->setText(QCoreApplication::translate("MainWindow", "Delete Task", nullptr));
#if QT_CONFIG(tooltip)
        actionDelete->setToolTip(QCoreApplication::translate("MainWindow", "Delete Task", nullptr));
#endif // QT_CONFIG(tooltip)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "Info", nullptr));
        actionAddChild->setText(QCoreApplication::translate("MainWindow", "Add SubTask", nullptr));
        actionAddComposite->setText(QCoreApplication::translate("MainWindow", "Add Composite", nullptr));
        actionAdd_SubTaskComposite->setText(QCoreApplication::translate("MainWindow", "Add SubTaskComposite", nullptr));
#if QT_CONFIG(tooltip)
        centralwidget->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"justify\"><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        Search->setText(QCoreApplication::translate("MainWindow", "Search:", nullptr));
        TaskDetails->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:700; font-style:italic; color:#55aa00;\">Task Details: </span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Due Date:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "State:", nullptr));
        taskIdLabel->setText(QCoreApplication::translate("MainWindow", "No Task Selected", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID: ", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Title: ", nullptr));
        stateCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Not_Ready", nullptr));
        stateCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Ready_Todo", nullptr));
        stateCombo->setItemText(2, QCoreApplication::translate("MainWindow", "In_Progress", nullptr));
        stateCombo->setItemText(3, QCoreApplication::translate("MainWindow", "Done", nullptr));

        label_7->setText(QCoreApplication::translate("MainWindow", "Description :", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Dependencies", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Predecessors: ", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Successors:", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        cancelButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        menuFichier->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        menuEdition->setTitle(QCoreApplication::translate("MainWindow", "Edition", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
