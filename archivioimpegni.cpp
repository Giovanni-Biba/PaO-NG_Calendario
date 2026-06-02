#include "archivioimpegni.h"

#include "appuntamento.h"
#include "attivita.h"
#include "consegna.h"
#include "datafiles.h"
#include "evento.h"
#include "festivita.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

ArchivioImpegni::ArchivioImpegni()
    : percorsoJson(DataFiles::path("datiAttivitaFestivita.json")),
      percorsoXml(DataFiles::path("datiEventoAppuntamento.xml"))
{
}

ArchivioImpegni &ArchivioImpegni::instance()
{
    static ArchivioImpegni archivio;
    return archivio;
}

QVector<std::shared_ptr<Agenda>> ArchivioImpegni::tutti() const
{
    return impegni;
}

QVector<std::shared_ptr<Agenda>> ArchivioImpegni::cerca(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita) const
{
    QVector<std::shared_ptr<Agenda>> risultati;
    for (const auto &impegno : impegni) {
        if (impegno && impegno->matchesFiltro(titolo, data, tipo, priorita))
            risultati.append(impegno);
    }
    return risultati;
}

bool ArchivioImpegni::caricaDefault()
{
    impegni.clear();
    const bool okJson = caricaJson();
    const bool okXml = caricaXml();
    return okJson || okXml;
}

bool ArchivioImpegni::caricaDaFileJson(const QString &path)
{
    percorsoJson = path;
    impegni.clear();
    const bool okJson = caricaJson();
    caricaXml();
    return okJson;
}

bool ArchivioImpegni::caricaDaFileXml(const QString &path)
{
    percorsoXml = path;
    impegni.clear();
    caricaJson();
    return caricaXml();
}

bool ArchivioImpegni::salvaSuFileJson(const QString &path)
{
    percorsoJson = path;
    return salvaJson();
}

bool ArchivioImpegni::salvaSuFileXml(const QString &path)
{
    percorsoXml = path;
    return salvaXml();
}

bool ArchivioImpegni::aggiungi(const std::shared_ptr<Agenda> &impegno)
{
    if (!impegno)
        return false;
    impegni.append(impegno);
    return salvaTutto();
}

bool ArchivioImpegni::aggiorna(const std::shared_ptr<Agenda> &originale, const std::shared_ptr<Agenda> &modificato)
{
    if (!originale || !modificato)
        return false;

    for (int i = 0; i < impegni.size(); ++i) {
        if (impegni[i] == originale || impegni[i]->stessaChiave(*originale)) {
            impegni[i] = modificato;
            return salvaTutto();
        }
    }
    return false;
}

bool ArchivioImpegni::elimina(const std::shared_ptr<Agenda> &impegno)
{
    if (!impegno)
        return false;

    for (int i = 0; i < impegni.size(); ++i) {
        if (impegni[i] == impegno || impegni[i]->stessaChiave(*impegno)) {
            impegni.removeAt(i);
            return salvaTutto();
        }
    }
    return false;
}

bool ArchivioImpegni::salvaTutto() const
{
    return salvaJson() && salvaXml();
}

QString ArchivioImpegni::getPercorsoJson() const { return percorsoJson; }
QString ArchivioImpegni::getPercorsoXml() const { return percorsoXml; }

std::shared_ptr<Agenda> ArchivioImpegni::creaDaJson(const QJsonObject &json) const
{
    const QString tipo = json["tipo"].toString().toLower();
    std::shared_ptr<Agenda> impegno;

    if (tipo == "attivita" || tipo == "attività")
        impegno = std::make_shared<Attivita>();
    else if (tipo == "evento")
        impegno = std::make_shared<Evento>();
    else if (tipo == "appuntamento")
        impegno = std::make_shared<Appuntamento>();
    else if (tipo == "festivita" || tipo == "festività")
        impegno = std::make_shared<Festivita>();
    else if (tipo == "consegna")
        impegno = std::make_shared<Consegna>();

    if (impegno)
        impegno->fromJson(json);

    return impegno;
}

bool ArchivioImpegni::caricaJson()
{
    QFile file(percorsoJson);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    const QJsonArray array = doc.isArray() ? doc.array() : doc.object().value("agenda").toArray();
    for (const QJsonValue &value : array) {
        const auto impegno = creaDaJson(value.toObject());
        if (impegno)
            impegni.append(impegno);
    }
    return true;
}

bool ArchivioImpegni::caricaXml()
{
    QFile file(percorsoXml);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader reader(&file);
    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNext() == QXmlStreamReader::StartElement && reader.name() == "item") {
            QJsonObject obj;
            while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "item")) {
                if (reader.readNext() == QXmlStreamReader::StartElement)
                    obj.insert(reader.name().toString(), reader.readElementText());
            }

            const auto impegno = creaDaJson(obj);
            if (impegno)
                impegni.append(impegno);
        }
    }

    file.close();
    return !reader.hasError();
}

bool ArchivioImpegni::salvaJson() const
{
    QFile file(percorsoJson);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QJsonArray array;
    for (const auto &impegno : impegni) {
        if (!impegno)
            continue;

        const QString tipo = impegno->getTipo();
        if (tipo == "Attivita" || tipo == "Festivita" || tipo == "Consegna") {
            QJsonObject obj;
            impegno->toJson(obj);
            array.append(obj);
        }
    }

    QJsonObject root;
    root["agenda"] = array;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool ArchivioImpegni::salvaXml() const
{
    QFile file(percorsoXml);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("dati");

    for (const auto &impegno : impegni) {
        if (!impegno)
            continue;

        const QString tipo = impegno->getTipo();
        if (tipo != "Evento" && tipo != "Appuntamento")
            continue;

        QJsonObject obj;
        impegno->toJson(obj);
        writer.writeStartElement("item");
        for (auto it = obj.begin(); it != obj.end(); ++it)
            writer.writeTextElement(it.key(), it.value().toVariant().toString());
        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
    return true;
}
