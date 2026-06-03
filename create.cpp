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

    QLabel *title = new QLabel("CREA", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    mainLayout->addWidget(title);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    rbAttivita = new QRadioButton("Attività", this);
    rbEvento = new QRadioButton("Evento", this);
    rbAppuntamento = new QRadioButton("Appuntamento", this);
    rbFestivita = new QRadioButton("Festività", this);
    rbConsegna = new QRadioButton("Consegna", this);
    radioLayout->addWidget(rbAttivita);
    radioLayout->addWidget(rbEvento);
    radioLayout->addWidget(rbAppuntamento);
    radioLayout->addWidget(rbFestivita);
    radioLayout->addWidget(rbConsegna);
    mainLayout->addLayout(radioLayout);

    QLabel *titoloLabel = new QLabel("Titolo", this);
    titoloEdit = new QLineEdit(this);
    mainLayout->addWidget(titoloLabel);
    mainLayout->addWidget(titoloEdit);

    descrizioneLabel = new QLabel("Descrizione", this);
    descrizioneEdit = new QTextEdit(this);
    mainLayout->addWidget(descrizioneLabel);
    mainLayout->addWidget(descrizioneEdit);

    QHBoxLayout *dateTimeLayout = new QHBoxLayout;
    dataEdit = new QDateEdit(QDate::currentDate(), this);
    dataEdit->setCalendarPopup(true);
    oraEdit = new QTimeEdit(QTime::currentTime(), this);
    dateTimeLayout->addWidget(dataEdit);
    dateTimeLayout->addWidget(oraEdit);
    mainLayout->addLayout(dateTimeLayout);

    prioritaLabel = new QLabel("Priorità", this);
    prioritaBox = new QComboBox(this);
    prioritaBox->addItems({"Alta", "Media", "Bassa"});
    mainLayout->addWidget(prioritaLabel);
    mainLayout->addWidget(prioritaBox);

    dataFineLabel = new QLabel("Data scadenza / fine", this);
    dataFineEdit = new QDateEdit(QDate::currentDate(), this);
    dataFineEdit->setCalendarPopup(true);
    mainLayout->addWidget(dataFineLabel);
    mainLayout->addWidget(dataFineEdit);

    oraFineLabel = new QLabel("Ora fine", this);
    oraFineEdit = new QTimeEdit(QTime::currentTime(), this);
    mainLayout->addWidget(oraFineLabel);
    mainLayout->addWidget(oraFineEdit);

    durataLabel = new QLabel("Durata (ore)", this);
    durataSpin = new QSpinBox(this);
    durataSpin->setRange(1, 24);
    durataSpin->setValue(1);
    mainLayout->addWidget(durataLabel);
    mainLayout->addWidget(durataSpin);

    categoriaLabel = new QLabel("Categoria", this);
    categoriaEdit = new QLineEdit(this);
    completataCheck = new QCheckBox("Completata", this);
    mainLayout->addWidget(categoriaLabel);
    mainLayout->addWidget(categoriaEdit);
    mainLayout->addWidget(completataCheck);

    luogoLabel = new QLabel("Luogo", this);
    luogoEdit = new QLineEdit(this);
    organizzatoreLabel = new QLabel("Organizzatore", this);
    organizzatoreEdit = new QLineEdit(this);
    capienzaLabel = new QLabel("Capienza massima", this);
    capienzaSpin = new QSpinBox(this);
    capienzaSpin->setRange(0, 100000);
    costoLabel = new QLabel("Costo biglietto", this);
    costoSpin = new QSpinBox(this);
    costoSpin->setRange(0, 10000);
    pubblicoCheck = new QCheckBox("Evento pubblico", this);
    mainLayout->addWidget(luogoLabel);
    mainLayout->addWidget(luogoEdit);
    mainLayout->addWidget(organizzatoreLabel);
    mainLayout->addWidget(organizzatoreEdit);
    mainLayout->addWidget(capienzaLabel);
    mainLayout->addWidget(capienzaSpin);
    mainLayout->addWidget(costoLabel);
    mainLayout->addWidget(costoSpin);
    mainLayout->addWidget(pubblicoCheck);

    partecipantiLabel = new QLabel("Partecipanti (separati da virgola)", this);
    partecipantiEdit = new QLineEdit(this);
    modalitaLabel = new QLabel("Modalità", this);
    modalitaBox = new QComboBox(this);
    modalitaBox->addItems({"Presenza", "Online", "Telefono"});
    linkOnlineLabel = new QLabel("Link online", this);
    linkOnlineEdit = new QLineEdit(this);
    confermatoCheck = new QCheckBox("Confermato", this);
    mainLayout->addWidget(partecipantiLabel);
    mainLayout->addWidget(partecipantiEdit);
    mainLayout->addWidget(modalitaLabel);
    mainLayout->addWidget(modalitaBox);
    mainLayout->addWidget(linkOnlineLabel);
    mainLayout->addWidget(linkOnlineEdit);
    mainLayout->addWidget(confermatoCheck);

    statoLabel = new QLabel("Stato", this);
    statoBox = new QComboBox(this);
    statoBox->addItems({"Italia", "USA", "Cina", "Worldwide"});
    ricorrenzaCheck = new QCheckBox("Ricorrenza annuale", this);
    ricorrenzaCheck->setChecked(true);
    nonLavorativoCheck = new QCheckBox("Giorno non lavorativo", this);
    nomeUfficialeLabel = new QLabel("Nome ufficiale", this);
    nomeUfficialeEdit = new QLineEdit(this);
    mainLayout->addWidget(statoLabel);
    mainLayout->addWidget(statoBox);
    mainLayout->addWidget(ricorrenzaCheck);
    mainLayout->addWidget(nonLavorativoCheck);
    mainLayout->addWidget(nomeUfficialeLabel);
    mainLayout->addWidget(nomeUfficialeEdit);

    materiaLabel = new QLabel("Materia o progetto", this);
    materiaEdit = new QLineEdit(this);
    destinatarioLabel = new QLabel("Destinatario", this);
    destinatarioEdit = new QLineEdit(this);
    formatoLabel = new QLabel("Formato", this);
    formatoBox = new QComboBox(this);
    formatoBox->addItems({"PDF", "ZIP", "Codice", "Relazione", "Altro"});
    piattaformaLabel = new QLabel("Piattaforma", this);
    piattaformaEdit = new QLineEdit(this);
    consegnataCheck = new QCheckBox("Consegnata", this);
    mainLayout->addWidget(materiaLabel);
    mainLayout->addWidget(materiaEdit);
    mainLayout->addWidget(destinatarioLabel);
    mainLayout->addWidget(destinatarioEdit);
    mainLayout->addWidget(formatoLabel);
    mainLayout->addWidget(formatoBox);
    mainLayout->addWidget(piattaformaLabel);
    mainLayout->addWidget(piattaformaEdit);
    mainLayout->addWidget(consegnataCheck);

    creaButton = new QPushButton("Crea", this);
    mainLayout->addWidget(creaButton);
    connect(creaButton, &QPushButton::clicked, this, &Create::salvaDati);

    indietroButton = new QPushButton("Indietro", this);
    mainLayout->addWidget(indietroButton);
    connect(indietroButton, &QPushButton::clicked, this, &Create::tornaIndietro);

    aggiornaVisibilita();
    connect(rbAttivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbEvento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbAppuntamento, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbFestivita, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
    connect(rbConsegna, &QRadioButton::toggled, this, &Create::aggiornaVisibilita);
}

void Create::aggiornaVisibilita()
{
    const bool attivita = rbAttivita->isChecked();
    const bool evento = rbEvento->isChecked();
    const bool appuntamento = rbAppuntamento->isChecked();
    const bool festivita = rbFestivita->isChecked();
    const bool consegna = rbConsegna->isChecked();

    const bool haDurata = !festivita && !attivita;
    durataLabel->setVisible(haDurata);
    durataSpin->setVisible(haDurata);
    oraEdit->setVisible(!festivita);
    oraFineLabel->setVisible(attivita);
    oraFineEdit->setVisible(attivita);

    prioritaLabel->setVisible(!festivita);
    prioritaBox->setVisible(!festivita);
    dataFineLabel->setVisible(attivita);
    dataFineEdit->setVisible(attivita);
    categoriaLabel->setVisible(attivita);
    categoriaEdit->setVisible(attivita);
    completataCheck->setVisible(attivita);

    luogoLabel->setVisible(evento || appuntamento);
    luogoEdit->setVisible(evento || appuntamento);
    organizzatoreLabel->setVisible(evento || appuntamento);
    organizzatoreEdit->setVisible(evento || appuntamento);
    capienzaLabel->setVisible(evento);
    capienzaSpin->setVisible(evento);
    costoLabel->setVisible(evento);
    costoSpin->setVisible(evento);
    pubblicoCheck->setVisible(evento);

    partecipantiLabel->setVisible(appuntamento);
    partecipantiEdit->setVisible(appuntamento);
    modalitaLabel->setVisible(appuntamento);
    modalitaBox->setVisible(appuntamento);
    linkOnlineLabel->setVisible(appuntamento);
    linkOnlineEdit->setVisible(appuntamento);
    confermatoCheck->setVisible(appuntamento);

    statoLabel->setVisible(festivita);
    statoBox->setVisible(festivita);
    ricorrenzaCheck->setVisible(festivita);
    nonLavorativoCheck->setVisible(festivita);
    nomeUfficialeLabel->setVisible(festivita);
    nomeUfficialeEdit->setVisible(festivita);

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
        QMessageBox::warning(this, "Errore", "Compila tutti i campi obbligatori");
        return;
    }

    const auto impegno = creaImpegnoDaForm();
    if (!ArchivioImpegni::instance().aggiungi(impegno)) {
        QMessageBox::critical(this, "Errore", "Errore durante il salvataggio");
        return;
    }

    QMessageBox::information(this, "OK", "Agenda salvato");
    emit tornaIndietro();
}

