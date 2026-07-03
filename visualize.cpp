#include "visualize.h"
#include "agendavisitor.h"
#include "archivioimpegni.h"
#include "appuntamento.h"
#include "attivita.h"
#include "consegna.h"
#include "evento.h"
#include "festivita.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

// modifiche seconda consegna: visitor che costruisce widget diversi in base al tipo
class DettaglioAgendaVisitor : public AgendaVisitor
{
public:
    explicit DettaglioAgendaVisitor(QVBoxLayout *layout)
        : dettagliLayout(layout)
    {
    }

    void visit(const Attivita &attivita) override
    {
        aggiungiSezione("Dettagli attivita");
        aggiungiIndicatore("Completata", attivita.isCompletata(), "Completata", "Da completare");
        aggiungiRiga("Scadenza:", attivita.getDataScadenza().toString("dd/MM/yyyy"));
        aggiungiRiga("Categoria:", attivita.getCategoria());

        if (attivita.isScaduta())
            aggiungiBadge("Stato", "Scaduta", "#c0392b", "#fdecea");
    }

    void visit(const Evento &evento) override
    {
        aggiungiDettagliEvento(evento);
    }

    void visit(const Appuntamento &appuntamento) override
    {
        aggiungiDettagliEvento(appuntamento);
        aggiungiSezione("Dettagli appuntamento");
        aggiungiLista("Partecipanti", appuntamento.getPartecipanti());
        aggiungiRiga("Modalita:", appuntamento.modalitaToString());

        if (!appuntamento.getLinkOnline().isEmpty())
            aggiungiLink("Link:", appuntamento.getLinkOnline());
        else
            aggiungiRiga("Link:", "-");

        aggiungiIndicatore("Confermato", appuntamento.isConfermato(), "Confermato", "Da confermare");
    }

    void visit(const Consegna &consegna) override
    {
        aggiungiSezione("Dettagli consegna");
        aggiungiRiga("Materia o progetto:", consegna.getMateriaOProgetto());
        aggiungiRiga("Destinatario:", consegna.getDestinatario());
        aggiungiRiga("Formato:", consegna.formatoToString());
        aggiungiRiga("Piattaforma:", consegna.getPiattaforma());
        aggiungiIndicatore("Consegnata", consegna.isConsegnata(), "Consegnata", "Non consegnata");

        if (!consegna.isConsegnata() && consegna.isInRitardo())
            aggiungiBadge("Scadenza", "In ritardo", "#c0392b", "#fdecea");
        else
            aggiungiRiga("Giorni rimanenti:", QString::number(consegna.giorniRimanenti()));
    }

    void visit(const Festivita &festivita) override
    {
        aggiungiSezione("Dettagli festivita");
        aggiungiRiga("Stato:", festivita.statoToString());
        aggiungiIndicatore("Ricorrenza annuale", festivita.isRicorrenzaAnnuale(), "Annuale", "Singola");
        aggiungiIndicatore("Giorno non lavorativo", festivita.isGiornoNonLavorativo(), "Non lavorativo", "Lavorativo");
        aggiungiRiga("Nome ufficiale:", festivita.getNomeUfficiale());
    }

private:
    QVBoxLayout *dettagliLayout;

    void aggiungiDettagliEvento(const Evento &evento)
    {
        aggiungiSezione("Dettagli evento");
        aggiungiRiga("Luogo:", evento.getLuogo());
        aggiungiRiga("Organizzatore:", evento.getOrganizzatore());
        aggiungiRiga("Capienza:", QString::number(evento.getCapienzaMassima()));
        aggiungiRiga("Costo:", QString("%1 euro").arg(QString::number(evento.getCostoBiglietto(), 'f', 2)));
        aggiungiIndicatore("Visibilita", evento.isPubblico(), "Pubblico", "Privato");
    }

    void aggiungiSezione(const QString &testo)
    {
        QLabel *label = new QLabel(testo.toUpper());
        label->setStyleSheet("font-size: 13px; font-weight: bold; color: #274C69; margin-top: 12px; border: none;");
        dettagliLayout->addWidget(label);
    }

    void aggiungiRiga(const QString &nome, const QString &valore)
    {
        QFrame *rigaFrame = new QFrame();
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
        dettagliLayout->addWidget(rigaFrame);
    }

    void aggiungiIndicatore(const QString &nome, bool attivo, const QString &testoSi, const QString &testoNo)
    {
        aggiungiBadge(nome + ":", attivo ? testoSi : testoNo, attivo ? "#1e8449" : "#7f8c8d", attivo ? "#eafaf1" : "#f4f6f6");
    }

