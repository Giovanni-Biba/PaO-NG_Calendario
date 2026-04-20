#include "home.h"
#include "search.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Home Calendario;
    Calendario.showMaximized();
    return QCoreApplication::exec();
}
