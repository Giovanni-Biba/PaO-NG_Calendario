#include "research.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFrame>
#include <QDebug>


Research::Research(QWidget *parent) : QWidget(parent)
{
    // Layout Principale
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // --- PULSANTE INDIETRO ---
    buttonIndietro = new QPushButton("← Torna alla Home", this);
    buttonIndietro->setFixedWidth(150);
    buttonIndietro->setStyleSheet(
        "QPushButton { "
        "   background-color: #7f8c8d; color: white; border-radius: 5px; "
        "   padding: 8px; font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #95a5a6; }"
        );
    mainLayout->addWidget(buttonIndietro);

    // --- TITOLO PAGINA ---
    QLabel *title = new QLabel("RISULTATI DELLA RICERCA");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 10px 0;");
    mainLayout->addWidget(title);

    // --- LISTA RISULTATI ---
    listaRisultati = new QListWidget(this);
    listaRisultati->setStyleSheet("border: none; background: transparent;");
    listaRisultati->setSpacing(12);
    listaRisultati->setSelectionMode(QAbstractItemView::NoSelection);
    mainLayout->addWidget(listaRisultati);

    // Connessione per tornare indietro
    connect(buttonIndietro, &QPushButton::clicked, this, &Research::ritornaHome);

    // Carichiamo i dati iniziali dal file
    caricaDatiJson();
}

void Research::caricaDatiJson() {
    QFile file(pathFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Errore: Impossibile trovare il file JSON in" << pathFile;
        return;
    }

    QByteArray dati = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(dati);
    if (!doc.isNull() && doc.isObject()) {
        tutteLeAttivita = doc.object().value("agenda").toArray();
    }
}

void Research::eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QTime &ora, const QString &priorita) {
    // 'ora' non viene usata nel filtro per evitare conflitti
    Q_UNUSED(ora);

    //Pulizia della lista
    listaRisultati->clear();

    //Ricarichiamo i dati per sicurezza
    caricaDatiJson();

    QString queryTitolo = titolo.trimmed().toLower();

    //Ciclo di filtro
    for (const QJsonValue &val : std::as_const(tutteLeAttivita)) {
        QJsonObject obj = val.toObject();

        // Estrazione dati dal JSON
        QString jTitolo = obj.value("titolo").toString().toLower();
        QString jPriorita = obj.value("priorita").toString();
        // Convertiamo "yyyy-MM-dd" del JSON in QDate
        QDate jData = QDate::fromString(obj.value("data").toString(), "yyyy-MM-dd");

        //LOGICA DEL FILTRO
        //ok se la barra è vuota oppure se il titolo contiene la parola cercata
        bool matchTitolo = queryTitolo.isEmpty() || jTitolo.contains(queryTitolo);
        //deve essere lo stesso giorno
        bool matchData = (jData == data);
        //ok se l'utente ha scelto "Tutte" oppure se corrispondono
        bool matchPriorita = (priorita == "Tutte") || (jPriorita == priorita);

        if (matchTitolo && matchData && matchPriorita) {
            QWidget *cardWidget = creaCardAttivita(
                obj.value("tipo").toString("N/A"),
                obj.value("titolo").toString("Senza Titolo"),
                obj.value("descrizione").toString("..."),
                jPriorita,
                obj.value("ora").toString("--:--")
                );

            QListWidgetItem *item = new QListWidgetItem(listaRisultati);
            item->setSizeHint(cardWidget->sizeHint());
            listaRisultati->addItem(item);
            listaRisultati->setItemWidget(item, cardWidget);
        }
    }

    // Se non ci sono card, mostriamo un messaggio
    if (listaRisultati->count() == 0) {
        QLabel *noResult = new QLabel("Nessun impegno trovato per i filtri selezionati.");
        noResult->setAlignment(Qt::AlignCenter);
        noResult->setStyleSheet("color: #c0392b; font-style: italic; font-size: 16px; padding: 50px;");
        QListWidgetItem *item = new QListWidgetItem(listaRisultati);
        item->setSizeHint(noResult->sizeHint());
        listaRisultati->addItem(item);
        listaRisultati->setItemWidget(item, noResult);
    }
}

QWidget* Research::creaCardAttivita(const QString &tipo, const QString &titolo, const QString &descrizione, const QString &priorita, const QString &ora) {
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(5, 2, 5, 2);

    QFrame *card = new QFrame();
    card->setStyleSheet(
        "QFrame { "
        "   border: 2px solid #3498db; border-radius: 15px; "
        "   background-color: white; padding: 12px; "
        "}"
        );

    QVBoxLayout *vbox = new QVBoxLayout(card);

    // Intestazione
    QHBoxLayout *header = new QHBoxLayout();
    QLabel *lblTipo = new QLabel(tipo.toUpper());
    lblTipo->setStyleSheet("color: #e67e22; font-weight: bold; font-size: 11px; border: none;");
    QLabel *lblPrio = new QLabel("PRIORITÀ: " + priorita.toUpper());
    lblPrio->setStyleSheet("color: #7f8c8d; font-weight: bold; font-size: 10px; border: none;");
    header->addWidget(lblTipo);
    header->addStretch();
    header->addWidget(lblPrio);


    QHBoxLayout *midRow = new QHBoxLayout();
    QLabel *lblTitolo = new QLabel(titolo);
    lblTitolo->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; border: none;");
    QLabel *lblOra = new QLabel("🕒 " + ora);
    lblOra->setStyleSheet("font-size: 14px; font-weight: bold; color: #2980b9; border: none;");
    midRow->addWidget(lblTitolo);
    midRow->addStretch();
    midRow->addWidget(lblOra);
    QLabel *lblDesc = new QLabel(descrizione);
    lblDesc->setStyleSheet("color: #34495e; font-style: italic; border: none; padding-top: 5px;");
    lblDesc->setWordWrap(true);
    vbox->addLayout(header);
    vbox->addLayout(midRow);
    vbox->addWidget(lblDesc);

    layout->addWidget(card);
    return container;
}