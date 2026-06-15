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
#include <QJsonParseError>
#include <QSaveFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

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

// --- LOGICA DI RICERCA CORRETTA ---
QVector<std::shared_ptr<Agenda>> ArchivioImpegni::cerca(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita)
{
    QVector<std::shared_ptr<Agenda>> risultati;
    for (const auto &impegno : impegni) {
        if (!impegno) continue;

        bool corrisponde = true;

        // 1. Filtro Titolo: se l'utente ha scritto qualcosa, cerchiamo corrispondenza parziale
        if (!titolo.isEmpty()) {
            if (!impegno->getTitolo().contains(titolo, Qt::CaseInsensitive)) {
                corrisponde = false;
            }
        }

        // 2. Filtro Data: se la data è valida (pulsante check attivo), deve corrispondere esattamente
        if (corrisponde && data.isValid()) {
            if (impegno->getData() != data) {
                corrisponde = false;
            }
        }

        // 3. Filtro Tipo
        if (corrisponde && tipo != "Tutte") {
            if (impegno->getTipo().toLower() != tipo.toLower()) {
                corrisponde = false;
            }
        }

        // 4. Filtro Priorità (solo se l'impegno ha il metodo per la priorità)
        if (corrisponde && priorita != "Tutte") {
            auto att = impegno->usaRigaFestivita() ? std::shared_ptr<Agenda>() : impegno;
            if (att) {
                if (att->prioritaToString().toLower() != priorita.toLower()) {
                    corrisponde = false;
                }
            } else {
                corrisponde = false;
            }
        }

        if (corrisponde)
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
    if (impegni.isEmpty())
        caricaDefault();
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

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError)
        return false;

    QJsonArray array;
    if (doc.isArray()) {
        array = doc.array();
    } else if (doc.isObject() && doc.object().value("agenda").isArray()) {
        array = doc.object().value("agenda").toArray();
    } else {
        return false;
    }

    for (const QJsonValue &value : array) {
        if (!value.isObject())
            continue;
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
    QSaveFile file(percorsoJson);
    if (!file.open(QIODevice::WriteOnly))
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
    return file.commit();
}

bool ArchivioImpegni::salvaXml() const
{
    QSaveFile file(percorsoXml);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    {
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
    }

    return file.commit();
}
