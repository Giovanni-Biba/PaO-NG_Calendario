#include "modify.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

modify::modify(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // BARRA SUPERIORE
    QHBoxLayout *topBar = new QHBoxLayout();
    QPushButton *indietro = new QPushButton("← Torna al dettaglio", this);
    indietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    topBar->addWidget(indietro);
    topBar->addStretch();
    mainLayout->addLayout(topBar);

    titoloPagina = new QLabel("MODIFICA IMPEGNO", this);
    titoloPagina->setAlignment(Qt::AlignCenter);
    titoloPagina->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50;");
    mainLayout->addWidget(titoloPagina);

    // AREA SCORREVOLE
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none; background: transparent;");

    QWidget *formContainer = new QWidget();
    QFormLayout *formLayout = new QFormLayout(formContainer);

    // Inizializzazione Widget
    editTitolo = new QLineEdit(this);
    editData = new QDateEdit(this);
    editData->setCalendarPopup(true);
    editOra = new QTimeEdit(this);
    spinDurata = new QSpinBox(this);
    spinDurata->setRange(1, 24);
    spinDurata->setSuffix(" ore");
    comboPriorita = new QComboBox(this);
    comboPriorita->addItems({"Bassa", "Media", "Alta"});
    editDescrizione = new QTextEdit(this);
    editDescrizione->setMaximumHeight(100);

    editLuogo = new QLineEdit(this);
    labelLuogo = new QLabel("Luogo:", this);
    editStato = new QLineEdit(this);
    labelStato = new QLabel("Stato:", this);

    // Stile
    QString style = "padding: 8px; border: 1px solid #ccc; border-radius: 4px;";
    editTitolo->setStyleSheet(style); editData->setStyleSheet(style);
    editOra->setStyleSheet(style); spinDurata->setStyleSheet(style);
    comboPriorita->setStyleSheet(style); editDescrizione->setStyleSheet(style);
    editLuogo->setStyleSheet(style); editStato->setStyleSheet(style);

    formLayout->addRow("Titolo:", editTitolo);
    formLayout->addRow("Data:", editData);
    formLayout->addRow("Ora:", editOra);
    formLayout->addRow("Durata:", spinDurata);
    formLayout->addRow("Priorità:", comboPriorita);
    formLayout->addRow(labelLuogo, editLuogo);
    formLayout->addRow(labelStato, editStato);
    formLayout->addRow("Descrizione:", editDescrizione);

    scrollArea->setWidget(formContainer);
    mainLayout->addWidget(scrollArea);

    QPushButton *btnSalva = new QPushButton("Salva Modifiche", this);
    btnSalva->setStyleSheet("background-color: #27ae60; color: white; border-radius: 5px; padding: 12px; font-weight: bold;");
    mainLayout->addWidget(btnSalva);

    // CONNESSIONI
    connect(indietro, &QPushButton::clicked, this, &modify::tornaIndietro);

    connect(btnSalva, &QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Conferma", "Vuoi salvare le modifiche?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) return;

        QJsonObject nuovo = elementoCorrente;
        nuovo["titolo"] = editTitolo->text();
        nuovo["data"] = editData->date().toString("yyyy-MM-dd");
        nuovo["ora"] = editOra->time().toString("HH:mm");
        nuovo["durata_ore"] = QString::number(spinDurata->value());
        nuovo["priorita"] = comboPriorita->currentText();
        nuovo["descrizione"] = editDescrizione->toPlainText();
        if(editLuogo->isVisible()) nuovo["luogo"] = editLuogo->text();
        if(editStato->isVisible()) nuovo["stato"] = editStato->text();

        QString tipo = elementoCorrente.value("tipo").toString().toLower();
        bool ok = (tipo == "attivita" || tipo == "festivita") ? salvaSuJson(nuovo) : salvaSuXml(nuovo);

        if (ok) {
            QMessageBox::information(this, "Ok", "Modifica completata!");
            emit salvataggioCompletato(nuovo);
        } else {
            QMessageBox::critical(this, "Errore", "Errore nel salvataggio su file.");
        }
    });
}

void modify::caricaElemento(const QJsonObject &elemento) {
    elementoCorrente = elemento;
    labelLuogo->hide(); editLuogo->hide();
    labelStato->hide(); editStato->hide();

    editTitolo->setText(elemento.value("titolo").toString());
    editData->setDate(QDate::fromString(elemento.value("data").toString(), "yyyy-MM-dd"));
    editOra->setTime(QTime::fromString(elemento.value("ora").toString(), "HH:mm"));
    spinDurata->setValue(elemento.value("durata_ore").toVariant().toInt());

    int pIdx = comboPriorita->findText(elemento.value("priorita").toString());
    if(pIdx != -1) comboPriorita->setCurrentIndex(pIdx);
    editDescrizione->setPlainText(elemento.value("descrizione").toString());

    if(elemento.contains("luogo")) { labelLuogo->show(); editLuogo->show(); editLuogo->setText(elemento.value("luogo").toString()); }
    if(elemento.contains("stato")) { labelStato->show(); editStato->show(); editStato->setText(elemento.value("stato").toString()); }
}

bool modify::elementiUguali(const QJsonObject &a, const QJsonObject &b) const {
    return a.value("titolo").toString() == b.value("titolo").toString() &&
           a.value("data").toString() == b.value("data").toString() &&
           a.value("ora").toString() == b.value("ora").toString();
}

bool modify::salvaSuJson(const QJsonObject &nuovo) {
    QFile file("datiAttivitaFestivita.json");
    if (!file.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.isObject() ? doc.object().value("agenda").toArray() : doc.array();
    QJsonArray nuovoArray;
    for (const QJsonValue &v : array) {
        QJsonObject obj = v.toObject();
        nuovoArray.append(elementiUguali(obj, elementoCorrente) ? nuovo : obj);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    if (doc.isObject()) {
        QJsonObject root = doc.object(); root["agenda"] = nuovoArray;
        file.write(QJsonDocument(root).toJson());
    } else file.write(QJsonDocument(nuovoArray).toJson());
    file.close();
    return true;
}

bool modify::salvaSuXml(const QJsonObject &nuovo) {
    QFile file("datiEventoAppuntamento.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QJsonArray elementi;
    QXmlStreamReader reader(&file);
    while (!reader.atEnd()) {
        if (reader.readNext() == QXmlStreamReader::StartElement && reader.name() == "item") {
            QJsonObject obj;
            while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "item")) {
                if (reader.readNext() == QXmlStreamReader::StartElement) obj.insert(reader.name().toString(), reader.readElementText());
            }
            elementi.append(obj);
        }
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true); writer.writeStartDocument();
    writer.writeStartElement("dati");
    for (const QJsonValue &v : elementi) {
        QJsonObject obj = v.toObject();
        writer.writeStartElement("item");
        QJsonObject daScrivere = elementiUguali(obj, elementoCorrente) ? nuovo : obj;
        for (auto it = daScrivere.begin(); it != daScrivere.end(); ++it)
            writer.writeTextElement(it.key(), it.value().toVariant().toString());
        writer.writeEndElement();
    }
    writer.writeEndElement(); writer.writeEndDocument();
    file.close();

    return true;
}