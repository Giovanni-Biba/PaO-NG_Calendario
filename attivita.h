#ifndef ATTIVITA_H
#define ATTIVITA_H

#include "agenda.h"

class Attivita : public Agenda
{
private:
    bool completata;
    QDate dataScadenza;
    QString categoria;

public:
    Attivita();
    Attivita(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
             Agenda::Priorita priorita, bool completata, const QDate &dataScadenza, const QString &categoria);

    QString getTipo() const override;
    QString riepilogo() const override;
    QColor coloreCalendario() const override;
    // modifiche seconda consegna
    void accept(AgendaVisitor &visitor) const override;
    bool matchesFiltro(const QString &testo, const QDate &dataFiltro, const QString &tipo, const QString &priorita) const override;

    bool isCompletata() const;
    QDate getDataScadenza() const;
    QString getCategoria() const;

    void setCompletata(bool nuovaCompletata);
    void setDataScadenza(const QDate &nuovaScadenza);
    void setCategoria(const QString &nuovaCategoria);

    bool isScaduta() const;

    void toJson(QJsonObject &json) const override;
    void fromJson(const QJsonObject &json) override;
};

#endif
