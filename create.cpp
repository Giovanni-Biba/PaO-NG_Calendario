#include "create.h"
#include "datafiles.h"

#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>

#include <QDate>
#include <QTime>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <QMessageBox>
#include <QDebug>
#include <QDir>

Create::Create(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("CREA", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    mainLayout->addWidget(title);

    // RADIO BUTTON

    QHBoxLayout *radioLayout = new QHBoxLayout;

    rbAttivita = new QRadioButton("Attività", this);
    rbEvento = new QRadioButton("Evento", this);
    rbAppuntamento = new QRadioButton("Appuntamento", this);

    radioLayout->addWidget(rbAttivita);
    radioLayout->addWidget(rbEvento);
    radioLayout->addWidget(rbAppuntamento);

    mainLayout->addLayout(radioLayout);

    // TITOLO

    QLabel *titoloLabel = new QLabel("Titolo", this);
    titoloEdit = new QLineEdit(this);

    mainLayout->addWidget(titoloLabel);
    mainLayout->addWidget(titoloEdit);

    // DESCRIZIONE

    descrizioneLabel = new QLabel("Descrizione", this);
    descrizioneEdit = new QTextEdit(this);

    mainLayout->addWidget(descrizioneLabel);
    mainLayout->addWidget(descrizioneEdit);

    // DATA E ORA

    QHBoxLayout *dateTimeLayout = new QHBoxLayout;

    dataEdit = new QDateEdit(QDate::currentDate(), this);
    dataEdit->setCalendarPopup(true);

    oraEdit = new QTimeEdit(QTime::currentTime(), this);

    dateTimeLayout->addWidget(dataEdit);
    dateTimeLayout->addWidget(oraEdit);

    mainLayout->addLayout(dateTimeLayout);

    // PRIORITÀ

    prioritaLabel = new QLabel("Priorità", this);

    prioritaBox = new QComboBox(this);
    prioritaBox->addItem("Seleziona Priorità...");
    prioritaBox->addItems({"Alta", "Media", "Bassa"});

    mainLayout->addWidget(prioritaLabel);
    mainLayout->addWidget(prioritaBox);

    // DATA FINE

    dataFineLabel = new QLabel("Data Fine", this);

    dataFineEdit = new QDateEdit(QDate::currentDate(), this);
    dataFineEdit->setCalendarPopup(true);

    mainLayout->addWidget(dataFineLabel);
    mainLayout->addWidget(dataFineEdit);

    // ORA FINE

    oraFineLabel = new QLabel("Ora Fine", this);

    oraFineEdit = new QTimeEdit(QTime::currentTime(), this);

    mainLayout->addWidget(oraFineLabel);
    mainLayout->addWidget(oraFineEdit);

    // DURATA

    durataLabel = new QLabel("Durata (ore)", this);

    durataSpin = new QSpinBox(this);
    durataSpin->setRange(0, 24);
    durataSpin->setValue(0);

    mainLayout->addWidget(durataLabel);
    mainLayout->addWidget(durataSpin);

    // LUOGO

    luogoLabel = new QLabel("Luogo", this);

    luogoEdit = new QLineEdit(this);

    mainLayout->addWidget(luogoLabel);
    mainLayout->addWidget(luogoEdit);

    // STATO

    statoLabel = new QLabel("Stato", this);

    statoBox = new QComboBox(this);
    statoBox->addItem("Seleziona Stato...");
    statoBox->addItems({"Italia", "USA", "Cina", "Worldwide"});

    mainLayout->addWidget(statoLabel);
    mainLayout->addWidget(statoBox);

    // BOTTONE CREA

    creaButton = new QPushButton("Crea", this);
    mainLayout->addWidget(creaButton);

    connect(creaButton, &QPushButton::clicked,
            this, &Create::salvaDati);

    // BOTTONE INDIETRO

    indietroButton = new QPushButton("Indietro", this);
    mainLayout->addWidget(indietroButton);

    connect(indietroButton, &QPushButton::clicked,
            this, &Create::tornaIndietro);

    // VISIBILITÀ

    aggiornaVisibilita();

    connect(rbAttivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);

    connect(rbEvento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);

    connect(rbAppuntamento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
}

void Create::aggiornaVisibilita()
{
    bool attivita = rbAttivita->isChecked();
    bool evento = rbEvento->isChecked();
    bool appuntamento = rbAppuntamento->isChecked();

    // ATTIVITÀ

    dataFineLabel->setVisible(attivita);
    dataFineEdit->setVisible(attivita);

    oraFineLabel->setVisible(attivita);
    oraFineEdit->setVisible(attivita);

    // EVENTO / APPUNTAMENTO

    durataLabel->setVisible(evento || appuntamento);
    durataSpin->setVisible(evento || appuntamento);

    // EVENTO

    luogoLabel->setVisible(evento);
    luogoEdit->setVisible(evento);

    // APPUNTAMENTO

    statoLabel->setVisible(appuntamento);
    statoBox->setVisible(appuntamento);
}

void Create::salvaDati()
{
    if (!validaCampi()) {
        QMessageBox::warning(this, "Errore", "Compila tutti i campi");
        return;
    }

    bool successo = false;

    if (rbAttivita->isChecked())
        successo = salvaJSON();
    else
        successo = salvaXML();

    if (successo)
        emit tornaIndietro();
}

bool Create::validaCampi()
{
    if (titoloEdit->text().isEmpty())
        return false;

    if (descrizioneEdit->toPlainText().isEmpty())
        return false;

    if (!rbAttivita->isChecked() && !rbEvento->isChecked() && !rbAppuntamento->isChecked())
        return false;

    if (rbEvento->isChecked() && luogoEdit->text().isEmpty())
        return false;

    return true;
}

bool Create::salvaJSON()
{
    QFile file(DataFiles::path("datiAttivitaFestivita.json"));

    QJsonArray array;

    if (file.exists() &&
        file.open(QIODevice::ReadOnly))
    {
        array = QJsonDocument::fromJson(file.readAll()).array();

        file.close();
    }

    QJsonObject obj;

    obj["tipo"] = "attivita";
    obj["titolo"] = titoloEdit->text();
    obj["descrizione"] = descrizioneEdit->toPlainText();

    obj["data"] = dataEdit->date().toString(Qt::ISODate);

    obj["ora"] = oraEdit->time().toString();

    obj["priorita"] = prioritaBox->currentText();

    obj["dataFine"] = dataFineEdit->date().toString(Qt::ISODate);

    obj["oraFine"] = oraFineEdit->time().toString();

    array.append(obj);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Errore", "Errore apertura file JSON");

        return false;
    }

    file.write(QJsonDocument(array).toJson());

    file.close();

    QMessageBox::information(this, "OK", "Salvato in JSON");

    return true;
}

bool Create::salvaXML()
{
    QFile file(DataFiles::path("datiEventoAppuntamento.xml"));

    QList<QVariantMap> elementi;

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader reader(&file);

        while (!reader.atEnd()) {

            reader.readNext();

            if (reader.isStartElement() && reader.name() == "item")
            {
                QVariantMap m;

                while (!(reader.isEndElement() && reader.name() == "item"))
                {
                    reader.readNext();

                    if (reader.isStartElement()) {

                        m[reader.name().toString()] = reader.readElementText();
                    }
                }
                elementi.append(m);
            }
        }

        file.close();
    }

    QVariantMap nuovo;

    nuovo["titolo"] = titoloEdit->text();

    nuovo["descrizione"] = descrizioneEdit->toPlainText();

    nuovo["data"] = dataEdit->date().toString(Qt::ISODate);

    nuovo["ora"] = oraEdit->time().toString();

    nuovo["durata"] = QString::number(durataSpin->value());

    nuovo["priorita"] = prioritaBox->currentText();

    if (rbEvento->isChecked()) {

        nuovo["tipo"] = "evento";
        nuovo["luogo"] = luogoEdit->text();
    }
    else {

        nuovo["tipo"] = "appuntamento";
        nuovo["stato"] = statoBox->currentText();
    }

    elementi.append(nuovo);

    if (!file.open(QIODevice::WriteOnly)) {

        QMessageBox::critical(this, "Errore", "Errore apertura file XML");

        return false;
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);

    writer.writeStartDocument();

    writer.writeStartElement("dati");

    for (const auto &m : elementi) {

        writer.writeStartElement("item");

        for (auto it = m.begin(); it != m.end(); ++it)
        {
            writer.writeTextElement(it.key(), it.value().toString());
        }

        writer.writeEndElement();
    }

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    QMessageBox::information(this, "OK", "Salvato in XML");

    return true;
}
