#include "research.h"
#include "home.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

Research::Research(const RicercaConfig &config, QWidget *parent)
    : QMainWindow(parent),
    rConfig(config)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    closeResearch = new QPushButton("Chiudi ricerca", this);
    mainLayout->addWidget(closeResearch);

    QWidget *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    connect(closeResearch, &QPushButton::clicked, this, &Research::RitornaHome);

}

void Research::RitornaHome(){
    Home *h = new Home();
    h->showMaximized();
    this->close();
}
