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
        stackHome->setCurrentWidget(researchPage);
    });
}