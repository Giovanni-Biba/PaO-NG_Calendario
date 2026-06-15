#include "create.h"
#include "appuntamento.h"
#include "archivioimpegni.h"
#include "attivita.h"
#include "consegna.h"
#include "evento.h"
#include "festivita.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimeEdit>
#include <QtGlobal>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFormLayout>

namespace
{
int durataOreDaIntervallo(const QDate &dataInizio, const QTime &oraInizio, const QDate &dataFine, const QTime &oraFine)
{
    const qint64 secondi = QDateTime(dataInizio, oraInizio).secsTo(QDateTime(dataFine, oraFine));
    return qMax(1, static_cast<int>(secondi / 3600));
}
}

Create::Create(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("CREA NUOVO IMPEGNO", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("background-color: transparent; color: #274C69; font-size: 22px; font-weight: bold; margin-bottom: 10px; border: none;");
    mainLayout->addWidget(title);

    // Selezione Tipo
    QHBoxLayout *radioLayout = new QHBoxLayout;
    rbAttivita = new QRadioButton("Attività", this);
    rbEvento = new QRadioButton("Evento", this);
    rbAppuntamento = new QRadioButton("Appuntamento", this);
    rbFestivita = new QRadioButton("Festività", this);
    rbConsegna = new QRadioButton("Consegna", this);
    rbAttivita->setChecked(true);

    radioLayout->addWidget(rbAttivita);
    radioLayout->addWidget(rbEvento);
    radioLayout->addWidget(rbAppuntamento);
    radioLayout->addWidget(rbFestivita);
    radioLayout->addWidget(rbConsegna);
    mainLayout->addLayout(radioLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *container = new QWidget();
    QFormLayout *formLayout = new QFormLayout(container);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    // Titolo
    titoloEdit = new QLineEdit(this);
    formLayout->addRow("Titolo (*):", titoloEdit);

    // Descrizione con ALTEZZA MASSIMA FISSA
    descrizioneEdit = new QTextEdit(this);
    descrizioneEdit->setMaximumHeight(80);
    formLayout->addRow("Descrizione (*):", descrizioneEdit);

    // Data e Ora Inizio
    QHBoxLayout *dateTimeLayout = new QHBoxLayout;
    dataEdit = new QDateEdit(QDate::currentDate(), this);
    dataEdit->setCalendarPopup(true);
    oraEdit = new QTimeEdit(QTime::currentTime(), this);
    dateTimeLayout->addWidget(dataEdit);
    dateTimeLayout->addWidget(oraEdit);
    formLayout->addRow("Data e ora:", dateTimeLayout);

    // Campi Attività (Scadenza)
    dataFineLabel = new QLabel("Data scadenza:", this);
    dataFineEdit = new QDateEdit(QDate::currentDate(), this);
    dataFineEdit->setCalendarPopup(true);
    oraFineLabel = new QLabel("Ora scadenza:", this);
    oraFineEdit = new QTimeEdit(QTime::currentTime().addSecs(3600), this);
    QHBoxLayout *dateTimeLayout2 = new QHBoxLayout;
    dateTimeLayout2->addWidget(dataFineLabel);
    dateTimeLayout2->addWidget(dataFineEdit);
    dateTimeLayout2->addWidget(oraFineLabel);
    dateTimeLayout2->addWidget(oraFineEdit);
    formLayout->addRow(dateTimeLayout2);

    // Priorità
    prioritaLabel = new QLabel("Priorità:", this);
    prioritaBox = new QComboBox(this);
    prioritaBox->addItems({"Alta", "Media", "Bassa"});
    formLayout->addRow(prioritaLabel, prioritaBox);



    // Durata
    durataLabel = new QLabel("Durata (ore):", this);
    durataSpin = new QSpinBox(this);
    durataSpin->setRange(1, 24);
    formLayout->addRow(durataLabel, durataSpin);

    // Categoria e Completata
    categoriaLabel = new QLabel("Categoria:", this);
    categoriaEdit = new QLineEdit(this);
    completataCheck = new QCheckBox("Segna come completata", this);
    formLayout->addRow(categoriaLabel, categoriaEdit);
    formLayout->addRow("", completataCheck);

    // Campi Evento / Appuntamento
    luogoLabel = new QLabel("Luogo (*):", this);
    luogoEdit = new QLineEdit(this);
    organizzatoreLabel = new QLabel("Organizzatore:", this);
    organizzatoreEdit = new QLineEdit(this);
    formLayout->addRow(luogoLabel, luogoEdit);
    formLayout->addRow(organizzatoreLabel, organizzatoreEdit);

    // Solo Evento
    capienzaLabel = new QLabel("Capienza max:", this);
    capienzaSpin = new QSpinBox(this);
    capienzaSpin->setRange(0, 100000);
    costoLabel = new QLabel("Costo biglietto (€):", this);
    costoSpin = new QSpinBox(this);
    costoSpin->setRange(0, 10000);
    pubblicoCheck = new QCheckBox("Evento pubblico", this);
    formLayout->addRow(capienzaLabel, capienzaSpin);
    formLayout->addRow(costoLabel, costoSpin);
    formLayout->addRow("", pubblicoCheck);

    // Solo Appuntamento
    partecipantiLabel = new QLabel("Partecipanti:", this);
    partecipantiEdit = new QLineEdit(this);
    partecipantiEdit->setPlaceholderText("Es: Mario, Luigi");
    modalitaLabel = new QLabel("Modalità:", this);
    modalitaBox = new QComboBox(this);
    modalitaBox->addItems({"Presenza", "Online", "Telefono"});
    linkOnlineLabel = new QLabel("Link online:", this);
    linkOnlineEdit = new QLineEdit(this);
    confermatoCheck = new QCheckBox("Confermato", this);
    formLayout->addRow(partecipantiLabel, partecipantiEdit);
    formLayout->addRow(modalitaLabel, modalitaBox);
    formLayout->addRow(linkOnlineLabel, linkOnlineEdit);
    formLayout->addRow("", confermatoCheck);

    // Solo Festività
    statoLabel = new QLabel("Stato:", this);
    statoBox = new QComboBox(this);
    statoBox->addItems({"Italia", "USA", "Cina", "Worldwide"});
    ricorrenzaCheck = new QCheckBox("Ricorrenza annuale", this);
    nonLavorativoCheck = new QCheckBox("Giorno non lavorativo", this);
    nomeUfficialeLabel = new QLabel("Nome ufficiale:", this);
    nomeUfficialeEdit = new QLineEdit(this);
    formLayout->addRow(statoLabel, statoBox);
    formLayout->addRow(nomeUfficialeLabel, nomeUfficialeEdit);
    formLayout->addRow("", ricorrenzaCheck);
    formLayout->addRow("", nonLavorativoCheck);

    // Solo Consegna
    materiaLabel = new QLabel("Materia/Progetto (*):", this);
    materiaEdit = new QLineEdit(this);
    destinatarioLabel = new QLabel("Destinatario:", this);
    destinatarioEdit = new QLineEdit(this);
    formatoLabel = new QLabel("Formato:", this);
    formatoBox = new QComboBox(this);
    formatoBox->addItems({"PDF", "ZIP", "Codice", "Relazione", "Altro"});
    piattaformaLabel = new QLabel("Piattaforma:", this);
    piattaformaEdit = new QLineEdit(this);
    consegnataCheck = new QCheckBox("Consegnata", this);
    formLayout->addRow(materiaLabel, materiaEdit);
    formLayout->addRow(destinatarioLabel, destinatarioEdit);
    formLayout->addRow(formatoLabel, formatoBox);
    formLayout->addRow(piattaformaLabel, piattaformaEdit);
    formLayout->addRow("", consegnataCheck);

    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);

    // Pulsanti Azione
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    indietroButton = new QPushButton("Indietro", this);
    indietroButton->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; font-weight: bold; padding: 8px;} QPushButton:hover { background-color: #9aa6ab; }");
    creaButton = new QPushButton("Crea ", this);
    creaButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; font-weight: bold; padding: 8px;} QPushButton:hover { background-color: #2ecc71; }");

    buttonLayout->addWidget(indietroButton);
    buttonLayout->addWidget(creaButton);
    mainLayout->addLayout(buttonLayout);

    // Connessioni
    connect(creaButton, &QPushButton::clicked, this, &Create::salvaDati);
    connect(indietroButton, &QPushButton::clicked, this, &Create::tornaIndietro);

    connect(rbAttivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbEvento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbAppuntamento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbFestivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbConsegna, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);

    aggiornaVisibilita();
}

void Create::aggiornaVisibilita()
{
    const bool attivita = rbAttivita->isChecked();
    const bool evento = rbEvento->isChecked();
    const bool appuntamento = rbAppuntamento->isChecked();
    const bool festivita = rbFestivita->isChecked();
    const bool consegna = rbConsegna->isChecked();

    oraEdit->setVisible(!festivita);
    prioritaLabel->setVisible(!festivita);
    prioritaBox->setVisible(!festivita);

    const bool haDurata = !festivita && !attivita;
    durataLabel->setVisible(haDurata);
    durataSpin->setVisible(haDurata);

    // Attività
    dataFineLabel->setVisible(attivita);
    dataFineEdit->setVisible(attivita);
    oraFineLabel->setVisible(attivita);
    oraFineEdit->setVisible(attivita);
    categoriaLabel->setVisible(attivita);
    categoriaEdit->setVisible(attivita);
    completataCheck->setVisible(attivita);

    // Luogo/Organizzatore (Evento o Appuntamento)
    luogoLabel->setVisible(evento || appuntamento);
    luogoEdit->setVisible(evento || appuntamento);
    organizzatoreLabel->setVisible(evento || appuntamento);
    organizzatoreEdit->setVisible(evento || appuntamento);

    // Evento
    capienzaLabel->setVisible(evento);
    capienzaSpin->setVisible(evento);
    costoLabel->setVisible(evento);
    costoSpin->setVisible(evento);
    pubblicoCheck->setVisible(evento);

    // Appuntamento
    partecipantiLabel->setVisible(appuntamento);
    partecipantiEdit->setVisible(appuntamento);
    modalitaLabel->setVisible(appuntamento);
    modalitaBox->setVisible(appuntamento);
    linkOnlineLabel->setVisible(appuntamento);
    linkOnlineEdit->setVisible(appuntamento);
    confermatoCheck->setVisible(appuntamento);

    // Festività
    statoLabel->setVisible(festivita);
    statoBox->setVisible(festivita);
    ricorrenzaCheck->setVisible(festivita);
    nonLavorativoCheck->setVisible(festivita);
    nomeUfficialeLabel->setVisible(festivita);
    nomeUfficialeEdit->setVisible(festivita);

    // Consegna
    materiaLabel->setVisible(consegna);
    materiaEdit->setVisible(consegna);
    destinatarioLabel->setVisible(consegna);
    destinatarioEdit->setVisible(consegna);
    formatoLabel->setVisible(consegna);
    formatoBox->setVisible(consegna);
    piattaformaLabel->setVisible(consegna);
    piattaformaEdit->setVisible(consegna);
    consegnataCheck->setVisible(consegna);
}

void Create::salvaDati()
{
    if (!validaCampi()) {
        QMessageBox::warning(this, "Errore", "Compila tutti i campi obbligatori e controlla le date.");
        return;
    }

    const auto impegno = creaImpegnoDaForm();
    if (!ArchivioImpegni::instance().aggiungi(impegno)) {
        QMessageBox::critical(this, "Errore", "Errore durante il salvataggio.");
        return;
    }

    QMessageBox::information(this, "OK", "Impegno creato con successo!");
    emit tornaIndietro();
}

bool Create::validaCampi()
{
    if (titoloEdit->text().trimmed().isEmpty()) return false;
    if (descrizioneEdit->toPlainText().trimmed().isEmpty()) return false;

    if (rbAttivita->isChecked()) {
        const QDateTime inizio(dataEdit->date(), oraEdit->time());
        const QDateTime fine(dataFineEdit->date(), oraFineEdit->time());
        if (fine <= inizio) return false;
    }

    if ((rbEvento->isChecked() || rbAppuntamento->isChecked()) && luogoEdit->text().trimmed().isEmpty())
        return false;

    if (rbConsegna->isChecked() && materiaEdit->text().trimmed().isEmpty())
        return false;

    return true;
}

std::shared_ptr<Agenda> Create::creaImpegnoDaForm() const
{
    const QString titolo = titoloEdit->text();
    const QString descrizione = descrizioneEdit->toPlainText();
    const QDate data = dataEdit->date();
    const QTime ora = oraEdit->time();

    const int durata = rbAttivita->isChecked()
                           ? durataOreDaIntervallo(data, ora, dataFineEdit->date(), oraFineEdit->time())
                           : durataSpin->value();

    const Agenda::Priorita priorita = rbFestivita->isChecked()
                                          ? Agenda::Bassa
                                          : Agenda::prioritaFromString(prioritaBox->currentText());

    if (rbAttivita->isChecked()) {
        return std::make_shared<Attivita>(
            titolo, descrizione, data, ora, durata,
            priorita, completataCheck->isChecked(),
            dataFineEdit->date(), categoriaEdit->text()
            );
    }

    if (rbEvento->isChecked()) {
        auto ev = std::make_shared<Evento>(
            titolo, descrizione, data, ora, durata,
            luogoEdit->text(), organizzatoreEdit->text(), capienzaSpin->value(),
            costoSpin->value(), pubblicoCheck->isChecked()
            );
        ev->setPriorita(priorita);
        return ev;
    }

    if (rbAppuntamento->isChecked()) {
        auto app = std::make_shared<Appuntamento>(
            titolo, descrizione, data, ora, durata,
            luogoEdit->text(), organizzatoreEdit->text(),
            partecipantiEdit->text().split(",", Qt::SkipEmptyParts),
            Appuntamento::modalitaFromString(modalitaBox->currentText()),
            linkOnlineEdit->text(), confermatoCheck->isChecked()
            );
        app->setPriorita(priorita);
        return app;
    }

    if (rbFestivita->isChecked()) {
        return std::make_shared<Festivita>(
            titolo, descrizione, data,
            Festivita::statoFromString(statoBox->currentText()),
            ricorrenzaCheck->isChecked(),
            nonLavorativoCheck->isChecked(),
            nomeUfficialeEdit->text()
            );
    }

    auto cons = std::make_shared<Consegna>(
        titolo, descrizione, data, ora, durata,
        materiaEdit->text(), destinatarioEdit->text(),
        Consegna::formatoFromString(formatoBox->currentText()),
        piattaformaEdit->text(), consegnataCheck->isChecked()
        );
    cons->setPriorita(priorita);
    return cons;
}