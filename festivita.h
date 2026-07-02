#ifndef FESTIVITA_H
#define FESTIVITA_H

#include "agenda.h"

class Festivita : public Agenda
{
public:
    enum Stato { Italia, USA, Cina, Worldwide };

private:
    Stato stato;
    bool ricorrenzaAnnuale;
    bool giornoNonLavorativo;
    QString nomeUfficiale;

public:
    Festivita();
    Festivita(const QString &titolo, const QString &descrizione, const QDate &data, Stato stato,
              bool ricorrenzaAnnuale, bool giornoNonLavorativo, const QString &nomeUfficiale);

    QString getTipo() const override;
    QString riepilogo() const override;
    QColor coloreCalendario() const override;
    // modifiche seconda consegna
    void accept(AgendaVisitor &visitor) const override;
    bool usaRigaFestivita() const override;

    Stato getStato() const;
    bool isRicorrenzaAnnuale() const;
    bool isGiornoNonLavorativo() const;
    QString getNomeUfficiale() const;

    void setStato(Stato nuovoStato);
    void setRicorrenzaAnnuale(bool nuovaRicorrenza);
    void setGiornoNonLavorativo(bool nuovoValore);
    void setNomeUfficiale(const QString &nuovoNome);

    QString statoToString() const;
    static Stato statoFromString(const QString &testo);

    void toJson(QJsonObject &json) const override;
    void fromJson(const QJsonObject &json) override;
};

#endif
