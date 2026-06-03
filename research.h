#ifndef RESEARCH_H
#define RESEARCH_H

#include <QWidget>
#include <QPushButton>
#include <QDate>
#include <QGridLayout>
#include <memory>

#include "agenda.h"

class Research : public QWidget
{
    Q_OBJECT
public:
    explicit Research(QWidget *parent = nullptr);

    // Esegue la ricerca e popola la griglia a 2 colonne
    void eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita);

signals:
    void ritornaHome();
    void richiestaVisualize(std::shared_ptr<Agenda> elemento);

private:
    // Crea il widget della singola card
    QWidget* creaCardAttivita(const std::shared_ptr<Agenda> &elemento);

    // Membri per la gestione del layout a griglia (2 colonne)
    QWidget *containerRisultati;
    QGridLayout *gridRisultati;

    QPushButton *buttonIndietro;

private slots:
    void ritornaHomeSlot() { emit ritornaHome(); }
};

#endif // RESEARCH_H