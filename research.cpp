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
#include <QPushButton>

Research::Research(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // --- PULSANTE INDIETRO ---
    buttonIndietro = new QPushButton("← Torna alla Home", this);
    buttonIndietro->setFixedWidth(150);
    buttonIndietro->setStyleSheet(
        "QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; } "
        "QPushButton:hover { background-color: #95a5a6; }"
        );
    mainLayout->addWidget(buttonIndietro);

    // --- TITOLO PAGINA ---
    QLabel *titleLabel = new QLabel("RISULTATI DELLA RICERCA", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 10px 0;");
    mainLayout->addWidget(titleLabel);

    // --- LISTA RISULTATI ---
    listaRisultati = new QListWidget(this);
    listaRisultati->setStyleSheet("border: none; background: transparent;");
    listaRisultati->setSpacing(12);
    listaRisultati->setSelectionMode(QAbstractItemView::NoSelection);
    mainLayout->addWidget(listaRisultati);

    connect(buttonIndietro, &QPushButton::clicked, this, &Research::ritornaHome);
    caricaDatiJson();
}

void Research::caricaDatiJson() {
    QFile file(pathFile);
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isNull() && doc.isObject()) {
        tutteLeAttivita = doc.object().value("agenda").toArray();
    }
}

void Research::eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita) {
    listaRisultati->clear();
    caricaDatiJson();

    QString queryTitolo = titolo.trimmed().toLower();

    for (const QJsonValue &val : std::as_const(tutteLeAttivita)) {
        QJsonObject obj = val.toObject();

        QString jTitolo = obj.value("titolo").toString().toLower();
        QString jPriorita = obj.value("priorita").toString();
        QString jTipo = obj.value("tipo").toString();
        QString dataStr = obj.value("data").toString();
        QDate jData = QDate::fromString(dataStr, "yyyy-MM-dd");
        int jDurata = obj.value("durata_ore").toInt(0);

        // --- LOGICA FILTRO ---
        bool matchTestoOData = !queryTitolo.isEmpty() ? jTitolo.contains(queryTitolo) : (jData == data);
        bool matchPriorita = (priorita == "Tutte") || (jPriorita == priorita);
        bool matchTipo = (tipo == "Tutte") || (jTipo.toLower() == tipo.toLower());

        if (matchTestoOData && matchPriorita && matchTipo) {
            QWidget *cardWidget = creaCardAttivita(
                jTipo,
                obj.value("titolo").toString("Senza Titolo"),
                obj.value("descrizione").toString("..."),
                jPriorita,
                obj.value("ora").toString("--:--"),
                dataStr,
                jDurata
                );

            QListWidgetItem *item = new QListWidgetItem(listaRisultati);
            item->setSizeHint(cardWidget->sizeHint());
            listaRisultati->addItem(item);
            listaRisultati->setItemWidget(item, cardWidget);
        }
    }

    if (listaRisultati->count() == 0) {
        QLabel *noResult = new QLabel("Nessun impegno trovato.");
        noResult->setAlignment(Qt::AlignCenter);
        noResult->setStyleSheet("color: #c0392b; font-size: 16px; padding: 50px;");
        QListWidgetItem *item = new QListWidgetItem(listaRisultati);
        item->setSizeHint(noResult->sizeHint());
        listaRisultati->addItem(item);
        listaRisultati->setItemWidget(item, noResult);
    }
}

QWidget* Research::creaCardAttivita(const QString &tipo, const QString &titolo, const QString &descrizione, const QString &priorita, const QString &ora, const QString &data, int durata) {
    QWidget *container = new QWidget();
    QVBoxLayout *mainVLayout = new QVBoxLayout(container);
    mainVLayout->setContentsMargins(5, 2, 5, 2);

    QFrame *card = new QFrame();
    card->setStyleSheet("QFrame { border: 2px solid #3498db; border-radius: 15px; background-color: white; padding: 12px; }");

    // Layout Orizzontale principale della card
    QHBoxLayout *cardHLayout = new QHBoxLayout(card);

    QVBoxLayout *leftLayout = new QVBoxLayout();

    QLabel *lblTipo = new QLabel(tipo.toUpper());
    lblTipo->setStyleSheet("color: #e67e22; font-weight: bold; font-size: 11px; border: none;");
    leftLayout->addWidget(lblTipo);

    QLabel *lblTitolo = new QLabel("<b>" + titolo + "</b>");
    lblTitolo->setStyleSheet("font-size: 18px; color: #2c3e50; border: none;");
    leftLayout->addWidget(lblTitolo);

    QLabel *lblDesc = new QLabel(descrizione);
    lblDesc->setStyleSheet("color: #34495e; font-style: italic; border: none;");
    lblDesc->setWordWrap(true);
    leftLayout->addWidget(lblDesc);

    cardHLayout->addLayout(leftLayout, 3); // Prende più spazio

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

    // Priorità
    QString pUpper = priorita.toUpper().isEmpty() ? "N/D" : priorita.toUpper();
    QLabel *lblPrio = new QLabel("PRIORITÀ: " + pUpper);
    QString stilePrio = "font-weight: bold; font-size: 11px; border: none; color: ";
    if (pUpper == "ALTA") stilePrio += "red;";
    else if (pUpper == "MEDIA") stilePrio += "orange;";
    else stilePrio += "green;";
    lblPrio->setStyleSheet(stilePrio);
    lblPrio->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(lblPrio);

    // Calendario
    QLabel *lblTempo = new QLabel(QString("📅 %1\n🕒 %2 (⏳ %3h)").arg(data, ora, QString::number(durata)));
    lblTempo->setStyleSheet("font-size: 11px; color: #7f8c8d; border: none;");
    lblTempo->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(lblTempo);

    rightLayout->addStretch(); // Spazio vuoto

    //Pulsante per pagina visualizza al momento solo creato
    QPushButton *btnVisualizza = new QPushButton("VISUALIZZA");
    btnVisualizza->setFixedWidth(100);
    btnVisualizza->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; border-radius: 8px; padding: 5px; font-weight: bold; font-size: 10px; border: none; } "
        "QPushButton:hover { background-color: #2980b9; }"
        );
    rightLayout->addWidget(btnVisualizza);

    cardHLayout->addLayout(rightLayout, 1);

    mainVLayout->addWidget(card);
    return container;
}