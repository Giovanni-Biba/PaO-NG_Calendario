#include "home.h"
#include "calendar.h"
#include "research.h"
#include "create.h"
#include "visualize.h"
#include "modify.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTime>
#include <QDate>
#include <QJsonObject>
#include <QString>

Home::Home(QWidget *parent)
    : QMainWindow{parent}
{
    stackHome = new QStackedWidget;

    calendarPage = new calendar(this);
    createPage = new Create(this);
    researchPage = new Research(this);
    visualizePage = new visualize(this);
    modifyPage = new modify(this);
    stackHome->addWidget(calendarPage);
    stackHome->addWidget(createPage);
    stackHome->addWidget(researchPage);
    stackHome->addWidget(visualizePage);
    stackHome->addWidget(modifyPage);
    paginaPrimaDiVisualize = calendarPage;
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

    connect(calendarPage, &calendar::richiestaVisualize, this, [this](const QString &titolo, const QString &data, const QString &ora) {
        paginaPrimaDiVisualize = calendarPage;
        visualizePage->caricaDaChiave(titolo, data, ora);
        stackHome->setCurrentWidget(visualizePage);
    });

    connect(researchPage, &Research::richiestaVisualize, this, [this](const QJsonObject &elemento) {
        paginaPrimaDiVisualize = researchPage;
        visualizePage->caricaElemento(elemento);
        stackHome->setCurrentWidget(visualizePage);
    });

    connect(createPage, &Create::tornaIndietro, this, [this]() {
        if (stackHome->currentWidget() != calendarPage) {
            calendarPage->aggiornaCalendario();
            stackHome->setCurrentWidget(calendarPage);
        }
    });
    connect(researchPage, &Research::ritornaHome, this, [this]() {
        if (stackHome->currentWidget() != calendarPage) {
            stackHome->setCurrentWidget(calendarPage);
        }
    });

    connect(visualizePage, &visualize::tornaIndietro, this, [this]() {
        stackHome->setCurrentWidget(paginaPrimaDiVisualize);
    });

    connect(visualizePage, &visualize::richiestaModifica, this, [this](const QJsonObject &elemento) {
        modifyPage->caricaElemento(elemento);
        stackHome->setCurrentWidget(modifyPage);
    });

    connect(visualizePage, &visualize::elementoEliminato, this, [this]() {
        calendarPage->aggiornaCalendario();
        if (paginaPrimaDiVisualize == researchPage) {
            stackHome->setCurrentWidget(calendarPage);
        } else {
            stackHome->setCurrentWidget(paginaPrimaDiVisualize);
        }
    });

    connect(modifyPage, &modify::tornaIndietro, this, [this]() {
        stackHome->setCurrentWidget(visualizePage);
    });
}
