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
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QTimeEdit>
#include <QVBoxLayout>

modify::modify(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Barra superiore
    QHBoxLayout *topBar = new QHBoxLayout();
    QPushButton *indietro = new QPushButton("← Torna al dettaglio", this);
    indietro->setCursor(Qt::PointingHandCursor);
    indietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #9aa6ab; }");
    topBar->addWidget(indietro);
    topBar->addStretch();
    mainLayout->addLayout(topBar);

    titoloPagina = new QLabel("MODIFICA IMPEGNO", this);
    titoloPagina->setAlignment(Qt::AlignCenter);
    titoloPagina->setStyleSheet("QLabel { background-color: transparent; color: #274C69; font-size: 26px; font-weight: bold; margin: 10px; border: none; }");
    mainLayout->addWidget(titoloPagina);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background: transparent;");

    QWidget *formContainer = new QWidget();
    formContainer->setObjectName("formCard");
    formContainer->setStyleSheet(
        "QCheckBox { color: palette(text); font-weight: bold; spacing: 8px; } "
        "QCheckBox::indicator { width: 20px; height: 20px; border: 2px solid #7f8c8d; border-radius: 4px; background-color: transparent; } "
        "QCheckBox::indicator:checked { background-color: #2ecc71; border: 2px solid #27ae60; }"
        );

    QFormLayout *formLayout = new QFormLayout(formContainer);
    formLayout->setContentsMargins(25, 25, 25, 25);
    formLayout->setSpacing(18);

    auto creaLabel = [this](const QString &testo) {
        QLabel *l = new QLabel(testo, this);
        l->setObjectName("campoLabel");
        return l;
    };

    // --- Inizializzazione Widget ---
    editTitolo = new QLineEdit(this);
    editData = new QDateEdit(this);
    editData->setCalendarPopup(true);
    labelOra = creaLabel("Ora:");
    editOra = new QTimeEdit(this);

    labelDurata = creaLabel("Durata (ore):");
    spinDurata = new QSpinBox(this);
    spinDurata->setRange(1, 48);

    editDescrizione = new QTextEdit(this);
    editDescrizione->setMaximumHeight(80);

    // Attività
    comboPriorita = new QComboBox(this);
    comboPriorita->addItems({"Bassa", "Media", "Alta"});
    labelPriorita = creaLabel("Priorità:");
    labelCategoria = creaLabel("Categoria:");
    editCategoria = new QLineEdit(this);
    checkCompletata = new QCheckBox("Segna come completata", this);

    // Evento
    labelLuogo = creaLabel("Luogo:");
    editLuogo = new QLineEdit(this);
    labelOrganizzatore = creaLabel("Organizzatore:");
    editOrganizzatore = new QLineEdit(this);
    labelCapienza = creaLabel("Capienza Max:");
    spinCapienza = new QSpinBox(this);
    spinCapienza->setRange(0, 100000);
    labelCosto = creaLabel("Costo Biglietto:");
    spinCosto = new QDoubleSpinBox(this);
    spinCosto->setRange(0, 9999.99);
    spinCosto->setSuffix(" €");
    checkPubblico = new QCheckBox("Evento Pubblico", this);

    // Appuntamento
    labelModalita = creaLabel("Modalità:");
    comboModalita = new QComboBox(this);
    comboModalita->addItems({"Presenza", "Online", "Telefono"});
    labelPartecipanti = creaLabel("Partecipanti:");
    editPartecipanti = new QLineEdit(this);
    editPartecipanti->setPlaceholderText("Separa con ,");
    labelLinkOnline = creaLabel("Link Online:");
    editLinkOnline = new QLineEdit(this);
    checkConfermato = new QCheckBox("Confermato", this);

    // Festività
    labelStato = creaLabel("Stato:");
    comboStato = new QComboBox(this);
    comboStato->addItems({"Italia", "USA", "Cina", "Worldwide"});
    labelNomeUfficiale = creaLabel("Nome Ufficiale:");
    editNomeUfficiale = new QLineEdit(this);
    checkRicorrenza = new QCheckBox("Ricorrenza Annuale", this);
    checkLavorativo = new QCheckBox("Giorno Non Lavorativo", this);

    // Consegna
    labelMateria = creaLabel("Materia:");
    editMateria = new QLineEdit(this);
    labelDestinatario = creaLabel("Destinatario:");
    editDestinatario = new QLineEdit(this);
    labelFormato = creaLabel("Formato:");
    comboFormato = new QComboBox(this);
    comboFormato->addItems({"PDF", "ZIP", "Codice", "Relazione", "Altro"});
    labelPiattaforma = creaLabel("Piattaforma:");
    editPiattaforma = new QLineEdit(this);
    checkConsegnata = new QCheckBox("Consegnata", this);

    // --- Montaggio Layout ---
    formLayout->addRow(creaLabel("Titolo:"), editTitolo);
    formLayout->addRow(creaLabel("Data:"), editData);
    formLayout->addRow(labelOra, editOra);
    formLayout->addRow(labelDurata, spinDurata);
    formLayout->addRow(creaLabel("Descrizione:"), editDescrizione);

    formLayout->addRow(labelPriorita, comboPriorita);
    formLayout->addRow(labelCategoria, editCategoria);
    formLayout->addRow("", checkCompletata);

    formLayout->addRow(labelLuogo, editLuogo);
    formLayout->addRow(labelOrganizzatore, editOrganizzatore);
    formLayout->addRow(labelCapienza, spinCapienza);
    formLayout->addRow(labelCosto, spinCosto);
    formLayout->addRow("", checkPubblico);

    formLayout->addRow(labelModalita, comboModalita);
    formLayout->addRow(labelPartecipanti, editPartecipanti);
    formLayout->addRow(labelLinkOnline, editLinkOnline);
    formLayout->addRow("", checkConfermato);

    formLayout->addRow(labelStato, comboStato);
    formLayout->addRow(labelNomeUfficiale, editNomeUfficiale);
    formLayout->addRow("", checkRicorrenza);
    formLayout->addRow("", checkLavorativo);

    formLayout->addRow(labelMateria, editMateria);
    formLayout->addRow(labelDestinatario, editDestinatario);
    formLayout->addRow(labelFormato, comboFormato);
    formLayout->addRow(labelPiattaforma, editPiattaforma);
    formLayout->addRow("", checkConsegnata);

    scrollArea->setWidget(formContainer);
    mainLayout->addWidget(scrollArea);

    QPushButton *btnSalva = new QPushButton("SALVA MODIFICHE", this);
    btnSalva->setCursor(Qt::PointingHandCursor);
    btnSalva->setStyleSheet("QPushButton { background-color: #27ae60; color: white; border-radius: 10px; padding: 15px; font-size: 16px; font-weight: bold; } QPushButton:hover { background-color: #2ecc71; }");
    mainLayout->addWidget(btnSalva);

    // --- Connessioni ---
    connect(indietro, &QPushButton::clicked, this, &modify::tornaIndietro);
    connect(btnSalva, &QPushButton::clicked, this, [this]() {
        if (!elementoCorrente) return;
        QMessageBox customMsg(this);
        customMsg.setWindowTitle("Conferma");
        customMsg.setText("Vuoi salvare le modifiche?");
        customMsg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if (customMsg.exec() == QMessageBox::No) return;

        const auto nuovo = creaModificato();
        if (ArchivioImpegni::instance().aggiorna(elementoCorrente, nuovo)) {
            elementoCorrente = nuovo;
            QMessageBox::information(this, "Ok", "Modifica completata!");
            emit salvataggioCompletato(nuovo);
        } else {
            QMessageBox::critical(this, "Errore", "Errore nel salvataggio.");
        }
    });
}

