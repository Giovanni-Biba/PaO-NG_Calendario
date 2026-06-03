#include "research.h"
#include "archivioimpegni.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

Research::Research(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    buttonIndietro = new QPushButton("← Torna alla Home", this);
    buttonIndietro->setFixedWidth(150);
    buttonIndietro->setStyleSheet(
        "QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; } "
        "QPushButton:hover { background-color: #95a5a6; }"
        );
    mainLayout->addWidget(buttonIndietro);

    QLabel *titleLabel = new QLabel("RISULTATI DELLA RICERCA", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #274C69; margin: 10px 0;");
    mainLayout->addWidget(titleLabel);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none; background: transparent;");

    containerRisultati = new QWidget();
    containerRisultati->setStyleSheet("background: transparent;");
    gridRisultati = new QGridLayout(containerRisultati);
    gridRisultati->setSpacing(15);
    gridRisultati->setContentsMargins(0, 0, 0, 0);
    gridRisultati->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(containerRisultati);
    mainLayout->addWidget(scrollArea);

    connect(buttonIndietro, &QPushButton::clicked, this, &Research::ritornaHomeSlot);
}

void Research::eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita)
{
    QLayoutItem *item;
    while ((item = gridRisultati->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    const QVector<std::shared_ptr<Agenda>> risultati = ArchivioImpegni::instance().cerca(titolo, data, tipo, priorita);

    int row = 0;
    int col = 0;

    for (const auto &elemento : risultati) {
        QWidget *cardWidget = creaCardAttivita(elemento);
        gridRisultati->addWidget(cardWidget, row, col);

        col++;
        if (col > 1) {
            col = 0;
            row++;
        }
    }

    if (risultati.isEmpty()) {
        QLabel *noResult = new QLabel("Nessun impegno trovato.");
        noResult->setAlignment(Qt::AlignCenter);
        noResult->setStyleSheet("color: #e74c3c; font-size: 16px; padding: 50px;");
        gridRisultati->addWidget(noResult, 0, 0, 1, 2);
    }
}

QWidget *Research::creaCardAttivita(const std::shared_ptr<Agenda> &elemento)
{
    QWidget *container = new QWidget();
    QVBoxLayout *mainVLayout = new QVBoxLayout(container);
    mainVLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *card = new QFrame();
    card->setStyleSheet("QFrame { border: 2px solid #7D7D7D; border-radius: 15px; background-color: #F2F2F2; padding: 12px; }");

    QHBoxLayout *cardHLayout = new QHBoxLayout(card);
    QVBoxLayout *leftLayout = new QVBoxLayout();

    QLabel *lblTipo = new QLabel(elemento->getTipo().toUpper());
    lblTipo->setStyleSheet("color: #245C24; font-weight: bold; font-size: 11px; border: none;");
    leftLayout->addWidget(lblTipo);

    QLabel *lblTitolo = new QLabel("<b>" + elemento->getTitolo() + "</b>");
    lblTitolo->setStyleSheet("font-size: 18px; color: #000000; border: none;");
    leftLayout->addWidget(lblTitolo);

    cardHLayout->addLayout(leftLayout, 3);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

    // DETERMINAZIONE COLORE PRIORITÀ
    QString colorePriorita = "#000000"; // Default Nero
    QString testoPriorita = "N/D";

    if (!elemento->usaRigaFestivita()) {
        Agenda::Priorita p = elemento->getPriorita();
        testoPriorita = elemento->prioritaToString().toUpper();

        if (p == Agenda::Alta) {
            colorePriorita = "#e74c3c"; // Rosso
        } else if (p == Agenda::Media) {
            colorePriorita = "#f1c40f"; // Giallo (scuro per leggibilità)
        } else if (p == Agenda::Bassa) {
            colorePriorita = "#27ae60"; // Verde
        }
    }

    QLabel *lblPrio = new QLabel("PRIORITÀ: " + testoPriorita);
    lblPrio->setStyleSheet(QString("font-weight: bold; font-size: 11px; border: none; color: %1;").arg(colorePriorita));
    lblPrio->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(lblPrio);

    QLabel *lblTempo = new QLabel(QString("%1\n%2 (%3h)")
                                      .arg(elemento->getData().toString("yyyy-MM-dd"),
                                           elemento->getOra().toString("HH:mm"),
                                           QString::number(elemento->getDurataOre())));
    lblTempo->setStyleSheet("font-size: 11px; color: #000000; border: none;");
    lblTempo->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(lblTempo);

    rightLayout->addStretch();

    QPushButton *btnVisualizza = new QPushButton("VISUALIZZA");
    btnVisualizza->setFixedWidth(100);
    btnVisualizza->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; border-radius: 8px; padding: 5px; font-weight: bold; font-size: 10px; border: none; } "
        "QPushButton:hover { background-color: #2980b9; }"
        );
    rightLayout->addWidget(btnVisualizza);

    connect(btnVisualizza, &QPushButton::clicked, this, [this, elemento]() {
        emit richiestaVisualize(elemento);
    });

    cardHLayout->addLayout(rightLayout, 1);
    mainVLayout->addWidget(card);
    return container;
}