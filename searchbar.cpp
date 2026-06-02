#include "searchbar.h"
#include <QHBoxLayout>
#include <QLabel>

SearchBar::SearchBar(QWidget *parent)
    : QWidget{parent}
{
    // Layout orizzontale principale per tutta la barra
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(30);

    // --- TITOLO
    QHBoxLayout *layoutTitolo = new QHBoxLayout();
    QLabel *lblTitolo = new QLabel("Titolo:", this);
    lblTitolo->setStyleSheet("font-weight: bold;");
    TitoloInput = new QLineEdit(this);
    TitoloInput->setPlaceholderText("Cosa cerchi?");
    // Rimosso color: white se lo sfondo è chiaro, altrimenti non si vede il testo
    TitoloInput->setStyleSheet("padding: 3px;");

    layoutTitolo->addWidget(lblTitolo);
    layoutTitolo->addWidget(TitoloInput);
    mainLayout->addLayout(layoutTitolo);

    // --- DATA
    QHBoxLayout *layoutData = new QHBoxLayout();
    QLabel *lblData = new QLabel("Data:", this);
    lblData->setStyleSheet("font-weight: bold;");
    DataInput = new QDateEdit(QDate::currentDate(), this);
    DataInput->setCalendarPopup(true);

    layoutData->addWidget(lblData);
    layoutData->addWidget(DataInput);
    mainLayout->addLayout(layoutData);

    // --- TIPO (Sostituisce l'Ora nel tuo vecchio file)
    QHBoxLayout *layoutTipo = new QHBoxLayout();
    QLabel *lblTipo = new QLabel("Tipo:", this);
    lblTipo->setStyleSheet("font-weight: bold;");
    TipoInput = new QComboBox(this);
    TipoInput->addItems({"Tutte", "Attivita", "Evento", "Appuntamento", "Festivita", "Consegna"});

    layoutTipo->addWidget(lblTipo);
    layoutTipo->addWidget(TipoInput);
    mainLayout->addLayout(layoutTipo);

    // --- ORA
    /*QHBoxLayout *layoutOra = new QHBoxLayout();
    QLabel *lblOra = new QLabel("Ora:", this);
    lblOra->setStyleSheet("font-weight: bold;");
    OraInput = new QTimeEdit(QTime::currentTime(), this);

    layoutOra->addWidget(lblOra);
    layoutOra->addWidget(OraInput);
    mainLayout->addLayout(layoutOra);
    */
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
    CercaButton->setStyleSheet("font-weight: bold; padding: 5px 15px; background-color: #3498db; color: white; border-radius: 4px;");
    mainLayout->addWidget(CercaButton);

    // Connessione al segnale per la pagina Research
    connect(CercaButton, &QPushButton::clicked, this, &SearchBar::cercaClicked);
}