    void aggiungiBadge(const QString &nome, const QString &testo, const QString &coloreTesto, const QString &sfondo)
    {
        QFrame *rigaFrame = new QFrame();
        rigaFrame->setStyleSheet("border: none; background: transparent;");

        QHBoxLayout *layout = new QHBoxLayout(rigaFrame);
        layout->setContentsMargins(0, 5, 0, 5);

        QLabel *labelNome = new QLabel(nome, rigaFrame);
        labelNome->setMinimumWidth(150);
        labelNome->setMaximumWidth(150);
        labelNome->setStyleSheet("font-weight: bold; color: #46484A; font-size: 14px; border: none;");
        labelNome->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QLabel *badge = new QLabel(testo, rigaFrame);
        badge->setStyleSheet(QString("color: %1; background-color: %2; border: 1px solid %1; border-radius: 8px; padding: 5px 10px; font-weight: bold;").arg(coloreTesto, sfondo));
        badge->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

        layout->addWidget(labelNome);
        layout->addWidget(badge);
        layout->addStretch();
        dettagliLayout->addWidget(rigaFrame);
    }

    void aggiungiLista(const QString &nome, const QStringList &valori)
    {
        QFrame *rigaFrame = new QFrame();
        rigaFrame->setStyleSheet("border: none; background: transparent;");

        QHBoxLayout *layout = new QHBoxLayout(rigaFrame);
        layout->setContentsMargins(0, 5, 0, 5);

        QLabel *labelNome = new QLabel(nome + ":", rigaFrame);
        labelNome->setMinimumWidth(150);
        labelNome->setMaximumWidth(150);
        labelNome->setStyleSheet("font-weight: bold; color: #46484A; font-size: 14px; border: none;");
        labelNome->setAlignment(Qt::AlignRight | Qt::AlignTop);

        QVBoxLayout *listaLayout = new QVBoxLayout();
        listaLayout->setSpacing(4);

        if (valori.isEmpty()) {
            QLabel *vuoto = new QLabel("-", rigaFrame);
            vuoto->setStyleSheet("color: #4B88BF; font-size: 15px; border: none; padding-left: 10px;");
            listaLayout->addWidget(vuoto);
        } else {
            for (const QString &valore : valori) {
                QLabel *partecipante = new QLabel("• " + valore, rigaFrame);
                partecipante->setStyleSheet("color: #4B88BF; font-size: 15px; border: none; padding-left: 10px;");
                listaLayout->addWidget(partecipante);
            }
        }

        layout->addWidget(labelNome);
        layout->addLayout(listaLayout, 1);
        dettagliLayout->addWidget(rigaFrame);
    }

    void aggiungiLink(const QString &nome, const QString &url)
    {
        QFrame *rigaFrame = new QFrame();
        rigaFrame->setStyleSheet("border: none; background: transparent;");

        QHBoxLayout *layout = new QHBoxLayout(rigaFrame);
        layout->setContentsMargins(0, 5, 0, 5);

        QLabel *labelNome = new QLabel(nome, rigaFrame);
        labelNome->setMinimumWidth(150);
        labelNome->setMaximumWidth(150);
        labelNome->setStyleSheet("font-weight: bold; color: #46484A; font-size: 14px; border: none;");
        labelNome->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QString link = url;
        if (!link.startsWith("http://") && !link.startsWith("https://"))
            link.prepend("https://");

        QLabel *labelLink = new QLabel(QString("<a href=\"%1\">%2</a>").arg(link.toHtmlEscaped(), url.toHtmlEscaped()), rigaFrame);
        labelLink->setOpenExternalLinks(true);
        labelLink->setWordWrap(true);
        labelLink->setStyleSheet("color: #1f618d; font-size: 15px; border: none; padding-left: 10px;");

        layout->addWidget(labelNome);
        layout->addWidget(labelLink, 1);
        dettagliLayout->addWidget(rigaFrame);
    }
};

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

    titoloPagina->setText(elementoCorrente->getTitolo().toUpper());

    // Elenco dettagli
    aggiungiRiga("Tipo:", elementoCorrente->getTipo());
    aggiungiRiga("Titolo:", elementoCorrente->getTitolo());
    aggiungiRiga("Descrizione:", elementoCorrente->getDescrizione());
    aggiungiRiga("Data:", elementoCorrente->getData().toString("dd MMMM yyyy"));

    // modifiche seconda consegna: evita confronti testuali
    if (!elementoCorrente->usaRigaFestivita()) {
        aggiungiRiga("Ora:", elementoCorrente->getOra().toString("HH:mm"));
        aggiungiRiga("Durata:", QString::number(elementoCorrente->getDurataOre()) + " ore");
    }

    if (!elementoCorrente->usaRigaFestivita())
        aggiungiRiga("Priorità:", elementoCorrente->prioritaToString());

    // modifiche seconda consegna: i dettagli specifici sono costruiti tramite visitor.
    DettaglioAgendaVisitor visitor(dettagliLayout);
    elementoCorrente->accept(visitor);

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
