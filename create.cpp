#include "create.h"

#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QPushButton>
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

    QHBoxLayout *radioLayout = new QHBoxLayout;

    rbAttivita = new QRadioButton("Attività", this);
    rbEvento = new QRadioButton("Evento", this);
    rbAppuntamento = new QRadioButton("Appuntamento", this);

    radioLayout->addWidget(rbAttivita);
    radioLayout->addWidget(rbEvento);
    radioLayout->addWidget(rbAppuntamento);

    mainLayout->addLayout(radioLayout);

    QLabel *titoloLabel = new QLabel("Titolo", this);
    titoloEdit = new QLineEdit;

    mainLayout->addWidget(titoloLabel);
    mainLayout->addWidget(titoloEdit);

    QHBoxLayout *dateTimeLayout = new QHBoxLayout(this);

    dataEdit = new QDateEdit(QDate(2026,1,1));
    dataEdit->setCalendarPopup(true);

    oraEdit = new QTimeEdit(QTime(0,0), this);

    dateTimeLayout->addWidget(dataEdit);
    dateTimeLayout->addWidget(oraEdit);

    mainLayout->addLayout(dateTimeLayout);

    prioritaLabel = new QLabel("Priorità", this);
    prioritaBox = new QComboBox(this);
    prioritaBox->addItem("seleziona Priorità...");
    prioritaBox->addItems({"Alta", "Media", "Bassa"});

    mainLayout->addWidget(prioritaLabel);
    mainLayout->addWidget(prioritaBox);

    luogoLabel = new QLabel("Luogo", this);
    luogoEdit = new QLineEdit(this);

    mainLayout->addWidget(luogoLabel);
    mainLayout->addWidget(luogoEdit);

    statoLabel = new QLabel("Stato", this);
    statoBox = new QComboBox(this);
    statoBox->addItem("Seleziona Stato...");
    statoBox->addItems({"Italia", "USA", "Cina", "Worldwide"});

    mainLayout->addWidget(statoLabel);
    mainLayout->addWidget(statoBox);

    creaButton = new QPushButton("Crea", this);
    mainLayout->addWidget(creaButton);

    connect(creaButton, &QPushButton::clicked, this, &Create::salvaDati);

    indietroButton = new QPushButton("Indietro", this);
    mainLayout->addWidget(indietroButton);

    connect(indietroButton, &QPushButton::clicked, this, &Create::tornaIndietro);

    aggiornaVisibilita();

    connect(rbAttivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbEvento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbAppuntamento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
}

void Create::aggiornaVisibilita()
{
    prioritaLabel->setVisible(rbAttivita->isChecked());
    prioritaBox->setVisible(rbAttivita->isChecked());

    luogoLabel->setVisible(rbEvento->isChecked());
    luogoEdit->setVisible(rbEvento->isChecked());

    statoLabel->setVisible(rbAppuntamento->isChecked());
    statoBox->setVisible(rbAppuntamento->isChecked());
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

    if (successo) {
        emit tornaIndietro();
    }
}

bool Create::validaCampi()
{
    if (titoloEdit->text().isEmpty())
        return false;

    if (!rbAttivita->isChecked() &&
        !rbEvento->isChecked() &&
        !rbAppuntamento->isChecked())
        return false;

    if (rbEvento->isChecked() && luogoEdit->text().isEmpty())
        return false;

    return true;
}

bool Create::salvaJSON()
{
    qDebug() << "Percorso JSON:" << QDir::currentPath();

    QFile file(QCoreApplication::applicationDirPath() + "/datiAttivitaFestivita.json");
    QJsonArray array;

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        array = QJsonDocument::fromJson(file.readAll()).array();
        file.close();
    }

    QJsonObject obj;
    obj["tipo"] = "attivita";
    obj["titolo"] = titoloEdit->text();
    obj["data"] = dataEdit->date().toString(Qt::ISODate);
    obj["ora"] = oraEdit->time().toString();
    obj["priorita"] = prioritaBox->currentText();

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
    qDebug() << "Percorso XML:" << QDir::currentPath();

    QFile file("datiEventoAppuntamento.xml");
    QList<QVariantMap> elementi;

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);

        while (!reader.atEnd()) {
            reader.readNext();

            if (reader.isStartElement() && reader.name() == "item") {
                QVariantMap m;

                while (!(reader.isEndElement() && reader.name() == "item")) {
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
    nuovo["data"] = dataEdit->date().toString(Qt::ISODate);
    nuovo["ora"] = oraEdit->time().toString();

    if (rbEvento->isChecked()) {
        nuovo["tipo"] = "evento";
        nuovo["luogo"] = luogoEdit->text();
    } else {
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

        for (auto it = m.begin(); it != m.end(); ++it) {
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