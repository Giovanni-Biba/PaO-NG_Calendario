#include "home.h"
#include "calendar.h"
#include "research.h"
#include "create.h"
#include "visualize.h"
#include "modify.h"
#include "archivioimpegni.h"
#include "searchbar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTime>
#include <QDate>
#include <QString>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

Home::Home(QWidget *parent)
    : QMainWindow{parent}
{
    // Inizializzazione dello StackedWidget per gestire le diverse pagine
    stackHome = new QStackedWidget;

    // Creazione delle istanze delle pagine
    calendarPage = new calendar(this);
    createPage = new Create(this);
    researchPage = new Research(this);
    visualizePage = new visualize(this);
    modifyPage = new modify(this);

    // Aggiunta delle pagine allo stack
    stackHome->addWidget(calendarPage);
    stackHome->addWidget(createPage);
    stackHome->addWidget(researchPage);
    stackHome->addWidget(visualizePage);
    stackHome->addWidget(modifyPage);

    // Impostazione della pagina iniziale
    paginaPrimaDiVisualize = calendarPage;
    setCentralWidget(stackHome);
    stackHome->setCurrentWidget(calendarPage);

    // --- CONFIGURAZIONE MENU BAR ---
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *caricaJson = fileMenu->addAction("Carica JSON...");
    QAction *caricaXml = fileMenu->addAction("Carica XML...");
    fileMenu->addSeparator();
    QAction *salvaJson = fileMenu->addAction("Salva JSON...");
    QAction *salvaXml = fileMenu->addAction("Salva XML...");

    // --- LOGICA DI CARICAMENTO E SALVATAGGIO FILE ---
    connect(caricaJson, &QAction::triggered, this, [this]() {
        const QString path = QFileDialog::getOpenFileName(this, "Carica file JSON", QString(), "JSON (*.json)");
        if (path.isEmpty()) return;
        if (!ArchivioImpegni::instance().caricaDaFileJson(path))
            QMessageBox::warning(this, "Errore", "Non sono riuscito a caricare il file JSON.");
        calendarPage->aggiornaCalendario();
        stackHome->setCurrentWidget(calendarPage);
    });

    connect(caricaXml, &QAction::triggered, this, [this]() {
        const QString path = QFileDialog::getOpenFileName(this, "Carica file XML", QString(), "XML (*.xml)");
        if (path.isEmpty()) return;
        if (!ArchivioImpegni::instance().caricaDaFileXml(path))
            QMessageBox::warning(this, "Errore", "Non sono riuscito a caricare il file XML.");
        calendarPage->aggiornaCalendario();
        stackHome->setCurrentWidget(calendarPage);
    });

    connect(salvaJson, &QAction::triggered, this, [this]() {
        const QString path = QFileDialog::getSaveFileName(this, "Salva file JSON", QString(), "JSON (*.json)");
        if (!path.isEmpty() && !ArchivioImpegni::instance().salvaSuFileJson(path))
            QMessageBox::warning(this, "Errore", "Non sono riuscito a salvare il file JSON.");
    });

    connect(salvaXml, &QAction::triggered, this, [this]() {
        const QString path = QFileDialog::getSaveFileName(this, "Salva file XML", QString(), "XML (*.xml)");
        if (!path.isEmpty() && !ArchivioImpegni::instance().salvaSuFileXml(path))
            QMessageBox::warning(this, "Errore", "Non sono riuscito a salvare il file XML.");
    });

    // --- LOGICA DI NAVIGAZIONE E SEGNALI ---
    connect(calendarPage, &calendar::richiestaCrea, this, [this]() {
        stackHome->setCurrentWidget(createPage);
    });

    // LOGICA DI RICERCA
    connect(calendarPage, &calendar::richiestaCerca, this, [this]() {
        SearchBar* bar = calendarPage->getSearchBar();
        if (bar) {
            QString tit = bar->getTestoTitolo();
            QString tip = bar->getTestoTipo();
            QDate dat = bar->getValoreData();
            QString pri = bar->getTestoPriorita();

            researchPage->eseguiRicercaFiltrata(tit, dat, tip, pri);
            stackHome->setCurrentWidget(researchPage);
        }
    });

    connect(calendarPage, &calendar::richiestaVisualize, this, [this](std::shared_ptr<Agenda> elemento) {
        paginaPrimaDiVisualize = calendarPage;
        visualizePage->caricaElemento(elemento);
        stackHome->setCurrentWidget(visualizePage);
    });

    connect(researchPage, &Research::richiestaVisualize, this, [this](std::shared_ptr<Agenda> elemento) {
        paginaPrimaDiVisualize = researchPage;
        visualizePage->caricaElemento(elemento);
        stackHome->setCurrentWidget(visualizePage);
    });

    connect(createPage, &Create::tornaIndietro, this, [this]() {
        calendarPage->aggiornaCalendario();
        stackHome->setCurrentWidget(calendarPage);
    });

    connect(researchPage, &Research::ritornaHome, this, [this]() {
        stackHome->setCurrentWidget(calendarPage);
    });

    connect(visualizePage, &visualize::tornaIndietro, this, [this]() {
        stackHome->setCurrentWidget(paginaPrimaDiVisualize);
    });

    connect(visualizePage, &visualize::richiestaModifica, this, [this](std::shared_ptr<Agenda> elemento) {
        modifyPage->caricaElemento(elemento);
        stackHome->setCurrentWidget(modifyPage);
    });

    connect(visualizePage, &visualize::elementoEliminato, this, [this]() {
        calendarPage->aggiornaCalendario();
        stackHome->setCurrentWidget(calendarPage);
    });

    connect(modifyPage, &modify::tornaIndietro, this, [this]() {
        stackHome->setCurrentWidget(visualizePage);
    });

    connect(modifyPage, &modify::salvataggioCompletato, this, [this](std::shared_ptr<Agenda> elemento) {
        visualizePage->caricaElemento(elemento);
        calendarPage->aggiornaCalendario();
        stackHome->setCurrentWidget(visualizePage);
    });
}