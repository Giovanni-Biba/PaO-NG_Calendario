#include "searchbar.h"
#include <QHBoxLayout>
#include <QLabel>

SearchBar::SearchBar(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(20);

    // --- TITOLO
    QHBoxLayout *layoutTitolo = new QHBoxLayout();
    layoutTitolo->addWidget(new QLabel("Titolo:", this));
    TitoloInput = new QLineEdit(this);
    TitoloInput->setPlaceholderText("Cerca nel titolo...");
    layoutTitolo->addWidget(TitoloInput);
    mainLayout->addLayout(layoutTitolo);

    // --- DATA
    QHBoxLayout *layoutData = new QHBoxLayout();
    DataCheck = new QCheckBox(this);
    layoutData->addWidget(DataCheck);

    QLabel *lblData = new QLabel("Data:", this);
    layoutData->addWidget(lblData);

    DataInput = new QDateEdit(QDate::currentDate(), this);
    DataInput->setCalendarPopup(true);
    DataInput->setEnabled(false);
    layoutData->addWidget(DataInput);
    mainLayout->addLayout(layoutData);

    connect(DataCheck, &QCheckBox::toggled, DataInput, &QDateEdit::setEnabled);

    QHBoxLayout *layoutTipo = new QHBoxLayout();
    layoutTipo->addWidget(new QLabel("Tipo:", this));
    TipoInput = new QComboBox(this);
    TipoInput->addItems({"Tutte", "Attivita", "Evento", "Appuntamento", "Festivita", "Consegna"});
    layoutTipo->addWidget(TipoInput);
    mainLayout->addLayout(layoutTipo);

    QHBoxLayout *layoutPrio = new QHBoxLayout();
    layoutPrio->addWidget(new QLabel("Priorità:", this));
    PrioritaInput = new QComboBox(this);
    PrioritaInput->addItems({"Tutte", "Bassa", "Media", "Alta"});
    layoutPrio->addWidget(PrioritaInput);
    mainLayout->addLayout(layoutPrio);

    CercaButton = new QPushButton("CERCA", this);
    CercaButton->setStyleSheet("QPushButton { font-weight: bold; padding: 5px 15px; background-color: #3498db; color: white; border-radius: 4px;} QPushButton:hover { background-color: #4DADEB; }");
    mainLayout->addWidget(CercaButton);

    connect(CercaButton, &QPushButton::clicked, this, &SearchBar::cercaClicked);
}