void modify::caricaElemento(std::shared_ptr<Agenda> elemento)
{
    elementoCorrente = elemento;
    if (!elementoCorrente) return;

    titoloPagina->setText("MODIFICA " + elementoCorrente->getTipo().toUpper());
    editTitolo->setText(elementoCorrente->getTitolo());
    editData->setDate(elementoCorrente->getData());
    editOra->setTime(elementoCorrente->getOra());
    spinDurata->setValue(elementoCorrente->getDurataOre());
    editDescrizione->setPlainText(elementoCorrente->getDescrizione());

    if (!elementoCorrente->usaRigaFestivita())
        comboPriorita->setCurrentText(elementoCorrente->prioritaToString());

    if (const auto att = std::dynamic_pointer_cast<Attivita>(elementoCorrente)) {
        editCategoria->setText(att->getCategoria());
        checkCompletata->setChecked(att->isCompletata());
    }
    if (const auto ev = std::dynamic_pointer_cast<Evento>(elementoCorrente)) {
        editLuogo->setText(ev->getLuogo());
        editOrganizzatore->setText(ev->getOrganizzatore());
        spinCapienza->setValue(ev->getCapienzaMassima());
        spinCosto->setValue(ev->getCostoBiglietto());
        checkPubblico->setChecked(ev->isPubblico());
    }
    if (const auto app = std::dynamic_pointer_cast<Appuntamento>(elementoCorrente)) {
        comboModalita->setCurrentText(app->modalitaToString());
        editPartecipanti->setText(app->getPartecipanti().join("; "));
        editLinkOnline->setText(app->getLinkOnline());
        checkConfermato->setChecked(app->isConfermato());
    }
    if (const auto fest = std::dynamic_pointer_cast<Festivita>(elementoCorrente)) {
        comboStato->setCurrentText(fest->statoToString());
        editNomeUfficiale->setText(fest->getNomeUfficiale());
        checkRicorrenza->setChecked(fest->isRicorrenzaAnnuale());
        checkLavorativo->setChecked(fest->isGiornoNonLavorativo());
    }
    if (const auto cons = std::dynamic_pointer_cast<Consegna>(elementoCorrente)) {
        editMateria->setText(cons->getMateriaOProgetto());
        editDestinatario->setText(cons->getDestinatario());
        comboFormato->setCurrentText(cons->formatoToString());
        editPiattaforma->setText(cons->getPiattaforma());
        checkConsegnata->setChecked(cons->isConsegnata());
    }
    aggiornaVisibilita();
}

