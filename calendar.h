#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QDate>
#include <QHBoxLayout>

#include "searchbar.h"

class calendar : public QWidget
{
    Q_OBJECT

public:
    explicit calendar(QWidget *parent = nullptr);

    SearchBar* getSearchBar() const { return BarraRicerca; }

private:
    QPushButton *Titolo;
    SearchBar *BarraRicerca;
    QPushButton *Crea;

    QGridLayout *grid;

    QHBoxLayout *celle[26][7];
    int conteggioCelle[26][7];

    QDate oggi;
    QDate lunediSettimana;

    void aggiungiEvento(const QString& titolo, const QDate& data, int oraInizio, int durataOre);
    void aggiungiFestivita(const QString& titolo, const QDate& data);
    void caricaJson();
    void caricaXml();

private slots:
    void closeCalendar();

signals:
    void richiestaCrea();
    void richiestaCerca();
    void richiestaVisualize(QString titolo);
};

#endif