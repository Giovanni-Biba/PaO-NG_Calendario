#include "research.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFrame>

Research::Research(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *title = new QLabel("TUTTE LE ATTIVITÀ");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50;");
    mainLayout->addWidget(title);

    listaRisultati = new QListWidget(this);
    listaRisultati->setStyleSheet("border: none; background: transparent;");
    listaRisultati->setSpacing(10);
    listaRisultati->setSelectionMode(QAbstractItemView::NoSelection);

    mainLayout->addWidget(listaRisultati);

    // Carica i dati dal file e li fa visualizzare
    caricaDatiJson();
    visualizzaTutto();
}

void Research::caricaDatiJson() {
    QFile file(pathFile);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray dati = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(dati);
        tutteLeAttivita = doc.object().value("agenda").toArray();
        file.close();
    }
}

void Research::visualizzaTutto() {
    listaRisultati->clear();

    // Ricarichiamo i dati ogni volta che entriamo nella pagina
    // per essere sicuri di avere l'ultima versione del JSON
    caricaDatiJson();

    // Usiamo 'const QJsonValue' per sicurezza e stabilità
    for (const QJsonValue &val : std::as_const(tutteLeAttivita)) {
        if (!val.isObject()) continue; // Salta se l'elemento non è un oggetto valido

        QJsonObject obj = val.toObject();

        QWidget *cardWidget = creaCardAttivita(
            obj.value("tipo").toString("N/A"),       // Valore di default se manca
            obj.value("titolo").toString("Senza Titolo"),
            obj.value("descrizione").toString("...")
            );

        QListWidgetItem *item = new QListWidgetItem(listaRisultati);
        item->setSizeHint(cardWidget->sizeHint());
        listaRisultati->addItem(item);
        listaRisultati->setItemWidget(item, cardWidget);
    }
}

QWidget* Research::creaCardAttivita(const QString &tipo, const QString &titolo, const QString &descrizione) {
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(5, 5, 5, 5);

    QFrame *card = new QFrame();
    card->setStyleSheet(
        "QFrame { "
        "   border: 2px solid #3498db; "
        "   border-radius: 12px; "
        "   background-color: white; "
        "   padding: 10px; "
        "}"
        );

    QVBoxLayout *vbox = new QVBoxLayout(card);

    QLabel *lblTipo = new QLabel(tipo.toUpper());
    lblTipo->setStyleSheet("color: #e67e22; font-weight: bold; border: none; font-size: 11px;");

    QLabel *lblTitolo = new QLabel(titolo);
    lblTitolo->setStyleSheet("font-size: 17px; font-weight: bold; border: none; color: #2c3e50;");

    QLabel *lblDesc = new QLabel(descrizione);
    lblDesc->setStyleSheet("color: #7f8c8d; border: none; font-style: italic;");
    lblDesc->setWordWrap(true);

    vbox->addWidget(lblTipo);
    vbox->addWidget(lblTitolo);
    vbox->addWidget(lblDesc);

    layout->addWidget(card);
    return container;
}