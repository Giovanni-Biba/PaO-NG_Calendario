#ifndef RESEARCH_H
#define RESEARCH_H

#include <QWidget>
#include <QJsonArray>
#include <QPushButton>

class QListWidget;
class QVBoxLayout;

class Research : public QWidget
{
    Q_OBJECT

public:
    explicit Research(QWidget *parent = nullptr);
    void eseguiRicercaFiltrata(const QString &titolo, const QDate &data, const QTime &ora, const QString &priorita);

    // Mostra tutto il contenuto del JSON
    void visualizzaTutto();

private:
    QPushButton *buttonIndietro;
    QListWidget *listaRisultati;
    QJsonArray tutteLeAttivita;
    const QString pathFile = ":/datiAttivitaFestivita.json";

    void caricaDatiJson();
private:
    QWidget* creaCardAttivita(const QString &tipo, const QString &titolo, const QString &descrizione, const QString &priorita, const QString &ora);

signals:
    void ritornaHome();
};

#endif