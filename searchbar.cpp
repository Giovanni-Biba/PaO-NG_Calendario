#include "searchbar.h"
#include <QHBoxLayout>
#include <QLabel>

SearchBar::SearchBar(QWidget *parent)
    : QWidget{parent}
{
    //this->setStyleSheet("background-color: #f0f0f0; border: 1px solid lightgrey;");

    // Layout orizzontale principale per tutta la barra
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(30);

    // --- TITOLO
    QHBoxLayout *layoutTitolo = new QHBoxLayout();
    QLabel *lblTitolo = new QLabel("Titolo:", this);
    lblTitolo->setStyleSheet(" font-weight: bold;");
    TitoloInput = new QLineEdit(this);
    TitoloInput->setPlaceholderText("Cosa cerchi?");
    TitoloInput->setStyleSheet("color: white;");

    layoutTitolo->addWidget(lblTitolo);
    layoutTitolo->addWidget(TitoloInput);
    mainLayout->addLayout(layoutTitolo);

    // --- DATA
    QHBoxLayout *layoutData = new QHBoxLayout();
    QLabel *lblData = new QLabel("Data:", this);
    lblData->setStyleSheet(" font-weight: bold;");
    DataInput = new QDateEdit(QDate::currentDate(), this);
    DataInput->setCalendarPopup(true);

    layoutData->addWidget(lblData);
    layoutData->addWidget(DataInput);
    mainLayout->addLayout(layoutData);

    // --- ORA
    QHBoxLayout *layoutOra = new QHBoxLayout();
    QLabel *lblOra = new QLabel("Ora:", this);
    lblOra->setStyleSheet("font-weight: bold;");
    OraInput = new QTimeEdit(QTime::currentTime(), this);

    layoutOra->addWidget(lblOra);
    layoutOra->addWidget(OraInput);
    mainLayout->addLayout(layoutOra);

    // --- PRIORITÀ
    QHBoxLayout *layoutPrio = new QHBoxLayout();
    QLabel *lblPrio = new QLabel("Priorità:", this);
    lblPrio->setStyleSheet("font-weight: bold;");
    PrioritaInput = new QComboBox(this);
    PrioritaInput->addItems({"Tutte", "Bassa", "Media", "Alta"});

    layoutPrio->addWidget(lblPrio);
    layoutPrio->addWidget(PrioritaInput);
    mainLayout->addLayout(layoutPrio);

    // --- PULSANTE CERCA
    CercaButton = new QPushButton("CERCA", this);
    CercaButton->setStyleSheet("font-weight: bold; padding: 5px 15px;");
    mainLayout->addWidget(CercaButton);


    //connect(CercaButton, &QPushButton::clicked, this, &SearchBar::Ricerca);
    connect(CercaButton, &QPushButton::clicked, this, &SearchBar::cercaClicked);
}