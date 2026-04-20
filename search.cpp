#include "search.h"
#include <QHBoxLayout>
#include <QLabel>

Search::Search(QWidget *parent)
    : QWidget{parent}
{
    // Stile base
    this->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ccc;");

    // Layout orizzontale principale per tutta la barra
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(30);

    // --- TITOLO
    QHBoxLayout *layoutTitolo = new QHBoxLayout();
    QLabel *lblTitolo = new QLabel("Titolo:");
    lblTitolo->setStyleSheet("color: white; font-weight: bold;");
    TitoloInput = new QLineEdit();
    TitoloInput->setPlaceholderText("Cosa cerchi?");
    TitoloInput->setStyleSheet("color: white;");

    layoutTitolo->addWidget(lblTitolo);
    layoutTitolo->addWidget(TitoloInput);
    mainLayout->addLayout(layoutTitolo);

    // --- DATA
    QHBoxLayout *layoutData = new QHBoxLayout();
    QLabel *lblData = new QLabel("Data:");
    lblData->setStyleSheet("color: white; font-weight: bold;");
    DataInput = new QDateEdit(QDate::currentDate());
    DataInput->setCalendarPopup(true);
    DataInput->setStyleSheet("color: white;");

    layoutData->addWidget(lblData);
    layoutData->addWidget(DataInput);
    mainLayout->addLayout(layoutData);

    // --- ORA
    QHBoxLayout *layoutOra = new QHBoxLayout();
    QLabel *lblOra = new QLabel("Ora:");
    lblOra->setStyleSheet("color: white; font-weight: bold;");
    OraInput = new QTimeEdit(QTime::currentTime());
    OraInput->setStyleSheet("color: white;");

    layoutOra->addWidget(lblOra);
    layoutOra->addWidget(OraInput);
    mainLayout->addLayout(layoutOra);

    // --- PRIORITÀ
    QHBoxLayout *layoutPrio = new QHBoxLayout();
    QLabel *lblPrio = new QLabel("Priorità:");
    lblPrio->setStyleSheet("color: white; font-weight: bold;");
    PrioritaInput = new QComboBox();
    PrioritaInput->addItems({"Tutte", "Bassa", "Media", "Alta"});
    PrioritaInput->setStyleSheet("color: white;");

    layoutPrio->addWidget(lblPrio);
    layoutPrio->addWidget(PrioritaInput);
    mainLayout->addLayout(layoutPrio);

    // --- PULSANTE CERCA
    CercaButton = new QPushButton("CERCA");
    CercaButton->setStyleSheet("font-weight: bold; padding: 5px 15px;");
    mainLayout->addWidget(CercaButton);
}