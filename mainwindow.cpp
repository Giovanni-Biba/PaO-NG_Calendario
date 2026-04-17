#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    btnValuta = new QPushButton("Valuta app", this);
    radioSi = new QRadioButton("Sì", this);
    radioNo = new QRadioButton("No", this);
    btnInvia = new QPushButton("Invia valutazione", this);
    labelRisultato = new QLabel("", this);
    btnClose = new QPushButton("chiudi", this);

    //prova
    radioSi->setEnabled(false);
    radioNo->setEnabled(false);
    btnInvia->setEnabled(false);
    btnClose->setEnabled(false);

    radioSi->setVisible(false);
    radioNo->setVisible(false);
    btnInvia->setVisible(false);
    btnClose->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(btnValuta);
    layout->addWidget(radioSi);
    layout->addWidget(radioNo);
    layout->addWidget(btnInvia);
    layout->addWidget(labelRisultato);
    layout->addWidget(btnClose);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);
    setCentralWidget(central);

    connect(btnValuta, &QPushButton::clicked, this, &MainWindow::abilitaValutazione);

    connect(radioSi, &QRadioButton::clicked, this, &MainWindow::sbloccaBtnInvia);

    connect(radioNo, &QRadioButton::clicked, this, &MainWindow::sbloccaBtnInvia);

    connect(btnInvia, &QPushButton::clicked, this, &MainWindow::inviaValutazione);

    connect(btnClose, &QPushButton::clicked, this, &MainWindow::chiudiValutazione);
}

void MainWindow::abilitaValutazione()
{
    radioSi->setEnabled(true);
    radioSi->setVisible(true);
    radioNo->setEnabled(true);
    radioNo->setVisible(true);
    btnInvia->setVisible(true);
}

void MainWindow::sbloccaBtnInvia()
{
    if (radioSi->isChecked() || radioNo->isChecked()) {
        btnInvia->setEnabled(true);
    } else {
        labelRisultato->setText("Seleziona un'opzione.");
    }
}

void MainWindow::inviaValutazione()
{
    if (radioSi->isChecked()) {
        radioSi->setEnabled(false);
        radioNo->setEnabled(false);
        btnValuta->setEnabled(false);
        btnInvia->setEnabled(false);
        labelRisultato->setText("siamo felici che l'app ti sia piaciuta");
        btnClose->setVisible(true);
        btnClose->setEnabled(true);

    } else {
        radioSi->setEnabled(false);
        radioNo->setEnabled(false);
        btnValuta->setEnabled(false);
        btnInvia->setEnabled(false);
        labelRisultato->setText(
            "ci dispiace che tu sia rimasto insoddisfatto, cercheremo di migliorare");
        btnClose->setVisible(true);
        btnClose->setEnabled(true);
    }
}

void MainWindow::chiudiValutazione()
{
    btnValuta->setEnabled(true);
    radioSi->setVisible(false);
    radioNo->setVisible(false);
    btnInvia->setVisible(false);
    btnClose->setVisible(false);
    labelRisultato->clear();
}

MainWindow::~MainWindow() = default;


//prova test prova