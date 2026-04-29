#include "calendar.h"
#include "searchbar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

calendar::calendar(QWidget *parent)
    : QWidget{parent}
{
    // --- LAYOUT PRINCIPALE
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // --- TOP BAR
    QWidget *top = new QWidget;
    QVBoxLayout *topLayout = new QVBoxLayout(top);

    Titolo = new QPushButton("NG_CALENDARIO");
    /*Titolo->setStyleSheet(
        "QPushButton {"
        "background-color: transparent;"
        "border: none;"
        "}"
        );*/

    topLayout->addWidget(Titolo);
    mainLayout->addWidget(top);


    // --- BARRA DI RICERCA
    BarraRicerca = new SearchBar();
    mainLayout->addWidget(BarraRicerca);




    // --- CONTENITORE SCROLLABILE (IMPORTANTE)
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);

    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    // --- CALENDAR
    QWidget *calendarWidget = new QWidget;
    QGridLayout *grid = new QGridLayout(calendarWidget);

    QStringList giorni = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};

    for (int col = 0; col < 7; col++) {
        QLabel *header = new QLabel(giorni[col]);
        header->setAlignment(Qt::AlignCenter);
        grid->addWidget(header, 0, col + 1);
    }

    for (int row = 1; row <= 24; row++) {
        QLabel *ora = new QLabel(QString("%1:00").arg(row));
        grid->addWidget(ora, row, 0);

        for (int col = 0; col < 7; col++) {
            QLabel *cella = new QLabel;
            cella->setMinimumSize(100, 60);
            cella->setStyleSheet("border: 1px solid lightgray;");
            grid->addWidget(cella, row, col + 1);
        }
    }

    scrollLayout->addWidget(calendarWidget);
    scroll->setWidget(scrollContent);

    // aggiungi scroll al layout principale
    mainLayout->addWidget(scroll);

    // connect(BarraRicerca->CercaButton, &QPushButton::clicked, this, &calendar::closeCalendar);
}

void calendar::closeCalendar(){
    this->close();
}