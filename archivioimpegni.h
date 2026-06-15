#ifndef ARCHIVIOIMPEGNI_H
#define ARCHIVIOIMPEGNI_H

#include "agenda.h"
#include <QVector>
#include <QString>
#include <QDate>
#include <QJsonObject>
#include <memory>

class ArchivioImpegni
{
private:
    QVector<std::shared_ptr<Agenda>> impegni;
    QString percorsoJson;
    QString percorsoXml;

    ArchivioImpegni();

    std::shared_ptr<Agenda> creaDaJson(const QJsonObject &json) const;
    bool salvaJson() const;
    bool salvaXml() const;
    bool caricaJson();
    bool caricaXml();

public:
    static ArchivioImpegni &instance();

    QVector<std::shared_ptr<Agenda>> tutti() const;
    QVector<std::shared_ptr<Agenda>> cerca(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita);

    bool caricaDefault();
    bool caricaDaFileJson(const QString &path);
    bool caricaDaFileXml(const QString &path);
    bool salvaSuFileJson(const QString &path);
    bool salvaSuFileXml(const QString &path);

    bool aggiungi(const std::shared_ptr<Agenda> &impegno);
    bool aggiorna(const std::shared_ptr<Agenda> &originale, const std::shared_ptr<Agenda> &modificato);
    bool elimina(const std::shared_ptr<Agenda> &impegno);
    bool salvaTutto() const;

    QString getPercorsoJson() const;
    QString getPercorsoXml() const;
};

#endif