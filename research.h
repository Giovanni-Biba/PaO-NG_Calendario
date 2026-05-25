#ifndef RESEARCH_H
#define RESEARCH_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>

class Research : public QWidget
{
    Q_OBJECT
public:
    explicit Research(QWidget *parent = nullptr);
    void eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita);

signals:
    void ritornaHome();
    void richiestaVisualize(const QJsonObject &elemento);

private:
    void caricaDatiJson();
    void caricaDatiXml();

    QWidget* creaCardAttivita(const QJsonObject &elemento, const QString &tipo, const QString &titolo, const QString &descrizione,const QString &priorita, const QString &ora, const QString &data, int durata);

    QListWidget *listaRisultati;
    QPushButton *buttonIndietro;
    QJsonArray tutteLeAttivita;
    QString pathFilejson = "datiAttivitaFestivita.json";
    QString pathFilexml = "datiEventoAppuntamento.xml";
};

#endif
