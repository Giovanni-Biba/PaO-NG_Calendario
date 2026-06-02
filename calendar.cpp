#include "calendar.h"

#include "archivioimpegni.h"

#include <QDate>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QScrollArea>
#include <QStringList>
#include <QVBoxLayout>

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
        lay->setContentsMargins(2, 2, 2, 2);
        lay->setSpacing(2);
        lay->setAlignment(Qt::AlignLeft);
        cell->setStyleSheet("border:1px solid lightgray; background:transparent;border-radius: 6px;");
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
            lay->setContentsMargins(2, 2, 2, 2);
            lay->setSpacing(2);
            lay->setAlignment(Qt::AlignLeft);
            cell->setStyleSheet("border:1px solid lightgray; background:transparent;border-radius: 6px;");
            cell->setMinimumSize(140, 60);
            grid->addWidget(cell, row, col + 1);
            celle[row][col] = lay;
            conteggioCelle[row][col] = 0;
        }
    }

    scrollLayout->addWidget(calendarWidget);
    scroll->setWidget(scrollContent);
    mainLayout->addWidget(scroll);

    Crea = new QPushButton("+ CREA", this);
    Crea->setStyleSheet("font-weight: bold; padding: 5px 15px; background-color: #3498db; color: white; border-radius: 4px;");
    mainLayout->addWidget(Crea);

    connect(Titolo, &QPushButton::clicked, this, &calendar::refreshGenerale);
    connect(Crea, &QPushButton::clicked, this, &calendar::richiestaCrea);
    connect(BarraRicerca, &SearchBar::cercaClicked, this, &calendar::richiestaCerca);
    connect(settimanaPrecedente, &QPushButton::clicked, this, &calendar::vaiSettimanaPrecedente);
    connect(settimanaSuccessiva, &QPushButton::clicked, this, &calendar::vaiSettimanaSuccessiva);

    aggiornaIntestazioneSettimana();
    caricaImpegni();
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
            if (!celle[row][col])
                continue;

            while (QLayoutItem *item = celle[row][col]->takeAt(0)) {
                if (item->widget())
                    item->widget()->deleteLater();
                delete item;
            }
            conteggioCelle[row][col] = 0;
        }
    }

    indiceColoreElemento = 0;
    caricaImpegni();
}

QString calendar::prossimoColoreElemento()
{
    QStringList colori = {"#D0F0C0", "#FFDB58", "#FFD1DC", "#C8A2C8", "#5E86C1"};
    QString colore = colori[indiceColoreElemento % colori.size()];
    indiceColoreElemento++;
    return colore;
}

void calendar::aggiungiImpegno(const std::shared_ptr<Agenda> &impegno)
{
    if (!impegno)
        return;

    // Gestione festività
    if (impegno->usaRigaFestivita()) {
        int col = lunediSettimana.daysTo(impegno->getData());

        if (col < 0 || col > 6)
            return;

        int row = 1;

        if (conteggioCelle[row][col] >= 5)
            return;

        QPushButton *btn = new QPushButton(impegno->getTitolo());

        btn->setStyleSheet(
            "text-align: left;"
            "padding-left: 6px;"
            "background:" + impegno->coloreCalendario().name() + ";"
            );

        btn->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Preferred
            );

        celle[row][col]->addWidget(btn);
        conteggioCelle[row][col]++;

        connect(btn, &QPushButton::clicked, this,
                [this, impegno]() {
                    emit richiestaVisualize(impegno);
                });

        return;
    }

    // Gestione impegni normali
    QDateTime inizio(impegno->getData(), impegno->getOra());

    int durata = impegno->getDurataOre(); // durata in ore

    for (int i = 0; i < durata; ++i) {

        QDateTime corrente = inizio.addSecs(i * 3600);

        int col = lunediSettimana.daysTo(corrente.date());

        // Ignora le ore fuori dalla settimana visualizzata
        if (col < 0 || col > 6)
            continue;

        int row = corrente.time().hour() + 2;

        if (row < 2 || row >= 26)
            continue;

        if (conteggioCelle[row][col] >= 5)
            continue;

        QPushButton *btn = new QPushButton(impegno->getTitolo());

        btn->setStyleSheet(
            "text-align: left;"
            "padding-left: 6px;"
            "background:" + impegno->coloreCalendario().name() + ";"
            );

        btn->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Preferred
            );

        celle[row][col]->addWidget(btn);
        conteggioCelle[row][col]++;

        connect(btn, &QPushButton::clicked, this,
                [this, impegno]() {
                    emit richiestaVisualize(impegno);
                });
    }
}

void calendar::caricaImpegni()
{
    for (const auto &impegno : ArchivioImpegni::instance().tutti())
        aggiungiImpegno(impegno);
}

void calendar::closeCalendar()
{
    close();
}