bool Create::validaCampi()
{
    if (titoloEdit->text().trimmed().isEmpty())
        return false;
    if (descrizioneEdit->toPlainText().trimmed().isEmpty())
        return false;
    if (!rbAttivita->isChecked() && !rbEvento->isChecked() && !rbAppuntamento->isChecked() && !rbFestivita->isChecked() && !rbConsegna->isChecked())
        return false;
    if (rbAttivita->isChecked()) {
        const QDateTime inizio(dataEdit->date(), oraEdit->time());
        const QDateTime fine(dataFineEdit->date(), oraFineEdit->time());
        if (fine <= inizio)
            return false;
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
    const Agenda::Priorita priorita = Agenda::prioritaFromString(prioritaBox->currentText());

    if (rbAttivita->isChecked()) {
        return std::make_shared<Attivita>(
            titolo, descrizione, data, ora, durata,
            priorita,
            completataCheck->isChecked(),
            dataFineEdit->date(),
            categoriaEdit->text()
        );
    }

    if (rbEvento->isChecked()) {
        auto evento = std::make_shared<Evento>(
            titolo, descrizione, data, ora, durata,
            luogoEdit->text(), organizzatoreEdit->text(), capienzaSpin->value(),
            costoSpin->value(), pubblicoCheck->isChecked()
        );
        evento->setPriorita(priorita);
        return evento;
    }

    if (rbAppuntamento->isChecked()) {
        auto appuntamento = std::make_shared<Appuntamento>(
            titolo, descrizione, data, ora, durata,
            luogoEdit->text(), organizzatoreEdit->text(),
            partecipantiEdit->text().split(",", Qt::SkipEmptyParts),
            Appuntamento::modalitaFromString(modalitaBox->currentText()),
            linkOnlineEdit->text(), confermatoCheck->isChecked()
        );
        appuntamento->setPriorita(priorita);
        return appuntamento;
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

    auto consegna = std::make_shared<Consegna>(
        titolo, descrizione, data, ora, durata,
        materiaEdit->text(), destinatarioEdit->text(),
        Consegna::formatoFromString(formatoBox->currentText()),
        piattaformaEdit->text(), consegnataCheck->isChecked()
    );
    consegna->setPriorita(priorita);
    return consegna;
}
