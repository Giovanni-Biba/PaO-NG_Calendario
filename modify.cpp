#include "modify.h"

#include "appuntamento.h"
#include "archivioimpegni.h"
#include "attivita.h"
#include "consegna.h"
#include "evento.h"
#include "festivita.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimeEdit>
#include <QVBoxLayout>

modify::modify(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    QHBoxLayout *topBar = new QHBoxLayout();
    QPushButton *indietro = new QPushButton("← Torna al dettaglio", this);
    indietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    topBar->addWidget(indietro);
    topBar->addStretch();
    mainLayout->addLayout(topBar);

    titoloPagina = new QLabel("MODIFICA IMPEGNO", this);
    titoloPagina->setAlignment(Qt::AlignCenter);
    titoloPagina->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50;");
    mainLayout->addWidget(titoloPagina);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none; background: transparent;");

    QWidget *formContainer = new QWidget();
    QFormLayout *formLayout = new QFormLayout(formContainer);

    editTitolo = new QLineEdit(this);
    editData = new QDateEdit(this);
    editData->setCalendarPopup(true);
    editOra = new QTimeEdit(this);
    spinDurata = new QSpinBox(this);
    spinDurata->setRange(1, 24);
    spinDurata->setSuffix(" ore");
    editDescrizione = new QTextEdit(this);
    editDescrizione->setMaximumHeight(100);

    comboPriorita = new QComboBox(this);
    comboPriorita->addItems({"Bassa", "Media", "Alta"});
    labelPriorita = new QLabel("Priorità:", this);
    labelCategoria = new QLabel("Categoria:", this);
    editCategoria = new QLineEdit(this);
    checkCompletata = new QCheckBox("Completata", this);

    labelLuogo = new QLabel("Luogo:", this);
    editLuogo = new QLineEdit(this);
    labelOrganizzatore = new QLabel("Organizzatore:", this);
    editOrganizzatore = new QLineEdit(this);
    labelStato = new QLabel("Stato:", this);
    comboStato = new QComboBox(this);
    comboStato->addItems({"Italia", "USA", "Cina", "Worldwide"});
    checkConfermato = new QCheckBox("Confermato", this);

    labelMateria = new QLabel("Materia/progetto:", this);
    editMateria = new QLineEdit(this);
    labelDestinatario = new QLabel("Destinatario:", this);
    editDestinatario = new QLineEdit(this);
    labelFormato = new QLabel("Formato:", this);
    comboFormato = new QComboBox(this);
    comboFormato->addItems({"PDF", "ZIP", "Codice", "Relazione", "Altro"});
    labelPiattaforma = new QLabel("Piattaforma:", this);
    editPiattaforma = new QLineEdit(this);
    checkConsegnata = new QCheckBox("Consegnata", this);

    formLayout->addRow("Titolo:", editTitolo);
    formLayout->addRow("Data:", editData);
    formLayout->addRow("Ora:", editOra);
    formLayout->addRow("Durata:", spinDurata);
    formLayout->addRow("Descrizione:", editDescrizione);
    formLayout->addRow(labelPriorita, comboPriorita);
    formLayout->addRow(labelCategoria, editCategoria);
    formLayout->addRow("", checkCompletata);
    formLayout->addRow(labelLuogo, editLuogo);
    formLayout->addRow(labelOrganizzatore, editOrganizzatore);
    formLayout->addRow(labelStato, comboStato);
    formLayout->addRow("", checkConfermato);
    formLayout->addRow(labelMateria, editMateria);
    formLayout->addRow(labelDestinatario, editDestinatario);
    formLayout->addRow(labelFormato, comboFormato);
    formLayout->addRow(labelPiattaforma, editPiattaforma);
    formLayout->addRow("", checkConsegnata);

    scrollArea->setWidget(formContainer);
    mainLayout->addWidget(scrollArea);

    QPushButton *btnSalva = new QPushButton("Salva Modifiche", this);
    btnSalva->setStyleSheet("background-color: #27ae60; color: white; border-radius: 5px; padding: 12px; font-weight: bold;");
    mainLayout->addWidget(btnSalva);

    connect(indietro, &QPushButton::clicked, this, &modify::tornaIndietro);
    connect(btnSalva, &QPushButton::clicked, this, [this]() {
        if (!elementoCorrente)
            return;

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Conferma", "Vuoi salvare le modifiche?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return;

        const auto nuovo = creaModificato();
        if (ArchivioImpegni::instance().aggiorna(elementoCorrente, nuovo)) {
            elementoCorrente = nuovo;
            QMessageBox::information(this, "Ok", "Modifica completata!");
            emit salvataggioCompletato(nuovo);
        } else {
            QMessageBox::critical(this, "Errore", "Errore nel salvataggio su file.");
        }
    });
}

