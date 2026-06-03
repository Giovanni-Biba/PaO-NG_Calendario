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
#include <QPushButton>

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
    grid->setSpacing(0);

    // --- CONFIGURAZIONE COLONNE FISSE ---
    grid->setColumnStretch(0, 0);
    for (int col = 1; col <= 7; col++) {
        grid->setColumnStretch(col, 1);
        grid->setColumnMinimumWidth(col, 150);
    }

    // Header giorni
    for (int col = 0; col < 7; col++) {
        QLabel *header = new QLabel(this);
        header->setAlignment(Qt::AlignCenter);
        header->setMinimumHeight(40);
        header->setStyleSheet("font-weight: bold; border-bottom: 2px solid #3498db;");
        grid->addWidget(header, 0, col + 1);
        headerGiorni[col] = header;
    }

    QLabel *festivitaLabel = new QLabel("Festività");
    festivitaLabel->setAlignment(Qt::AlignCenter);
    grid->addWidget(festivitaLabel, 1, 0);

    // Riga Festività
    for (int col = 0; col < 7; col++) {
        QWidget *cell = new QWidget;
        QHBoxLayout *lay = new QHBoxLayout(cell);
        lay->setContentsMargins(1, 1, 1, 1);
        lay->setSpacing(1);
        cell->setStyleSheet("border: 1px solid lightgray; background: transparent;");
        cell->setMinimumHeight(50);
        grid->addWidget(cell, 1, col + 1);
        celle[1][col] = lay;
    }

    // Righe Ore
    for (int ora = 0; ora < 24; ora++) {
        int row = ora + 2;
        QLabel *oraLabel = new QLabel(QString("%1:00").arg(ora, 2, 10, QChar('0')));
        oraLabel->setAlignment(Qt::AlignCenter);
        grid->addWidget(oraLabel, row, 0);

        for (int col = 0; col < 7; col++) {
            QWidget *cell = new QWidget;
            QHBoxLayout *lay = new QHBoxLayout(cell);
            lay->setContentsMargins(1, 1, 1, 1);
            lay->setSpacing(1);
            cell->setStyleSheet("border: 1px solid #eee; background: transparent;");
            cell->setMinimumHeight(60);
            grid->addWidget(cell, row, col + 1);
            celle[row][col] = lay;
        }
    }

    scrollLayout->addWidget(calendarWidget);
    scroll->setWidget(scrollContent);
    mainLayout->addWidget(scroll);

    Crea = new QPushButton("+ CREA", this);
    Crea->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; border-radius: 5px; padding: 12px;} QPushButton:hover { background-color: #2ecc71; }");
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
    QStringList nomiGiorni = {"Lunedì", "Martedì", "Mercoledì", "Giovedì", "Venerdì", "Sabato", "Domenica"};
    QDate domenicaSettimana = lunediSettimana.addDays(6);

    labelSettimana->setText(QString("<b>%1</b> - <b>%2</b>")
                                .arg(lunediSettimana.toString("dd MMM yyyy"))
                                .arg(domenicaSettimana.toString("dd MMM yyyy")));

    for (int col = 0; col < 7; col++) {
        QDate giorno = lunediSettimana.addDays(col);
        headerGiorni[col]->setText(QString("%1\n%2").arg(nomiGiorni[col]).arg(giorno.toString("dd/MM")));

        if (giorno == oggi)
            headerGiorni[col]->setStyleSheet("color: #e74c3c; font-weight: bold; border-bottom: 2px solid #e74c3c;");
        else
            headerGiorni[col]->setStyleSheet("font-weight: bold; border-bottom: 2px solid #3498db;");
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
    caricaImpegni();
}

void calendar::aggiungiImpegno(const std::shared_ptr<Agenda> &impegno)
{
    if (!impegno) return;

    int col = lunediSettimana.daysTo(impegno->getData());
    if (col < 0 || col > 6) return;

    auto setupButton = [this, impegno](QPushButton* btn) {
        btn->setStyleSheet(QString(
                               "QPushButton {"
                               "  text-align: center;"
                               "  padding: 2px;"
                               "  font-size: 10px;"
                               "  font-weight: bold;"
                               "  border-radius: 2px;"
                               "  background-color: %1;"
                               "  color: #2c3e50;"
                               "  border: 1px solid rgba(0,0,0,0.1);"
                               "}"
                               ).arg(impegno->coloreCalendario().name()));

        // I bottoni si espandono per riempire la larghezza in parti uguali
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setMinimumWidth(30);

        connect(btn, &QPushButton::clicked, this, [this, impegno]() {
            emit richiestaVisualize(impegno);
        });
    };

    if (impegno->usaRigaFestivita()) {
        QPushButton *btn = new QPushButton(impegno->getTitolo());
        setupButton(btn);
        celle[1][col]->addWidget(btn);
    } else {
        QDateTime inizio(impegno->getData(), impegno->getOra());
        int durata = impegno->getDurataOre();

        for (int i = 0; i < durata; ++i) {
            QDateTime corrente = inizio.addSecs(i * 3600);
            int dCol = lunediSettimana.daysTo(corrente.date());
            if (dCol < 0 || dCol > 6) continue;

            int row = corrente.time().hour() + 2;
            if (row < 2 || row >= 26) continue;

            QPushButton *btn = new QPushButton(impegno->getTitolo());
            setupButton(btn);
            celle[row][dCol]->addWidget(btn);
        }
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