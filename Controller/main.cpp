
#include <QApplication>
#include "ControllerHead/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/KIRA_icon.png"));

    // Configuration de l'application
    app.setApplicationName("KIRA");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("KIRA Team");

    // Créer et afficher la fenêtre principale
    MainWindow window;
    window.show();

    return app.exec();
}