void modify::caricaElemento(std::shared_ptr<Agenda> elemento)
{
    elementoCorrente = elemento;
    if (!elementoCorrente)
        return;

    titoloPagina->setText("MODIFICA " + elementoCorrente->getTipo().toUpper());
    editTitolo->setText(elementoCorrente->getTitolo());
    editData->setDate(elementoCorrente->getData());
    editOra->setTime(elementoCorrente->getOra());
    spinDurata->setValue(elementoCorrente->getDurataOre());
    editDescrizione->setPlainText(elementoCorrente->getDescrizione());

    if (const auto attivita = std::dynamic_pointer_cast<Attivita>(elementoCorrente)) {
        comboPriorita->setCurrentText(attivita->prioritaToString());
        editCategoria->setText(attivita->getCategoria());
        checkCompletata->setChecked(attivita->isCompletata());
    }

    if (const auto evento = std::dynamic_pointer_cast<Evento>(elementoCorrente)) {
        editLuogo->setText(evento->getLuogo());
        editOrganizzatore->setText(evento->getOrganizzatore());
    }

    if (const auto appuntamento = std::dynamic_pointer_cast<Appuntamento>(elementoCorrente))
        checkConfermato->setChecked(appuntamento->isConfermato());

    if (const auto festivita = std::dynamic_pointer_cast<Festivita>(elementoCorrente))
        comboStato->setCurrentText(festivita->statoToString());

    if (const auto consegna = std::dynamic_pointer_cast<Consegna>(elementoCorrente)) {
        editMateria->setText(consegna->getMateriaOProgetto());
        editDestinatario->setText(consegna->getDestinatario());
        comboFormato->setCurrentText(consegna->formatoToString());
        editPiattaforma->setText(consegna->getPiattaforma());
        checkConsegnata->setChecked(consegna->isConsegnata());
    }

    aggiornaVisibilita();
}

void modify::aggiornaVisibilita()
{
    const bool attivita = std::dynamic_pointer_cast<Attivita>(elementoCorrente) != nullptr;
    const bool evento = std::dynamic_pointer_cast<Evento>(elementoCorrente) != nullptr;
    const bool appuntamento = std::dynamic_pointer_cast<Appuntamento>(elementoCorrente) != nullptr;
    const bool festivita = std::dynamic_pointer_cast<Festivita>(elementoCorrente) != nullptr;
    const bool consegna = std::dynamic_pointer_cast<Consegna>(elementoCorrente) != nullptr;

    labelPriorita->setVisible(attivita);
    comboPriorita->setVisible(attivita);
    labelCategoria->setVisible(attivita);
    editCategoria->setVisible(attivita);
    checkCompletata->setVisible(attivita);

    labelLuogo->setVisible(evento);
    editLuogo->setVisible(evento);
    labelOrganizzatore->setVisible(evento);
    editOrganizzatore->setVisible(evento);
    checkConfermato->setVisible(appuntamento);

    labelStato->setVisible(festivita);
    comboStato->setVisible(festivita);

    labelMateria->setVisible(consegna);
    editMateria->setVisible(consegna);
    labelDestinatario->setVisible(consegna);
    editDestinatario->setVisible(consegna);
    labelFormato->setVisible(consegna);
    comboFormato->setVisible(consegna);
    labelPiattaforma->setVisible(consegna);
    editPiattaforma->setVisible(consegna);
    checkConsegnata->setVisible(consegna);
}

std::shared_ptr<Agenda> modify::creaModificato() const
{
    const QString titolo = editTitolo->text();
    const QString descrizione = editDescrizione->toPlainText();
    const QDate data = editData->date();
    const QTime ora = editOra->time();
    const int durata = spinDurata->value();

    if (std::dynamic_pointer_cast<Attivita>(elementoCorrente)) {
        return std::make_shared<Attivita>(
            titolo, descrizione, data, ora, durata,
            Attivita::prioritaFromString(comboPriorita->currentText()),
            checkCompletata->isChecked(), data, editCategoria->text()
        );
    }

    if (std::dynamic_pointer_cast<Appuntamento>(elementoCorrente)) {
        return std::make_shared<Appuntamento>(
            titolo, descrizione, data, ora, durata,
            editLuogo->text(), editOrganizzatore->text(), QStringList(),
            Appuntamento::Presenza, QString(), checkConfermato->isChecked()
        );
    }

    if (std::dynamic_pointer_cast<Evento>(elementoCorrente)) {
        return std::make_shared<Evento>(
            titolo, descrizione, data, ora, durata,
            editLuogo->text(), editOrganizzatore->text(), 0, 0.0, false
        );
    }

    if (std::dynamic_pointer_cast<Festivita>(elementoCorrente)) {
        return std::make_shared<Festivita>(
            titolo, descrizione, data,
            Festivita::statoFromString(comboStato->currentText()),
            true, false, titolo
        );
    }

    return std::make_shared<Consegna>(
        titolo, descrizione, data, ora, durata,
        editMateria->text(), editDestinatario->text(),
        Consegna::formatoFromString(comboFormato->currentText()),
        editPiattaforma->text(), checkConsegnata->isChecked()
    );
}
