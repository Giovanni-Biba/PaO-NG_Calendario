#include "home.h"
#include "searchbar.h"
#include "archivioimpegni.h"
#include "agenda.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<std::shared_ptr<Agenda>>("std::shared_ptr<Agenda>");
    ArchivioImpegni::instance().caricaDefault();

    Home Calendario;
    Calendario.showMaximized();
    return a.exec();
}
