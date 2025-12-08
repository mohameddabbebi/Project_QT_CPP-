/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNouveau;
    QAction *actionOuvrir;
    QAction *actionEnregistrer;
    QAction *actionEnregistrer_Sous;
    QAction *actionImporter;
    QAction *actionExporter;
    QAction *actionQuitter;
    QAction *actionNew_Item;
    QAction *actionRemove_Item;
    QAction *actionInfo;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QMenu *menuEdition;
    QMenu *menuAbout;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionNouveau = new QAction(MainWindow);
        actionNouveau->setObjectName("actionNouveau");
        actionOuvrir = new QAction(MainWindow);
        actionOuvrir->setObjectName("actionOuvrir");
        actionEnregistrer = new QAction(MainWindow);
        actionEnregistrer->setObjectName("actionEnregistrer");
        actionEnregistrer_Sous = new QAction(MainWindow);
        actionEnregistrer_Sous->setObjectName("actionEnregistrer_Sous");
        actionImporter = new QAction(MainWindow);
        actionImporter->setObjectName("actionImporter");
        actionExporter = new QAction(MainWindow);
        actionExporter->setObjectName("actionExporter");
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName("actionQuitter");
        actionNew_Item = new QAction(MainWindow);
        actionNew_Item->setObjectName("actionNew_Item");
        actionRemove_Item = new QAction(MainWindow);
        actionRemove_Item->setObjectName("actionRemove_Item");
        actionInfo = new QAction(MainWindow);
        actionInfo->setObjectName("actionInfo");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName("menuFichier");
        menuEdition = new QMenu(menubar);
        menuEdition->setObjectName("menuEdition");
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName("menuAbout");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFichier->menuAction());
        menubar->addAction(menuEdition->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuFichier->addAction(actionNouveau);
        menuFichier->addAction(actionOuvrir);
        menuFichier->addAction(actionEnregistrer);
        menuFichier->addAction(actionEnregistrer_Sous);
        menuFichier->addAction(actionImporter);
        menuFichier->addAction(actionExporter);
        menuFichier->addAction(actionQuitter);
        menuEdition->addAction(actionNew_Item);
        menuEdition->addAction(actionRemove_Item);
        menuAbout->addAction(actionInfo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNouveau->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
        actionOuvrir->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionEnregistrer->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionEnregistrer_Sous->setText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
        actionImporter->setText(QCoreApplication::translate("MainWindow", "Import", nullptr));
        actionExporter->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        actionQuitter->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        actionNew_Item->setText(QCoreApplication::translate("MainWindow", "Add Item", nullptr));
        actionRemove_Item->setText(QCoreApplication::translate("MainWindow", "Remove Item", nullptr));
        actionInfo->setText(QCoreApplication::translate("MainWindow", "Info", nullptr));
        menuFichier->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        menuEdition->setTitle(QCoreApplication::translate("MainWindow", "Edition", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
