#include "home.h"
#include "searchbar.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Home Calendario;
    Calendario.showMaximized();
    return a.exec();
}
