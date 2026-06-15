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
    void eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita);

signals:
    void ritornaHome();
    void richiestaVisualize(std::shared_ptr<Agenda> elemento);

private:
    QWidget* creaCardAttivita(const std::shared_ptr<Agenda> &elemento);
    QWidget *containerRisultati;
    QGridLayout *gridRisultati;

    QPushButton *buttonIndietro;

private slots:
    void ritornaHomeSlot() { emit ritornaHome(); }
};

#endif