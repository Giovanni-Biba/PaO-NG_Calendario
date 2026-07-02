#ifndef CONSEGNA_H
#define CONSEGNA_H

#include "agenda.h"

class Consegna : public Agenda
{
public:
    enum Formato { PDF, ZIP, Codice, Relazione, Altro };

private:
    QString materiaOProgetto;
    QString destinatario;
    Formato formato;
    QString piattaforma;
    bool consegnata;

public:
    Consegna();
    Consegna(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
             const QString &materiaOProgetto, const QString &destinatario, Formato formato,
             const QString &piattaforma, bool consegnata);

    QString getTipo() const override;
    QString riepilogo() const override;
    QColor coloreCalendario() const override;
    // modifiche seconda consegna
    void accept(AgendaVisitor &visitor) const override;

    QString getMateriaOProgetto() const;
    QString getDestinatario() const;
    Formato getFormato() const;
    QString getPiattaforma() const;
    bool isConsegnata() const;

    void setMateriaOProgetto(const QString &nuovaMateria);
    void setDestinatario(const QString &nuovoDestinatario);
    void setFormato(Formato nuovoFormato);
    void setPiattaforma(const QString &nuovaPiattaforma);
    void setConsegnata(bool nuovaConsegnata);

    QString formatoToString() const;
    static Formato formatoFromString(const QString &testo);
    bool isInRitardo() const;
    int giorniRimanenti() const;

    void toJson(QJsonObject &json) const override;
    void fromJson(const QJsonObject &json) override;
};

#endif
