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

        SearchBar* bar = calendarPage->getSearchBar();
        QString tit = bar->getTestoTitolo();
        QString tip = bar->getTestoTipo();
        QDate dat = bar->getValoreData();
        QString pri = bar->getTestoPriorita();
        researchPage->eseguiRicercaFiltrata(tit, dat, tip, pri);
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