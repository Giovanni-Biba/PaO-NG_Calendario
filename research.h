#ifndef RESEARCH_H
#define RESEARCH_H

#include <QWidget>
#include <QJsonArray>

class QListWidget;
class QVBoxLayout;

class Research : public QWidget
{
    Q_OBJECT

public:
    explicit Research(QWidget *parent = nullptr);

    // Mostra tutto il contenuto del JSON
    void visualizzaTutto();

private:
    QListWidget *listaRisultati;
    QJsonArray tutteLeAttivita;
    const QString pathFile = ":/datiAttivitaFestivita.json";

    void caricaDatiJson();
    QWidget* creaCardAttivita(const QString &tipo, const QString &titolo, const QString &descrizione);
};

#endif