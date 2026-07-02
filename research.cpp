#include "research.h"
#include "agendavisitor.h"
#include "archivioimpegni.h"
#include "appuntamento.h"
#include "attivita.h"
#include "consegna.h"
#include "evento.h"
#include "festivita.h"
#include "searchbar.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

namespace {
const QString logoIconPath = ":/IMG/logo.png/IMG/logo.png";

// modifiche seconda consegna: visitor per scegliere l'immagine della card senza usare getTipo per il flusso.
class IconaRicercaVisitor : public AgendaVisitor
{
public:
    void visit(const Attivita &) override { path = ":/IMG/logo.png/IMG/Attivita.png"; }
    void visit(const Evento &) override { path = ":/IMG/logo.png/IMG/evento.png"; }
    void visit(const Appuntamento &) override { path = ":/IMG/logo.png/IMG/Appuntamento.png"; }
    void visit(const Consegna &) override { path = ":/IMG/logo.png/IMG/Consegna.png"; }
    void visit(const Festivita &) override { path = ":/IMG/logo.png/IMG/festivita.png"; }

    QString immagine() const { return path.isEmpty() ? logoIconPath : path; }

private:
    QString path;
};
}

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

    // modifiche seconda consegna: barra filtri nella pagina risultati per ricerca in tempo reale.
    barraRicerca = new SearchBar(this);
    mainLayout->addWidget(barraRicerca);

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
    auto aggiornaRisultati = [this]() {
        eseguiRicercaFiltrata(
            barraRicerca->getTestoTitolo(),
            barraRicerca->getValoreData(),
            barraRicerca->getTestoTipo(),
            barraRicerca->getTestoPriorita()
        );
    };
    connect(barraRicerca, &SearchBar::filtriCambiati, this, aggiornaRisultati);
    connect(barraRicerca, &SearchBar::cercaClicked, this, aggiornaRisultati);
}

void Research::impostaFiltri(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita)
{
    barraRicerca->impostaFiltri(titolo, data, tipo, priorita);
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
    // modifiche seconda consegna: immagine specifica per tipo scelta tramite visitor.
    IconaRicercaVisitor visitorIcona;
    elemento->accept(visitorIcona);

    QLabel *iconaTipo = new QLabel(card);
    iconaTipo->setPixmap(QPixmap(visitorIcona.immagine()).scaled(34, 34, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconaTipo->setStyleSheet("border: none; padding: 0;");
    cardHLayout->addWidget(iconaTipo);

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
    QString colorePriorita = "#000000";
    QString testoPriorita = "N/D";

    if (!elemento->usaRigaFestivita()) {
        Agenda::Priorita p = elemento->getPriorita();
        testoPriorita = elemento->prioritaToString().toUpper();

        if (p == Agenda::Alta) {
            colorePriorita = "#e74c3c";
        } else if (p == Agenda::Media) {
            colorePriorita = "#f1c40f";
        } else if (p == Agenda::Bassa) {
            colorePriorita = "#27ae60";
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
