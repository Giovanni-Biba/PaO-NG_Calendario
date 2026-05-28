#include "calendar.h"
#include "datafiles.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFrame>
#include <QDate>
#include <QTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamReader>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QStringList>

calendar::calendar(QWidget *parent)
    : QWidget{parent}
{
    oggi = QDate::currentDate();
    lunediSettimana = oggi.addDays(-(oggi.dayOfWeek() - 1));
    indiceColoreElemento = 0;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *top = new QWidget(this);
    QVBoxLayout *topLayout = new QVBoxLayout(top);

    Titolo = new QPushButton("NG_CALENDARIO", this);
    topLayout->addWidget(Titolo);
    mainLayout->addWidget(top);

    BarraRicerca = new SearchBar();
    mainLayout->addWidget(BarraRicerca);

    QWidget *navigazioneSettimana = new QWidget(this);
    QHBoxLayout *navLayout = new QHBoxLayout(navigazioneSettimana);

    settimanaPrecedente = new QPushButton("<", this);
    settimanaSuccessiva = new QPushButton(">", this);
    labelSettimana = new QLabel(this);

    labelSettimana->setAlignment(Qt::AlignCenter);
    labelSettimana->setMinimumHeight(32);

    navLayout->addWidget(settimanaPrecedente);
    navLayout->addWidget(labelSettimana, 1);
    navLayout->addWidget(settimanaSuccessiva);
    mainLayout->addWidget(navigazioneSettimana);

    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);

    QWidget *scrollContent = new QWidget(this);
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    QWidget *calendarWidget = new QWidget;
    grid = new QGridLayout(calendarWidget);

    for (int col = 0; col < 7; col++) {
        QLabel *header = new QLabel(this);
        header->setAlignment(Qt::AlignCenter);
        header->setMinimumHeight(40);

        grid->addWidget(header, 0, col + 1);
        headerGiorni[col] = header;
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

    connect(Titolo, &QPushButton::clicked, this, &calendar::refreshGenerale);
    connect(Crea, &QPushButton::clicked, this, &calendar::richiestaCrea);
    connect(BarraRicerca, &SearchBar::cercaClicked, this, &calendar::richiestaCerca);
    connect(settimanaPrecedente, &QPushButton::clicked, this, &calendar::vaiSettimanaPrecedente);
    connect(settimanaSuccessiva, &QPushButton::clicked, this, &calendar::vaiSettimanaSuccessiva);

    aggiornaIntestazioneSettimana();
    caricaJson();
    caricaXml();
}

void calendar::aggiornaIntestazioneSettimana()
{
    QStringList nomiGiorni = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};
    QDate domenicaSettimana = lunediSettimana.addDays(6);

    labelSettimana->setText(
        QString("Settimana %1 - %2")
            .arg(lunediSettimana.toString("dd/MM/yyyy"))
            .arg(domenicaSettimana.toString("dd/MM/yyyy"))
        );

    for (int col = 0; col < 7; col++) {
        QDate giorno = lunediSettimana.addDays(col);

        headerGiorni[col]->setText(
            QString("%1 %2/%3")
                .arg(nomiGiorni[col])
                .arg(giorno.day(), 2, 10, QChar('0'))
                .arg(giorno.month(), 2, 10, QChar('0'))
            );

        if (giorno == oggi)
            headerGiorni[col]->setStyleSheet("color: blue; font-weight: bold;");
        else
            headerGiorni[col]->setStyleSheet("");
    }
}

void calendar::vaiSettimanaPrecedente()
{
    lunediSettimana = lunediSettimana.addDays(-7);
    aggiornaIntestazioneSettimana();
    aggiornaCalendario();
}

void calendar::vaiSettimanaSuccessiva()
{
    lunediSettimana = lunediSettimana.addDays(7);
    aggiornaIntestazioneSettimana();
    aggiornaCalendario();
}

void calendar::refreshGenerale()
{
    oggi = QDate::currentDate();
    lunediSettimana = oggi.addDays(-(oggi.dayOfWeek() - 1));

    aggiornaIntestazioneSettimana();
    aggiornaCalendario();
}

void calendar::aggiornaCalendario()
{
    for (int row = 1; row < 26; ++row) {
        for (int col = 0; col < 7; ++col) {
            if (!celle[row][col]) continue;

            while (QLayoutItem *item = celle[row][col]->takeAt(0)) {
                if (item->widget()) item->widget()->deleteLater();
                delete item;
            }

            conteggioCelle[row][col] = 0;
        }
    }

    indiceColoreElemento = 0;
    caricaJson();
    caricaXml();
}

