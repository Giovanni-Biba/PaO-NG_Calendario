#include "visualize.h"
#include "datafiles.h"

#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLabel>
#include <QLayoutItem>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStringList>
#include <QVariant>
#include <QVBoxLayout>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

visualize::visualize(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    QHBoxLayout *topButtons = new QHBoxLayout();

    buttonIndietro = new QPushButton("← Indietro", this);
    buttonModifica = new QPushButton("Modifica", this);
    buttonElimina = new QPushButton("Elimina", this);

    buttonIndietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    buttonModifica->setStyleSheet("QPushButton { background-color: #f39c12; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    buttonElimina->setStyleSheet("QPushButton { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");

    topButtons->addWidget(buttonIndietro);
    topButtons->addStretch();
    topButtons->addWidget(buttonModifica);
    topButtons->addWidget(buttonElimina);
    mainLayout->addLayout(topButtons);

    titoloPagina = new QLabel("DETTAGLIO IMPEGNO", this);
    titoloPagina->setAlignment(Qt::AlignCenter);
    titoloPagina->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 10px 0;");
    mainLayout->addWidget(titoloPagina);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("border: none;");

    QWidget *contenuto = new QWidget(scroll);
    dettagliLayout = new QVBoxLayout(contenuto);
    dettagliLayout->setSpacing(10);
    dettagliLayout->setContentsMargins(8, 8, 8, 8);
    dettagliLayout->addStretch();

    scroll->setWidget(contenuto);
    mainLayout->addWidget(scroll);

    connect(buttonIndietro, &QPushButton::clicked, this, &visualize::tornaIndietro);
    connect(buttonModifica, &QPushButton::clicked, this, [this]() {
        emit richiestaModifica(elementoCorrente);
    });
    connect(buttonElimina, &QPushButton::clicked, this, &visualize::eliminaElemento);
}

void visualize::caricaElemento(const QJsonObject &elemento)
{
    elementoCorrente = elemento;
    aggiornaVista();
}

void visualize::caricaDaChiave(const QString &titolo, const QString &data, const QString &ora)
{
    if (!cercaElemento(titolo, data, ora)) {
        elementoCorrente = QJsonObject();
        QMessageBox::warning(this, "Elemento non trovato", "Non riesco a trovare questo impegno nei file dati.");
    }

    aggiornaVista();
}

bool visualize::cercaElemento(const QString &titolo, const QString &data, const QString &ora)
{
    QFile fileJson(DataFiles::path("datiAttivitaFestivita.json"));
    if (fileJson.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(fileJson.readAll());
        QJsonArray array = doc.isArray() ? doc.array() : doc.object().value("agenda").toArray();
        fileJson.close();

        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            if (obj.value("titolo").toString() == titolo &&
                obj.value("data").toString() == data &&
                obj.value("ora").toString() == ora) {
                elementoCorrente = obj;
                return true;
            }
        }
    }

    QFile fileXml(DataFiles::path("datiEventoAppuntamento.xml"));
    if (!fileXml.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QXmlStreamReader xml(&fileXml);
    while (!xml.atEnd() && !xml.hasError()) {
        if (xml.readNext() == QXmlStreamReader::StartElement && xml.name() == "item") {
            QJsonObject obj;
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "item")) {
                if (xml.readNext() == QXmlStreamReader::StartElement) {
                    obj.insert(xml.name().toString(), xml.readElementText());
                }
            }

            if (obj.value("titolo").toString() == titolo &&
                obj.value("data").toString() == data &&
                obj.value("ora").toString() == ora) {
                elementoCorrente = obj;
                fileXml.close();
                return true;
            }
        }
    }

    fileXml.close();
    return false;
}

