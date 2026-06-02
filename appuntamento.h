#ifndef APPUNTAMENTO_H
#define APPUNTAMENTO_H

#include "evento.h"

#include <QStringList>

class Appuntamento : public Evento
{
public:
    enum Modalita { Presenza, Online, Telefono };

private:
    QStringList partecipanti;
    Modalita modalita;
    QString linkOnline;
    bool confermato;

public:
    Appuntamento();
    Appuntamento(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
                 const QString &luogo, const QString &organizzatore, const QStringList &partecipanti,
                 Modalita modalita, const QString &linkOnline, bool confermato);

    QString getTipo() const override;
    QString riepilogo() const override;
    QMap<QString, QString> campiSpecifici() const override;
    QColor coloreCalendario() const override;

    QStringList getPartecipanti() const;
    Modalita getModalita() const;
    QString getLinkOnline() const;
    bool isConfermato() const;

    void setPartecipanti(const QStringList &nuoviPartecipanti);
    void setModalita(Modalita nuovaModalita);
    void setLinkOnline(const QString &nuovoLink);
    void setConfermato(bool nuovoConfermato);

    QString modalitaToString() const;
    static Modalita modalitaFromString(const QString &testo);

    void toJson(QJsonObject &json) const override;
    void fromJson(const QJsonObject &json) override;
};

#endif
