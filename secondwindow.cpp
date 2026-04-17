#include "secondwindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>

SecondWindow::SecondWindow(QWidget *parent)
    : QMainWindow{parent}
{
    Titolo = new QLabel("NB_Calendario", this);
    Messaggio = new QLabel("Vuoi valutare la nostra app? Clicca qui sotto", this);
    btnValutazione = new QPushButton("valuta ora", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(Titolo);
    layout->addWidget(Messaggio);
    layout->addWidget(btnValutazione);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);
    setCentralWidget(central);

    connect(btnValutazione, &QPushButton::clicked, this, &SecondWindow::valutazione);
}

void SecondWindow::valutazione(){
    MainWindow *pagValutazione = new MainWindow();
    pagValutazione->show();
    this->close();
}

//SecondWindow::~SecondWindow() = default;