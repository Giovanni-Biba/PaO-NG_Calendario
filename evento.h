#ifndef EVENTO_H
#define EVENTO_H

#include "agenda.h"

class Evento : public Agenda
{
private:
    QString luogo;
    QString organizzatore;
    int capienzaMassima;
    double costoBiglietto;
    bool pubblico;

public:
    Evento();
    Evento(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
           const QString &luogo, const QString &organizzatore, int capienzaMassima, double costoBiglietto, bool pubblico);

    QString getTipo() const override;
    QString riepilogo() const override;
    QColor coloreCalendario() const override;
    // modifiche seconda consegna
    void accept(AgendaVisitor &visitor) const override;

    QString getLuogo() const;
    QString getOrganizzatore() const;
    int getCapienzaMassima() const;
    double getCostoBiglietto() const;
    bool isPubblico() const;

    void setLuogo(const QString &nuovoLuogo);
    void setOrganizzatore(const QString &nuovoOrganizzatore);
    void setCapienzaMassima(int nuovaCapienza);
    void setCostoBiglietto(double nuovoCosto);
    void setPubblico(bool nuovoPubblico);

    void toJson(QJsonObject &json) const override;
    void fromJson(const QJsonObject &json) override;
};

#endif
