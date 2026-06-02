#ifndef ATTIVITA_H
#define ATTIVITA_H

#include "agenda.h"

class Attivita : public Agenda
{
public:
    enum Priorita { Bassa, Media, Alta };

private:
    Priorita priorita;
    bool completata;
    QDate dataScadenza;
    QString categoria;

public:
    Attivita();
    Attivita(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
             Priorita priorita, bool completata, const QDate &dataScadenza, const QString &categoria);

    QString getTipo() const override;
    QString riepilogo() const override;
    QMap<QString, QString> campiSpecifici() const override;
    QColor coloreCalendario() const override;
    bool matchesFiltro(const QString &testo, const QDate &dataFiltro, const QString &tipo, const QString &priorita) const override;

    Priorita getPriorita() const;
    bool isCompletata() const;
    QDate getDataScadenza() const;
    QString getCategoria() const;

    void setPriorita(Priorita nuovaPriorita);
    void setCompletata(bool nuovaCompletata);
    void setDataScadenza(const QDate &nuovaScadenza);
    void setCategoria(const QString &nuovaCategoria);

    QString prioritaToString() const;
    static Priorita prioritaFromString(const QString &testo);
    bool isScaduta() const;

    void toJson(QJsonObject &json) const override;
    void fromJson(const QJsonObject &json) override;
};

#endif
