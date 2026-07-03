#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QDate>
#include <QHBoxLayout>

#include "agenda.h"
#include "searchbar.h"

class calendar : public QWidget
{
    Q_OBJECT

public:
    explicit calendar(QWidget *parent = nullptr);

    SearchBar* getSearchBar() const { return BarraRicerca; }
    void aggiornaCalendario();
    // modifiche seconda consegna: metodi pubblici richiamabili dalle scorciatoie
    void mostraSettimanaPrecedente();
    void mostraSettimanaSuccessiva();
    void aggiornaHome();

private:
    QPushButton *Titolo;
    SearchBar *BarraRicerca;
    QPushButton *Crea;
    QPushButton *settimanaPrecedente;
    QPushButton *settimanaSuccessiva;
    QLabel *labelSettimana;

    QGridLayout *grid;

    QLabel *headerGiorni[7];
    QHBoxLayout *celle[26][7];
    int conteggioCelle[26][7];
    int indiceColoreElemento;

    QDate oggi;
    QDate lunediSettimana;

    QString prossimoColoreElemento();
    void aggiornaIntestazioneSettimana();
    void aggiungiImpegno(const std::shared_ptr<Agenda> &impegno);
    void caricaImpegni();

private slots:
    void closeCalendar();
    void refreshGenerale();
    void vaiSettimanaPrecedente();
    void vaiSettimanaSuccessiva();

signals:
    void richiestaCrea();
    void richiestaCerca();
    void richiestaVisualize(std::shared_ptr<Agenda> impegno);
};

#endif