void modify::aggiornaVisibilita()
{
    bool att = std::dynamic_pointer_cast<Attivita>(elementoCorrente) != nullptr;
    bool ev = std::dynamic_pointer_cast<Evento>(elementoCorrente) != nullptr;
    bool app = std::dynamic_pointer_cast<Appuntamento>(elementoCorrente) != nullptr;
    bool fest = std::dynamic_pointer_cast<Festivita>(elementoCorrente) != nullptr;
    bool cons = std::dynamic_pointer_cast<Consegna>(elementoCorrente) != nullptr;

    labelOra->setVisible(!fest);
    editOra->setVisible(!fest);
    labelDurata->setVisible(!fest);
    spinDurata->setVisible(!fest);

    labelPriorita->setVisible(!fest); comboPriorita->setVisible(!fest);
    labelCategoria->setVisible(att); editCategoria->setVisible(att);
    checkCompletata->setVisible(att);

    labelLuogo->setVisible(ev || app); editLuogo->setVisible(ev || app);
    labelOrganizzatore->setVisible(ev || app); editOrganizzatore->setVisible(ev || app);
    labelCapienza->setVisible(ev && !app); spinCapienza->setVisible(ev && !app);
    labelCosto->setVisible(ev && !app); spinCosto->setVisible(ev && !app);
    checkPubblico->setVisible(ev && !app);

    labelModalita->setVisible(app); comboModalita->setVisible(app);
    labelPartecipanti->setVisible(app); editPartecipanti->setVisible(app);
    labelLinkOnline->setVisible(app); editLinkOnline->setVisible(app);
    checkConfermato->setVisible(app);

    labelStato->setVisible(fest); comboStato->setVisible(fest);
    labelNomeUfficiale->setVisible(fest); editNomeUfficiale->setVisible(fest);
    checkRicorrenza->setVisible(fest); checkLavorativo->setVisible(fest);

    labelMateria->setVisible(cons); editMateria->setVisible(cons);
    labelDestinatario->setVisible(cons); editDestinatario->setVisible(cons);
    labelFormato->setVisible(cons); comboFormato->setVisible(cons);
    labelPiattaforma->setVisible(cons); editPiattaforma->setVisible(cons);
    checkConsegnata->setVisible(cons);
}

std::shared_ptr<Agenda> modify::creaModificato() const
{
    QString tit = editTitolo->text();
    QString desc = editDescrizione->toPlainText();
    QDate d = editData->date();
    QTime o = editOra->time();
    int dur = spinDurata->value();
    Agenda::Priorita priorita = Agenda::prioritaFromString(comboPriorita->currentText());

    if (std::dynamic_pointer_cast<Attivita>(elementoCorrente)) {
        return std::make_shared<Attivita>(tit, desc, d, o, dur, priorita, checkCompletata->isChecked(), d, editCategoria->text());
    }
    if (std::dynamic_pointer_cast<Appuntamento>(elementoCorrente)) {
        auto appuntamento = std::make_shared<Appuntamento>(tit, desc, d, o, dur, editLuogo->text(), editOrganizzatore->text(), editPartecipanti->text().split(",", Qt::SkipEmptyParts), Appuntamento::modalitaFromString(comboModalita->currentText()), editLinkOnline->text(), checkConfermato->isChecked());
        appuntamento->setPriorita(priorita);
        return appuntamento;
    }
    if (std::dynamic_pointer_cast<Evento>(elementoCorrente)) {
        auto evento = std::make_shared<Evento>(tit, desc, d, o, dur, editLuogo->text(), editOrganizzatore->text(), spinCapienza->value(), spinCosto->value(), checkPubblico->isChecked());
        evento->setPriorita(priorita);
        return evento;
    }
    if (std::dynamic_pointer_cast<Festivita>(elementoCorrente)) {
        return std::make_shared<Festivita>(tit, desc, d, Festivita::statoFromString(comboStato->currentText()), checkRicorrenza->isChecked(), checkLavorativo->isChecked(), editNomeUfficiale->text());
    }
    auto consegna = std::make_shared<Consegna>(tit, desc, d, o, dur, editMateria->text(), editDestinatario->text(), Consegna::formatoFromString(comboFormato->currentText()), editPiattaforma->text(), checkConsegnata->isChecked());
    consegna->setPriorita(priorita);
    return consegna;
}
