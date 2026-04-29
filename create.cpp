#include "create.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDate>
#include <QTime>

Create::Create(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20,20,20,20);

    // 🔸 TITOLO
    QLabel *title = new QLabel("CREA");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    mainLayout->addWidget(title);

    // 🔸 RADIO BUTTON
    QHBoxLayout *radioLayout = new QHBoxLayout;

    rbAttivita = new QRadioButton("Attività");
    rbEvento = new QRadioButton("Evento");
    rbAppuntamento = new QRadioButton("Appuntamento");

    // nessuno selezionato all'inizio
    rbAttivita->setAutoExclusive(false);
    rbEvento->setAutoExclusive(false);
    rbAppuntamento->setAutoExclusive(false);

    radioLayout->addWidget(rbAttivita);
    radioLayout->addWidget(rbEvento);
    radioLayout->addWidget(rbAppuntamento);

    mainLayout->addLayout(radioLayout);

    // riattiva esclusività
    rbAttivita->setAutoExclusive(true);
    rbEvento->setAutoExclusive(true);
    rbAppuntamento->setAutoExclusive(true);

    // 🔸 TITOLO CAMPO
    QLabel *titoloLabel = new QLabel("Titolo");
    titoloEdit = new QLineEdit;

    mainLayout->addWidget(titoloLabel);
    mainLayout->addWidget(titoloEdit);

    // 🔸 DATA + ORA
    QHBoxLayout *dateTimeLayout = new QHBoxLayout;

    dataEdit = new QDateEdit(QDate::currentDate());
    dataEdit->setCalendarPopup(true);

    oraEdit = new QTimeEdit(QTime::currentTime());

    dateTimeLayout->addWidget(dataEdit);
    dateTimeLayout->addWidget(oraEdit);

    mainLayout->addLayout(dateTimeLayout);

    // 🔸 PRIORITÀ
    prioritaLabel = new QLabel("Priorità");
    prioritaBox = new QComboBox;
    prioritaBox->addItems({"Alta", "Media", "Bassa"});

    mainLayout->addWidget(prioritaLabel);
    mainLayout->addWidget(prioritaBox);

    // 🔸 LUOGO
    luogoLabel = new QLabel("Luogo");
    luogoEdit = new QLineEdit;

    mainLayout->addWidget(luogoLabel);
    mainLayout->addWidget(luogoEdit);

    // 🔸 STATO
    statoLabel = new QLabel("Stato");
    statoBox = new QComboBox;
    statoBox->addItems({"Italia", "USA", "Cina", "Worldwide"});

    mainLayout->addWidget(statoLabel);
    mainLayout->addWidget(statoBox);

    // 🔸 BOTTONE CREA
    creaButton = new QPushButton("Crea");
    creaButton->setMinimumHeight(35);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(creaButton);

    // 🔹 CONNESSIONI
    connect(rbAttivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbEvento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbAppuntamento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);

    // stato iniziale
    aggiornaVisibilita();

    // 🔸 STILE
    setStyleSheet(R"(
        QLabel {
            font-size: 14px;
        }
        QLineEdit, QComboBox, QDateEdit, QTimeEdit {
            padding: 6px;
            border: 1px solid #ccc;
            border-radius: 6px;
        }
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border-radius: 8px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
    )");
}

void Create::aggiornaVisibilita()
{
    if (rbAttivita->isChecked()) {
        // Attività → solo priorità
        prioritaLabel->show();
        prioritaBox->show();

        luogoLabel->hide();
        luogoEdit->hide();
        statoLabel->hide();
        statoBox->hide();
    }
    else if (rbEvento->isChecked()) {
        // Evento → solo luogo
        luogoLabel->show();
        luogoEdit->show();

        prioritaLabel->hide();
        prioritaBox->hide();
        statoLabel->hide();
        statoBox->hide();
    }
    else if (rbAppuntamento->isChecked()) {
        // Appuntamento → solo stato
        statoLabel->show();
        statoBox->show();

        prioritaLabel->hide();
        prioritaBox->hide();
        luogoLabel->hide();
        luogoEdit->hide();
    }
    else {
        // Nessuna selezione → nascondi tutto
        prioritaLabel->hide();
        prioritaBox->hide();
        luogoLabel->hide();
        luogoEdit->hide();
        statoLabel->hide();
        statoBox->hide();
    }
}