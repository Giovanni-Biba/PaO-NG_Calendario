#include "visualize.h"
#include "archivioimpegni.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

visualize::visualize(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Barra pulsanti superiore
    QHBoxLayout *topButtons = new QHBoxLayout();
    buttonIndietro = new QPushButton("← Indietro", this);
    buttonModifica = new QPushButton("Modifica", this);
    buttonElimina = new QPushButton("Elimina", this);

    buttonIndietro->setCursor(Qt::PointingHandCursor);
    buttonModifica->setCursor(Qt::PointingHandCursor);
    buttonElimina->setCursor(Qt::PointingHandCursor);

    buttonIndietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 10px 15px; font-weight: bold; } QPushButton:hover { background-color: #95a5a6; }");
    buttonModifica->setStyleSheet("QPushButton { background-color: #3498db; color: white; border-radius: 5px; padding: 10px 15px; font-weight: bold; } QPushButton:hover { background-color: #2980b9; }");
    buttonElimina->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; border-radius: 5px; padding: 10px 15px; font-weight: bold; } QPushButton:hover { background-color: #c0392b; }");

    topButtons->addWidget(buttonIndietro);
    topButtons->addStretch();
    topButtons->addWidget(buttonModifica);
    topButtons->addWidget(buttonElimina);
    mainLayout->addLayout(topButtons);

    titoloPagina = new QLabel("DETTAGLIO IMPEGNO", this);
    titoloPagina->setAlignment(Qt::AlignCenter);
    titoloPagina->setStyleSheet("font-size: 26px; font-weight: bold; color: #274C69; margin: 15px 0; border: none;");
    mainLayout->addWidget(titoloPagina);

    // Scroll Area
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background: transparent;");

    QWidget *contenuto = new QWidget();
    contenuto->setStyleSheet("background: transparent;");
    dettagliLayout = new QVBoxLayout(contenuto);
    dettagliLayout->setSpacing(15);
    dettagliLayout->setContentsMargins(10, 10, 10, 10);
    dettagliLayout->addStretch();

    scroll->setWidget(contenuto);
    mainLayout->addWidget(scroll);

    // Connessioni
    connect(buttonIndietro, &QPushButton::clicked, this, &visualize::tornaIndietro);
    connect(buttonModifica, &QPushButton::clicked, this, [this]() {
        emit richiestaModifica(elementoCorrente);
    });
    connect(buttonElimina, &QPushButton::clicked, this, &visualize::eliminaElemento);
}

void visualize::caricaElemento(std::shared_ptr<Agenda> elemento)
{
    elementoCorrente = elemento;
    aggiornaVista();
}

void visualize::aggiornaVista()
{
    // Pulizia layout
    while (QLayoutItem *item = dettagliLayout->takeAt(0)) {
        if (item->widget())
            item->widget()->deleteLater();
        delete item;
    }

    if (!elementoCorrente) {
        aggiungiRiga("Stato", "Nessun elemento selezionato");
        dettagliLayout->addStretch();
        return;
    }

    // Aggiornamento Titolo Principale
    titoloPagina->setText(elementoCorrente->getTitolo().toUpper());

    // Elenco dettagli (simile allo stile del "Crea")
    aggiungiRiga("Tipo:", elementoCorrente->getTipo());
    aggiungiRiga("Titolo:", elementoCorrente->getTitolo());
    aggiungiRiga("Descrizione:", elementoCorrente->getDescrizione());
    aggiungiRiga("Data:", elementoCorrente->getData().toString("dd MMMM yyyy"));

    // Mostra Ora e Durata solo se non è una Festività (coerente con Create/Modify)
    if (elementoCorrente->getTipo().toLower() != "festività") {
        aggiungiRiga("Ora:", elementoCorrente->getOra().toString("HH:mm"));
        aggiungiRiga("Durata:", QString::number(elementoCorrente->getDurataOre()) + " ore");
    }

    if (!elementoCorrente->usaRigaFestivita())
        aggiungiRiga("Priorità:", elementoCorrente->prioritaToString());

    // Campi specifici delle sottoclassi
    const QMap<QString, QString> specifici = elementoCorrente->campiSpecifici();
    for (auto it = specifici.begin(); it != specifici.end(); ++it) {
        // Formatta la chiave (es: "linkOnline" -> "Link Online")
        QString label = it.key();
        aggiungiRiga(label + ":", it.value());
    }

    dettagliLayout->addStretch();
}

void visualize::aggiungiRiga(const QString &nome, const QString &valore)
{
    QFrame *rigaFrame = new QFrame(this);
    rigaFrame->setStyleSheet("border: none; background: transparent;");

    QHBoxLayout *layout = new QHBoxLayout(rigaFrame);
    layout->setContentsMargins(0, 5, 0, 5);

    QLabel *labelNome = new QLabel(nome, rigaFrame);
    labelNome->setMinimumWidth(150);
    labelNome->setMaximumWidth(150);
    labelNome->setStyleSheet("font-weight: bold; color: #46484A; font-size: 14px; border: none;");
    labelNome->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLabel *labelValore = new QLabel(valore.isEmpty() ? "-" : valore, rigaFrame);
    labelValore->setWordWrap(true);
    labelValore->setStyleSheet("color: #4B88BF; font-size: 15px; border: none; padding-left: 10px;");

    layout->addWidget(labelNome);
    layout->addWidget(labelValore, 1);

    dettagliLayout->insertWidget(dettagliLayout->count() - 1, rigaFrame);
}

void visualize::eliminaElemento()
{
    if (!elementoCorrente)
        return;

    const int risposta = QMessageBox::question(
        this,
        "Conferma eliminazione",
        "Sei sicuro di voler eliminare definitivamente questo impegno?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (risposta != QMessageBox::Yes)
        return;

    if (!ArchivioImpegni::instance().elimina(elementoCorrente)) {
        QMessageBox::warning(this, "Errore", "Impossibile eliminare l'impegno.");
        return;
    }

    QMessageBox::information(this, "Eliminato", "Impegno rimosso correttamente.");
    elementoCorrente.reset();
    emit elementoEliminato();
}