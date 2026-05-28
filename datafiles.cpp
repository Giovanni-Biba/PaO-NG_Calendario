#include "datafiles.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

namespace
{
void aggiungiSeValida(QStringList &cartelle, const QString &path)
{
    QDir dir(path);
    QString assoluto = dir.absolutePath();

    if (!cartelle.contains(assoluto))
        cartelle.append(assoluto);
}

QString cartellaEsternaAppMac()
{
    QDir dir(QCoreApplication::applicationDirPath());

    if (dir.dirName() == "MacOS" && dir.cdUp() && dir.dirName() == "Contents" && dir.cdUp()) {
        QString nomeBundle = dir.dirName();
        if (nomeBundle.endsWith(".app") && dir.cdUp())
            return dir.absolutePath();
    }

    return QString();
}

QStringList cartelleCandidate()
{
    QStringList cartelle;

    QString esternaApp = cartellaEsternaAppMac();
    if (!esternaApp.isEmpty())
        aggiungiSeValida(cartelle, esternaApp);

    aggiungiSeValida(cartelle, QDir::currentPath());
    aggiungiSeValida(cartelle, QCoreApplication::applicationDirPath());

    QStringList basi = cartelle;
    for (const QString &base : std::as_const(basi)) {
        QDir dir(base);

        for (int i = 0; i < 8 && dir.cdUp(); ++i)
            aggiungiSeValida(cartelle, dir.absolutePath());
    }

    return cartelle;
}
}

QString DataFiles::path(const QString &fileName)
{
    const QStringList cartelle = cartelleCandidate();

    for (const QString &cartella : cartelle) {
        const QString candidato = QDir(cartella).filePath(fileName);
        if (QFileInfo::exists(candidato))
            return candidato;
    }

    if (!cartelle.isEmpty())
        return QDir(cartelle.first()).filePath(fileName);

    return fileName;
}
