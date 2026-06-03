#include "research.h"
#include "archivioimpegni.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

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
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 10px 0;");
    mainLayout->addWidget(titleLabel);

    listaRisultati = new QListWidget(this);
    listaRisultati->setStyleSheet("border: none; background: transparent;");
    listaRisultati->setSpacing(12);
    listaRisultati->setSelectionMode(QAbstractItemView::NoSelection);
    mainLayout->addWidget(listaRisultati);

    connect(buttonIndietro, &QPushButton::clicked, this, &Research::ritornaHomeSlot);
}

void Research::eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita)
{
    listaRisultati->clear();

    const QVector<std::shared_ptr<Agenda>> risultati = ArchivioImpegni::instance().cerca(titolo, data, tipo, priorita);

    for (const auto &elemento : risultati) {
        QWidget *cardWidget = creaCardAttivita(elemento);
        QListWidgetItem *item = new QListWidgetItem(listaRisultati);
        item->setSizeHint(cardWidget->sizeHint());
        listaRisultati->addItem(item);
        listaRisultati->setItemWidget(item, cardWidget);
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

QWidget *Research::creaCardAttivita(const std::shared_ptr<Agenda> &elemento)
{
    QWidget *container = new QWidget();
    QVBoxLayout *mainVLayout = new QVBoxLayout(container);
    mainVLayout->setContentsMargins(5, 2, 5, 2);

    QFrame *card = new QFrame();
    card->setStyleSheet("QFrame { border: 2px solid #3498db; border-radius: 15px; background-color: white; padding: 12px; }");

    QHBoxLayout *cardHLayout = new QHBoxLayout(card);
    QVBoxLayout *leftLayout = new QVBoxLayout();

    QLabel *lblTipo = new QLabel(elemento->getTipo().toUpper());
    lblTipo->setStyleSheet("color: #e67e22; font-weight: bold; font-size: 11px; border: none;");
    leftLayout->addWidget(lblTipo);

    QLabel *lblTitolo = new QLabel("<b>" + elemento->getTitolo() + "</b>");
    lblTitolo->setStyleSheet("font-size: 18px; color: #2c3e50; border: none;");
    leftLayout->addWidget(lblTitolo);

    QLabel *lblDesc = new QLabel(elemento->riepilogo());
    lblDesc->setStyleSheet("color: #34495e; font-style: italic; border: none;");
    lblDesc->setWordWrap(true);
    leftLayout->addWidget(lblDesc);

    cardHLayout->addLayout(leftLayout, 3);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

    const QString prioritaStr = elemento->usaRigaFestivita() ? "N/D" : elemento->prioritaToString();

    QLabel *lblPrio = new QLabel("PRIORITA: " + prioritaStr.toUpper());
    lblPrio->setStyleSheet("font-weight: bold; font-size: 11px; border: none; color: #2c3e50;");
    lblPrio->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(lblPrio);

    QLabel *lblTempo = new QLabel(QString("%1\n%2 (%3h)")
                                      .arg(elemento->getData().toString("yyyy-MM-dd"),
                                           elemento->getOra().toString("HH:mm"),
                                           QString::number(elemento->getDurataOre())));
    lblTempo->setStyleSheet("font-size: 11px; color: #7f8c8d; border: none;");
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
