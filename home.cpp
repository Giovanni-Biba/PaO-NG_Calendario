#include "home.h"
#include "calendar.h"
#include "research.h"
#include "create.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <Qtime>
#include <QDate>
#include <QString>

Home::Home(QWidget *parent)
    : QMainWindow{parent}
{
    stackHome = new QStackedWidget;

    calendarPage = new calendar(this);
    createPage = new Create(this);
    researchPage = new Research(this);
    stackHome->addWidget(calendarPage);
    stackHome->addWidget(createPage);
    stackHome->addWidget(researchPage);
    setCentralWidget(stackHome);
    stackHome->setCurrentWidget(calendarPage);


    connect(calendarPage, &calendar::richiestaCrea, this, [this]() {
        stackHome->setCurrentWidget(createPage);
    });

    connect(calendarPage, &calendar::richiestaCerca, this, [this]() {
        // 1. Prendiamo il puntatore alla barra (serve il getter getSearchBar in calendar.h)
        SearchBar* bar = calendarPage->getSearchBar();
        // 2. Leggiamo i valori dai campi della barra
        QString tit = bar->getTestoTitolo();
        QDate dat = bar->getValoreData();
        QTime ora = bar->getValoreOra();
        QString pri = bar->getTestoPriorita();
        // 3. Chiamiamo la funzione di ricerca filtrata dentro Research
        researchPage->eseguiRicercaFiltrata(tit, dat, ora, pri);
        // 4. Cambiamo pagina per mostrare i risultati
        stackHome->setCurrentWidget(researchPage);
    });

    connect(createPage, &Create::tornaIndietro, this, [this]() {
        if (stackHome->currentWidget() != calendarPage) {
            stackHome->setCurrentWidget(calendarPage);
        }
    });
    connect(researchPage, &Research::ritornaHome, this, [this]() {
        if (stackHome->currentWidget() != calendarPage) {
            stackHome->setCurrentWidget(calendarPage);
        }
    });
}