void visualize::aggiornaVista()
{
    while (QLayoutItem *item = dettagliLayout->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (elementoCorrente.isEmpty()) {
        aggiungiRiga("Stato", "Nessun elemento selezionato");
        dettagliLayout->addStretch();
        return;
    }

    titoloPagina->setText(elementoCorrente.value("titolo").toString("DETTAGLIO IMPEGNO").toUpper());

    const QStringList ordineCampi = {
        "tipo", "titolo", "descrizione", "data", "ora", "durata_ore", "durata",
        "priorita", "dataFine", "oraFine", "luogo", "stato"
    };

    for (const QString &campo : ordineCampi) {
        if (elementoCorrente.contains(campo)) {
            QString nome = campo;
            if (campo == "durata") nome = "durata_ore";
            aggiungiRiga(nome, elementoCorrente.value(campo).toVariant().toString());
        }
    }

    for (auto it = elementoCorrente.begin(); it != elementoCorrente.end(); ++it) {
        if (!ordineCampi.contains(it.key())) {
            aggiungiRiga(it.key(), it.value().toVariant().toString());
        }
    }

    dettagliLayout->addStretch();
}

void visualize::aggiungiRiga(const QString &nome, const QString &valore)
{
    QFrame *card = new QFrame(this);
    card->setStyleSheet("QFrame { border: 1px solid #dfe6e9; border-radius: 8px; background-color: white; padding: 10px; }");

    QHBoxLayout *layout = new QHBoxLayout(card);

    QLabel *labelNome = new QLabel(nome.left(1).toUpper() + nome.mid(1), card);
    labelNome->setMinimumWidth(130);
    labelNome->setStyleSheet("font-weight: bold; color: #2c3e50; border: none;");

    QLabel *labelValore = new QLabel(valore.isEmpty() ? "N/D" : valore, card);
    labelValore->setWordWrap(true);
    labelValore->setStyleSheet("color: #34495e; border: none;");

    layout->addWidget(labelNome);
    layout->addWidget(labelValore, 1);
    dettagliLayout->addWidget(card);
}

QString visualize::tipoNormalizzato() const
{
    return elementoCorrente.value("tipo").toString().toLower();
}

void visualize::eliminaElemento()
{
    if (elementoCorrente.isEmpty()) return;

    const int risposta = QMessageBox::question(
        this,
        "Conferma eliminazione",
        "Vuoi eliminare definitivamente questo impegno?"
    );

    if (risposta != QMessageBox::Yes) return;

    bool eliminato = tipoNormalizzato() == "attivita" || tipoNormalizzato() == "festivita"
                         ? eliminaDaJson()
                         : eliminaDaXml();

    if (!eliminato) {
        QMessageBox::warning(this, "Errore", "Non sono riuscito a eliminare l'impegno selezionato.");
        return;
    }

    QMessageBox::information(this, "Eliminato", "Impegno eliminato correttamente.");
    emit elementoEliminato();
}

bool visualize::eliminaDaJson()
{
    QFile file(DataFiles::path("datiAttivitaFestivita.json"));
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    bool rootConAgenda = doc.isObject();
    QJsonArray array = doc.isArray() ? doc.array() : doc.object().value("agenda").toArray();
    file.close();

    QJsonArray aggiornato;
    bool trovato = false;

    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        if (!trovato && elementiUguali(obj, elementoCorrente)) {
            trovato = true;
            continue;
        }
        aggiornato.append(obj);
    }

    if (!trovato || !file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;

    if (rootConAgenda) {
        QJsonObject root = doc.object();
        root["agenda"] = aggiornato;
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    } else {
        file.write(QJsonDocument(aggiornato).toJson(QJsonDocument::Indented));
    }

    file.close();
    return true;
}

bool visualize::eliminaDaXml()
{
    QFile file(DataFiles::path("datiEventoAppuntamento.xml"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QJsonArray elementi;
    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        if (xml.readNext() == QXmlStreamReader::StartElement && xml.name() == "item") {
            QJsonObject obj;
            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "item")) {
                if (xml.readNext() == QXmlStreamReader::StartElement) {
                    obj.insert(xml.name().toString(), xml.readElementText());
                }
            }
            elementi.append(obj);
        }
    }

    file.close();

    QJsonArray aggiornato;
    bool trovato = false;
    for (const QJsonValue &value : elementi) {
        QJsonObject obj = value.toObject();
        if (!trovato && elementiUguali(obj, elementoCorrente)) {
            trovato = true;
            continue;
        }
        aggiornato.append(obj);
    }

    if (!trovato || !file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("dati");

    const QStringList ordineCampi = {
        "data", "descrizione", "durata_ore", "durata", "stato", "luogo", "ora", "priorita", "tipo", "titolo"
    };

    for (const QJsonValue &value : aggiornato) {
        QJsonObject obj = value.toObject();
        writer.writeStartElement("item");

        for (const QString &campo : ordineCampi) {
            if (obj.contains(campo)) writer.writeTextElement(campo, obj.value(campo).toVariant().toString());
        }

        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if (!ordineCampi.contains(it.key())) writer.writeTextElement(it.key(), it.value().toVariant().toString());
        }

        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
    return true;
}

bool visualize::elementiUguali(const QJsonObject &a, const QJsonObject &b) const
{
    bool stessaChiave = a.value("tipo").toString().compare(b.value("tipo").toString(), Qt::CaseInsensitive) == 0 &&
                        a.value("titolo").toString() == b.value("titolo").toString() &&
                        a.value("data").toString() == b.value("data").toString() &&
                        a.value("ora").toString() == b.value("ora").toString();

    if (!stessaChiave) return false;

    if (a.contains("descrizione") && b.contains("descrizione")) {
        return a.value("descrizione").toString() == b.value("descrizione").toString();
    }

    return true;
}
