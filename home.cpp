#include "home.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QApplication>

Home::Home(QWidget *parent)
    : QMainWindow{parent}
{
    Titolo = new QPushButton("NG_CALENDARIO", this);

    QVBoxLayout *topLayout = new QVBoxLayout;
    Titolo->setStyleSheet(
        "QPushButton {"
        "background-color: transparent;"
        "border: none;"
        "}");
    topLayout->addWidget(Titolo);

    QWidget *top = new QWidget(this);
    top->setLayout(topLayout);
    setCentralWidget(top);

    QWidget *calendarWidget = new QWidget;
    QGridLayout *grid = new QGridLayout(calendarWidget);

    // intestazioni giorni
    QStringList giorni = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};

    for (int col = 0; col < 7; col++) {
        QLabel *header = new QLabel(giorni[col]);
        header->setAlignment(Qt::AlignCenter);
        grid->addWidget(header, 0, col + 1);
    }

    // colonna orari + celle
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

    // --- SCROLL AREA
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setWidget(top);
    scroll->setWidget(calendarWidget);
    setCentralWidget(scroll);
}