QString calendar::prossimoColoreElemento()
{
    QStringList colori = {"#D0F0C0", "#FFDB58", "#FFD1DC", "#C8A2C8", "#5E86C1"};
    QString colore = colori[indiceColoreElemento % colori.size()];

    indiceColoreElemento++;

    return colore;
}

void calendar::aggiungiFestivita(const QString& titolo, const QDate& data, const QString& ora)
{
    int col = lunediSettimana.daysTo(data);
    if (col < 0 || col > 6) return;

    if (conteggioCelle[1][col] >= 5) return;

    QString colore = prossimoColoreElemento();
    QPushButton *btn = new QPushButton(titolo);

    btn->setStyleSheet(
        "text-align: left;"
        "padding-left: 6px;"
        "background:" + colore + ";"
        );

    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    celle[1][col]->addWidget(btn);
    conteggioCelle[1][col]++;

    connect(btn, &QPushButton::clicked, this, [this, titolo, data, ora]() {
        emit richiestaVisualize(titolo, data.toString(Qt::ISODate), ora);
    });
}

void calendar::aggiungiEvento(const QString& titolo, const QDate& data, const QString& ora, int durataOre)
{
    QTime oraParsed = QTime::fromString(ora, "HH:mm");
    if (!oraParsed.isValid()) oraParsed = QTime::fromString(ora, "HH:mm:ss");
    if (!oraParsed.isValid()) return;

    int oraIniziale = oraParsed.hour();
    if (durataOre <= 0) durataOre = 1;

    QString colore;

    for (int i = 0; i < durataOre; i++) {
        int oreTotali = oraIniziale + i;
        QDate giornoCorrente = data.addDays(oreTotali / 24);
        int col = lunediSettimana.daysTo(giornoCorrente);

        if (col < 0 || col > 6) continue;

        int row = (oreTotali % 24) + 2;

        if (conteggioCelle[row][col] >= 5) continue;

        if (colore.isEmpty())
            colore = prossimoColoreElemento();

        QPushButton *btn = new QPushButton(titolo);

        btn->setStyleSheet(
            "text-align: left;"
            "padding-left: 6px;"
            "background:" + colore + ";"
            );

        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        celle[row][col]->addWidget(btn);
        conteggioCelle[row][col]++;

        connect(btn, &QPushButton::clicked, this, [this, titolo, data, ora]() {
            emit richiestaVisualize(titolo, data.toString(Qt::ISODate), ora);
        });
    }
}

void calendar::caricaJson()
{
    QFile file(DataFiles::path("datiAttivitaFestivita.json"));
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray array = doc.isArray() ? doc.array() : doc.object().value("agenda").toArray();
    file.close();

    for (auto v : array) {
        QJsonObject o = v.toObject();
        QString titolo = o["titolo"].toString();
        QString tipo = o["tipo"].toString().toLower();
        QDate data = QDate::fromString(o["data"].toString(), Qt::ISODate);
        QString ora = o["ora"].toString();

        if (tipo == "festivita")
            aggiungiFestivita(titolo, data, ora);

        else if (tipo == "attivita") {
            int durata = o["durata_ore"].toInt(0);
            if (durata <= 0) {
                QTime inizio = QTime::fromString(ora, "HH:mm");
                QTime fine = QTime::fromString(o["oraFine"].toString(), "HH:mm:ss");
                durata = inizio.secsTo(fine) / 3600;
            }
            if (durata <= 0) durata = 1;
            aggiungiEvento(titolo, data, ora, durata);
        }
    }
}

void calendar::caricaXml()
{
    QFile file(DataFiles::path("datiEventoAppuntamento.xml"));
    if (!file.open(QIODevice::ReadOnly)) return;

    QXmlStreamReader r(&file);

    while (!r.atEnd()) {
        r.readNext();
        if (r.isStartElement() && r.name()=="item") {

            QString titolo;
            QDate data;
            QString ora;
            int durata = 1;

            while (!(r.isEndElement() && r.name()=="item")) {
                r.readNext();

                if (r.isStartElement()) {
                    QString n = r.name().toString();
                    QString v = r.readElementText();

                    if (n=="titolo") titolo=v;
                    else if (n=="data") data=QDate::fromString(v,Qt::ISODate);
                    else if (n=="ora") ora=v;
                    else if (n=="durata" || n=="durata_ore") durata=v.toInt();
                }
            }

            aggiungiEvento(titolo,data,ora,durata);
        }
    }

    file.close();
}

void calendar::closeCalendar()
{
    close();
}
