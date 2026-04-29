#include "searchbar.h"
#include <QHBoxLayout>
#include <QLabel>

SearchBar::SearchBar(QWidget *parent)
    : QWidget{parent}
{
    this->setStyleSheet("background-color: #f0f0f0; border: 1px solid lightgrey;");

    // Layout orizzontale principale per tutta la barra
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(30);

    // --- TITOLO
    QHBoxLayout *layoutTitolo = new QHBoxLayout();
    QLabel *lblTitolo = new QLabel("Titolo:");
    lblTitolo->setStyleSheet(" font-weight: bold;");
    TitoloInput = new QLineEdit();
    TitoloInput->setPlaceholderText("Cosa cerchi?");
    TitoloInput->setStyleSheet("color: white;");

    layoutTitolo->addWidget(lblTitolo);
    layoutTitolo->addWidget(TitoloInput);
    mainLayout->addLayout(layoutTitolo);

    // --- DATA
    QHBoxLayout *layoutData = new QHBoxLayout();
    QLabel *lblData = new QLabel("Data:");
    lblData->setStyleSheet(" font-weight: bold;");
    DataInput = new QDateEdit(QDate::currentDate());
    DataInput->setCalendarPopup(true);

    layoutData->addWidget(lblData);
    layoutData->addWidget(DataInput);
    mainLayout->addLayout(layoutData);

    // --- ORA
    QHBoxLayout *layoutOra = new QHBoxLayout();
    QLabel *lblOra = new QLabel("Ora:");
    lblOra->setStyleSheet("font-weight: bold;");
    OraInput = new QTimeEdit(QTime::currentTime());

    layoutOra->addWidget(lblOra);
    layoutOra->addWidget(OraInput);
    mainLayout->addLayout(layoutOra);

    // --- PRIORITÀ
    QHBoxLayout *layoutPrio = new QHBoxLayout();
    QLabel *lblPrio = new QLabel("Priorità:");
    lblPrio->setStyleSheet("font-weight: bold;");
    PrioritaInput = new QComboBox();
    PrioritaInput->addItems({"Tutte", "Bassa", "Media", "Alta"});

    layoutPrio->addWidget(lblPrio);
    layoutPrio->addWidget(PrioritaInput);
    mainLayout->addLayout(layoutPrio);

    // --- PULSANTE CERCA
    CercaButton = new QPushButton("CERCA");
    CercaButton->setStyleSheet("font-weight: bold; padding: 5px 15px;");
    mainLayout->addWidget(CercaButton);


    //connect(CercaButton, &QPushButton::clicked, this, &SearchBar::Ricerca);
    connect(CercaButton, &QPushButton::clicked, this, &SearchBar::cercaClicked);
}

/*void SearchBar::Ricerca(){
    RicercaConfig config;
    if(!TitoloInput->text().trimmed().isEmpty()){
        config.testo = TitoloInput->text();
    }
    if(DataInput->date().isValid()){
        config.data = DataInput->date();
    }
    if(OraInput->time().isValid()){
        config.orario = OraInput->time();
    }
    QString temp = PrioritaInput->currentText();
    if(!temp.isEmpty()){
        config.priorita = temp;
    }
    ricerca *r = new ricerca(config);
    r->showMaximized();
    this->close();*/