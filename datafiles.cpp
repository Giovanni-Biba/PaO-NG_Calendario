#include "datafiles.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace
{
QString cartellaProgettoDaMacro()
{
#ifdef PROJECT_SOURCE_DIR
    QDir dir(QStringLiteral(PROJECT_SOURCE_DIR));
    if (QFileInfo::exists(dir.filePath("NG_Calendario.pro")))
        return dir.absolutePath();
#endif
    return QString();
}

QString cercaCartellaProgettoSalendoDa(const QString &path)
{
    QDir dir(path);

    for (int i = 0; i < 8; ++i) {
        if (QFileInfo::exists(dir.filePath("NG_Calendario.pro")))
            return dir.absolutePath();
        if (!dir.cdUp())
            break;
    }

    return QString();
}

QString cartellaProgetto()
{
    const QString daMacro = cartellaProgettoDaMacro();
    if (!daMacro.isEmpty())
        return daMacro;

    const QString daCurrentPath = cercaCartellaProgettoSalendoDa(QDir::currentPath());
    if (!daCurrentPath.isEmpty())
        return daCurrentPath;

    return cercaCartellaProgettoSalendoDa(QCoreApplication::applicationDirPath());
}
}

QString DataFiles::path(const QString &fileName)
{
    const QString progetto = cartellaProgetto();
    if (!progetto.isEmpty())
        return QDir(progetto).filePath(fileName);

    return QDir(QDir::currentPath()).filePath(fileName);
}
