#include "searchbar.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QTimer>


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

    // modifiche seconda consegna: utilizzo timer per evitare una ricerca completa a ogni singolo carattere (per una questione di performance).
    timerRicerca = new QTimer(this);
    timerRicerca->setSingleShot(true);
    timerRicerca->setInterval(250);
    connect(timerRicerca, &QTimer::timeout, this, &SearchBar::filtriCambiati);

    connect(CercaButton, &QPushButton::clicked, this, [this]() {
        timerRicerca->stop();
        emit cercaClicked();
    });

    auto programmaRicerca = [this]() {
        timerRicerca->start();
    };

    connect(TitoloInput, &QLineEdit::textChanged, this, programmaRicerca);
    connect(DataCheck, &QCheckBox::toggled, this, programmaRicerca);
    connect(DataInput, &QDateEdit::dateChanged, this, programmaRicerca);
    connect(TipoInput, &QComboBox::currentTextChanged, this, programmaRicerca);
    connect(PrioritaInput, &QComboBox::currentTextChanged, this, programmaRicerca);
}

void SearchBar::impostaFiltri(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita)
{
    // modifiche seconda consegna: aggiorna i controlli senza emettere ricerche intermedie.
    const bool bloccoTitolo = TitoloInput->blockSignals(true);
    const bool bloccoDataCheck = DataCheck->blockSignals(true);
    const bool bloccoData = DataInput->blockSignals(true);
    const bool bloccoTipo = TipoInput->blockSignals(true);
    const bool bloccoPriorita = PrioritaInput->blockSignals(true);

    TitoloInput->setText(titolo);
    DataCheck->setChecked(data.isValid());
    DataInput->setEnabled(data.isValid());
    if (data.isValid())
        DataInput->setDate(data);
    TipoInput->setCurrentText(tipo);
    PrioritaInput->setCurrentText(priorita);

    TitoloInput->blockSignals(bloccoTitolo);
    DataCheck->blockSignals(bloccoDataCheck);
    DataInput->blockSignals(bloccoData);
    TipoInput->blockSignals(bloccoTipo);
    PrioritaInput->blockSignals(bloccoPriorita);
}
