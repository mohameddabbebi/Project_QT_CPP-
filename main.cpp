#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "todoitem.h"
#include "TodoState.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Test minimal
    TodoItem* tache = new TodoItem("test", "test");
    tache->setDescription("Ceci est un test");

    qDebug() << "Titre:" << tache->getTitle();
    qDebug() << "Date limite:" << tache->getDueDate().toString("dd/MM/yyyy");

    delete tache;

    qDebug() << "Test réussi!";

    return 0;
}
