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
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    QHBoxLayout *topButtons = new QHBoxLayout();

    buttonIndietro = new QPushButton("← Indietro", this);
    buttonModifica = new QPushButton("Modifica", this);
    buttonElimina = new QPushButton("Elimina", this);

    buttonIndietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    buttonModifica->setStyleSheet("QPushButton { background-color: #f39c12; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    buttonElimina->setStyleSheet("QPushButton { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");

    topButtons->addWidget(buttonIndietro);
    topButtons->addStretch();
    topButtons->addWidget(buttonModifica);
    topButtons->addWidget(buttonElimina);
    mainLayout->addLayout(topButtons);

    titoloPagina = new QLabel("DETTAGLIO IMPEGNO", this);
    titoloPagina->setAlignment(Qt::AlignCenter);
    titoloPagina->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 10px 0;");
    mainLayout->addWidget(titoloPagina);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("border: none;");

    QWidget *contenuto = new QWidget(scroll);
    dettagliLayout = new QVBoxLayout(contenuto);
    dettagliLayout->setSpacing(10);
    dettagliLayout->setContentsMargins(8, 8, 8, 8);
    dettagliLayout->addStretch();

    scroll->setWidget(contenuto);
    mainLayout->addWidget(scroll);

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

    titoloPagina->setText(elementoCorrente->getTitolo().toUpper());
    aggiungiRiga("Tipo", elementoCorrente->getTipo());
    aggiungiRiga("Titolo", elementoCorrente->getTitolo());
    aggiungiRiga("Descrizione", elementoCorrente->getDescrizione());
    aggiungiRiga("Data", elementoCorrente->getData().toString("dd/MM/yyyy"));
    aggiungiRiga("Ora", elementoCorrente->getOra().toString("HH:mm"));
    aggiungiRiga("Durata", QString::number(elementoCorrente->getDurataOre()) + " ore");
    aggiungiRiga("Riepilogo", elementoCorrente->riepilogo());

    const QMap<QString, QString> specifici = elementoCorrente->campiSpecifici();
    for (auto it = specifici.begin(); it != specifici.end(); ++it)
        aggiungiRiga(it.key(), it.value());

    dettagliLayout->addStretch();
}

void visualize::aggiungiRiga(const QString &nome, const QString &valore)
{
    QFrame *card = new QFrame(this);
    card->setStyleSheet("QFrame { border: 1px solid #dfe6e9; border-radius: 8px; background-color: white; padding: 10px; }");

    QHBoxLayout *layout = new QHBoxLayout(card);

    QLabel *labelNome = new QLabel(nome.left(1).toUpper() + nome.mid(1), card);
    labelNome->setMinimumWidth(130);
    labelNome->setStyleSheet("font-weight: bold; color: #2c3e50; border: none;");

    QLabel *labelValore = new QLabel(valore.isEmpty() ? "N/D" : valore, card);
    labelValore->setWordWrap(true);
    labelValore->setStyleSheet("color: #34495e; border: none;");

    layout->addWidget(labelNome);
    layout->addWidget(labelValore, 1);
    dettagliLayout->addWidget(card);
}

void visualize::eliminaElemento()
{
    if (!elementoCorrente)
        return;

    const int risposta = QMessageBox::question(
        this,
        "Conferma eliminazione",
        "Vuoi eliminare definitivamente questo impegno?"
    );

    if (risposta != QMessageBox::Yes)
        return;

    if (!ArchivioImpegni::instance().elimina(elementoCorrente)) {
        QMessageBox::warning(this, "Errore", "Non sono riuscito a eliminare l'impegno selezionato.");
        return;
    }

    QMessageBox::information(this, "Eliminato", "Agenda eliminato correttamente.");
    elementoCorrente.reset();
    emit elementoEliminato();
}
