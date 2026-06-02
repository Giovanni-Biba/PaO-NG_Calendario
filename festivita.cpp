#include "festivita.h"

Festivita::Festivita()
    : Agenda(), stato(Worldwide), ricorrenzaAnnuale(true), giornoNonLavorativo(false)
{
}

Festivita::Festivita(const QString &titolo, const QString &descrizione, const QDate &data, Stato stato,
                     bool ricorrenzaAnnuale, bool giornoNonLavorativo, const QString &nomeUfficiale)
    : Agenda(titolo, descrizione, data, QTime(0, 0), 1),
      stato(stato),
      ricorrenzaAnnuale(ricorrenzaAnnuale),
      giornoNonLavorativo(giornoNonLavorativo),
      nomeUfficiale(nomeUfficiale)
{
}

QString Festivita::getTipo() const { return "Festivita"; }

QString Festivita::riepilogo() const
{
    return QString("%1 - %2").arg(statoToString(), QString(giornoNonLavorativo ? "giorno non lavorativo" : "ricorrenza"));
}

QMap<QString, QString> Festivita::campiSpecifici() const
{
    return {
        {"Stato", statoToString()},
        {"Ricorrenza annuale", ricorrenzaAnnuale ? "Si" : "No"},
        {"Giorno non lavorativo", giornoNonLavorativo ? "Si" : "No"},
        {"Nome ufficiale", nomeUfficiale}
    };
}

QColor Festivita::coloreCalendario() const
{
    return giornoNonLavorativo ? QColor("#FADBD8") : QColor("#FCF3CF");
}

bool Festivita::usaRigaFestivita() const
{
    return true;
}

Festivita::Stato Festivita::getStato() const { return stato; }
bool Festivita::isRicorrenzaAnnuale() const { return ricorrenzaAnnuale; }
bool Festivita::isGiornoNonLavorativo() const { return giornoNonLavorativo; }
QString Festivita::getNomeUfficiale() const { return nomeUfficiale; }

void Festivita::setStato(Stato nuovoStato) { stato = nuovoStato; }
void Festivita::setRicorrenzaAnnuale(bool nuovaRicorrenza) { ricorrenzaAnnuale = nuovaRicorrenza; }
void Festivita::setGiornoNonLavorativo(bool nuovoValore) { giornoNonLavorativo = nuovoValore; }
void Festivita::setNomeUfficiale(const QString &nuovoNome) { nomeUfficiale = nuovoNome; }

QString Festivita::statoToString() const
{
    if (stato == Italia)
        return "Italia";
    if (stato == USA)
        return "USA";
    if (stato == Cina)
        return "Cina";
    return "Worldwide";
}

Festivita::Stato Festivita::statoFromString(const QString &testo)
{
    if (testo.compare("Italia", Qt::CaseInsensitive) == 0)
        return Italia;
    if (testo.compare("USA", Qt::CaseInsensitive) == 0 || testo.compare("America", Qt::CaseInsensitive) == 0)
        return USA;
    if (testo.compare("Cina", Qt::CaseInsensitive) == 0)
        return Cina;
    return Worldwide;
}

void Festivita::toJson(QJsonObject &json) const
{
    Agenda::toJson(json);
    json["stato"] = statoToString();
    json["ricorrenzaAnnuale"] = ricorrenzaAnnuale;
    json["giornoNonLavorativo"] = giornoNonLavorativo;
    json["nomeUfficiale"] = nomeUfficiale;
}

void Festivita::fromJson(const QJsonObject &json)
{
    Agenda::fromJson(json);
    stato = statoFromString(json["stato"].toString());
    ricorrenzaAnnuale = json["ricorrenzaAnnuale"].toBool(true);
    giornoNonLavorativo = json["giornoNonLavorativo"].toBool(false);
    nomeUfficiale = json["nomeUfficiale"].toString(titolo);
}
