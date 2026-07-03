#include "appuntamento.h"
#include "agendavisitor.h"

Appuntamento::Appuntamento()
    : Evento(), modalita(Presenza), confermato(false)
{
}

Appuntamento::Appuntamento(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
                           const QString &luogo, const QString &organizzatore, const QStringList &partecipanti,
                           Modalita modalita, const QString &linkOnline, bool confermato)
    : Evento(titolo, descrizione, data, ora, durataOre, luogo, organizzatore, 0, 0.0, false),
      partecipanti(partecipanti),
      modalita(modalita),
      linkOnline(linkOnline),
      confermato(confermato)
{
}

QString Appuntamento::getTipo() const { return "Appuntamento"; }

QString Appuntamento::riepilogo() const
{
    return QString("%1 con %2 partecipanti%3").arg(modalitaToString()).arg(partecipanti.size()).arg(confermato ? " - confermato" : "");
}

QColor Appuntamento::coloreCalendario() const
{
    return confermato ? QColor::fromRgb(215, 189, 226) : QColor::fromRgb(232, 218, 239);
}

// modifiche seconda consegna
void Appuntamento::accept(AgendaVisitor &visitor) const
{
    visitor.visit(*this);
}

QStringList Appuntamento::getPartecipanti() const { return partecipanti; }
Appuntamento::Modalita Appuntamento::getModalita() const { return modalita; }
QString Appuntamento::getLinkOnline() const { return linkOnline; }
bool Appuntamento::isConfermato() const { return confermato; }

void Appuntamento::setPartecipanti(const QStringList &nuoviPartecipanti) { partecipanti = nuoviPartecipanti; }
void Appuntamento::setModalita(Modalita nuovaModalita) { modalita = nuovaModalita; }
void Appuntamento::setLinkOnline(const QString &nuovoLink) { linkOnline = nuovoLink; }
void Appuntamento::setConfermato(bool nuovoConfermato) { confermato = nuovoConfermato; }

QString Appuntamento::modalitaToString() const
{
    if (modalita == Online)
        return "Online";
    if (modalita == Telefono)
        return "Telefono";
    return "Presenza";
}

Appuntamento::Modalita Appuntamento::modalitaFromString(const QString &testo)
{
    if (testo.compare("Online", Qt::CaseInsensitive) == 0)
        return Online;
    if (testo.compare("Telefono", Qt::CaseInsensitive) == 0)
        return Telefono;
    return Presenza;
}

void Appuntamento::toJson(QJsonObject &json) const
{
    Evento::toJson(json);
    json["tipo"] = getTipo();
    json["partecipanti"] = partecipanti.join(";");
    json["modalita"] = modalitaToString();
    json["linkOnline"] = linkOnline;
    json["confermato"] = confermato;
}

void Appuntamento::fromJson(const QJsonObject &json)
{
    Evento::fromJson(json);
    partecipanti = json["partecipanti"].toString().split(";", Qt::SkipEmptyParts);
    if (partecipanti.isEmpty() && json.contains("stato"))
        partecipanti = {json["stato"].toString()};
    modalita = modalitaFromString(json["modalita"].toString());
    linkOnline = json["linkOnline"].toString();
    confermato = json["confermato"].toVariant().toBool();
}
