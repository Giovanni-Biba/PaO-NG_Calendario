#include "calendar.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFrame>
#include <QDate>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamReader>
#include <QHBoxLayout>

calendar::calendar(QWidget *parent)
    : QWidget{parent}
{
    oggi = QDate::currentDate();
    lunediSettimana = oggi.addDays(-(oggi.dayOfWeek() - 1));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *top = new QWidget(this);
    QVBoxLayout *topLayout = new QVBoxLayout(top);

    Titolo = new QPushButton("NG_CALENDARIO", this);
    topLayout->addWidget(Titolo);
    mainLayout->addWidget(top);

    BarraRicerca = new SearchBar();
    mainLayout->addWidget(BarraRicerca);

    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);

    QWidget *scrollContent = new QWidget(this);
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    QWidget *calendarWidget = new QWidget;
    grid = new QGridLayout(calendarWidget);

    QStringList nomiGiorni = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};

    for (int col = 0; col < 7; col++) {
        QDate giorno = lunediSettimana.addDays(col);

        QLabel *header = new QLabel(
            QString("%1 %2").arg(nomiGiorni[col]).arg(giorno.day())
            );

        header->setAlignment(Qt::AlignCenter);
        header->setMinimumHeight(40);

        if (giorno == oggi)
            header->setStyleSheet("color: blue; font-weight: bold;");

        grid->addWidget(header, 0, col + 1);
    }

    QLabel *festivitaLabel = new QLabel("Festività");
    festivitaLabel->setAlignment(Qt::AlignLeft);
    grid->addWidget(festivitaLabel, 1, 0);

    for (int col = 0; col < 7; col++) {
        QWidget *cell = new QWidget;
        QHBoxLayout *lay = new QHBoxLayout(cell);

        lay->setContentsMargins(2,2,2,2);
        lay->setSpacing(2);
        lay->setAlignment(Qt::AlignLeft);

        cell->setStyleSheet("border:1px solid lightgray; background:white;");
        cell->setMinimumSize(140, 60);

        grid->addWidget(cell, 1, col + 1);

        celle[1][col] = lay;
        conteggioCelle[1][col] = 0;
    }

    for (int ora = 0; ora < 24; ora++) {
        int row = ora + 2;

        QLabel *oraLabel = new QLabel(QString("%1:00").arg(ora, 2, 10, QChar('0')));
        grid->addWidget(oraLabel, row, 0);

        for (int col = 0; col < 7; col++) {
            QWidget *cell = new QWidget;
            QHBoxLayout *lay = new QHBoxLayout(cell);

            lay->setContentsMargins(2,2,2,2);
            lay->setSpacing(2);
            lay->setAlignment(Qt::AlignLeft);

            cell->setStyleSheet("border:1px solid lightgray; background:white;");
            cell->setMinimumSize(140, 60);

            grid->addWidget(cell, row, col + 1);

            celle[row][col] = lay;
            conteggioCelle[row][col] = 0;
        }
    }

    scrollLayout->addWidget(calendarWidget);
    scroll->setWidget(scrollContent);
    mainLayout->addWidget(scroll);

    Crea = new QPushButton("+ Crea", this);
    mainLayout->addWidget(Crea);

    connect(Crea, &QPushButton::clicked, this, &calendar::richiestaCrea);
    connect(BarraRicerca, &SearchBar::cercaClicked, this, &calendar::richiestaCerca);

    caricaJson();
    caricaXml();
}

void calendar::aggiungiFestivita(const QString& titolo, const QDate& data)
{
    int col = lunediSettimana.daysTo(data);
    if (col < 0 || col > 6) return;

    if (conteggioCelle[1][col] >= 5) return;

    QPushButton *btn = new QPushButton(titolo);

    btn->setStyleSheet(
        "text-align: left;"
        "padding-left: 6px;"
        "background: pink;"
        );

    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    celle[1][col]->addWidget(btn);
    conteggioCelle[1][col]++;

    connect(btn, &QPushButton::clicked, this, [this, titolo]() {
        emit richiestaVisualize(titolo);
    });
}

void calendar::aggiungiEvento(const QString& titolo, const QDate& data, int oraInizio, int durataOre)
{
    int col = lunediSettimana.daysTo(data);
    if (col < 0 || col > 6) return;

    int row = oraInizio + 2;

    if (conteggioCelle[row][col] >= 5) return;

    QPushButton *btn = new QPushButton(titolo);

    btn->setStyleSheet(
        "text-align: left;"
        "padding-left: 6px;"
        "background: lightblue;"
        );

    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    celle[row][col]->addWidget(btn);
    conteggioCelle[row][col]++;

    connect(btn, &QPushButton::clicked, this, [this, titolo]() {
        emit richiestaVisualize(titolo);
    });
}

void calendar::caricaJson()
{
    QFile file("datiAttivitaFestivita.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonArray array = QJsonDocument::fromJson(file.readAll()).array();
    file.close();

    for (auto v : array) {
        QJsonObject o = v.toObject();
        QString titolo = o["titolo"].toString();
        QString tipo = o["tipo"].toString();
        QDate data = QDate::fromString(o["data"].toString(), Qt::ISODate);
        QTime ora = QTime::fromString(o["ora"].toString(), "HH:mm:ss");

        if (tipo == "festivita")
            aggiungiFestivita(titolo, data);

        else if (tipo == "attivita") {
            QTime fine = QTime::fromString(o["oraFine"].toString(), "HH:mm:ss");
            int durata = ora.secsTo(fine)/3600;
            if (durata <= 0) durata = 1;
            aggiungiEvento(titolo, data, ora.hour(), durata);
        }
    }
}

void calendar::caricaXml()
{
    QFile file("datiEventoAppuntamento.xml");
    if (!file.open(QIODevice::ReadOnly)) return;

    QXmlStreamReader r(&file);

    while (!r.atEnd()) {
        r.readNext();
        if (r.isStartElement() && r.name()=="item") {

            QString titolo;
            QDate data;
            QTime ora;
            int durata = 1;

            while (!(r.isEndElement() && r.name()=="item")) {
                r.readNext();

                if (r.isStartElement()) {
                    QString n = r.name().toString();
                    QString v = r.readElementText();

                    if (n=="titolo") titolo=v;
                    else if (n=="data") data=QDate::fromString(v,Qt::ISODate);
                    else if (n=="ora") ora=QTime::fromString(v,"HH:mm:ss");
                    else if (n=="durata") durata=v.toInt();
                }
            }

            aggiungiEvento(titolo,data,ora.hour(),durata);
        }
    }

    file.close();
}

void calendar::closeCalendar()
{
    close();
}