#include "attivita.h"

Attivita::Attivita()
    : Agenda(), completata(false)
{
}

Attivita::Attivita(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
                   Agenda::Priorita priorita, bool completata, const QDate &dataScadenza, const QString &categoria)
    : Agenda(titolo, descrizione, data, ora, durataOre, priorita),
      completata(completata),
      dataScadenza(dataScadenza),
      categoria(categoria)
{
}

QString Attivita::getTipo() const { return "Attivita"; }

QString Attivita::riepilogo() const
{
    return QString("%1 - priorita %2%3").arg(categoria, prioritaToString(), QString(completata ? " - completata" : ""));
}

QMap<QString, QString> Attivita::campiSpecifici() const
{
    return {
        {"Completata", completata ? "Si" : "No"},
        {"Scadenza", dataScadenza.toString("dd/MM/yyyy")},
        {"Categoria", categoria}
    };
}

QColor Attivita::coloreCalendario() const
{
    if (completata)
        return QColor("#D5DBDB");
    if (getPriorita() == Alta)
        return QColor("#F5B7B1");
    if (getPriorita() == Media)
        return QColor("#F9E79F");
    return QColor("#ABEBC6");
}

bool Attivita::matchesFiltro(const QString &testo, const QDate &dataFiltro, const QString &tipo, const QString &prioritaFiltro) const
{
    return Agenda::matchesFiltro(testo, dataFiltro, tipo, prioritaFiltro);
}

bool Attivita::isCompletata() const { return completata; }
QDate Attivita::getDataScadenza() const { return dataScadenza; }
QString Attivita::getCategoria() const { return categoria; }

void Attivita::setCompletata(bool nuovaCompletata) { completata = nuovaCompletata; }
void Attivita::setDataScadenza(const QDate &nuovaScadenza) { dataScadenza = nuovaScadenza; }
void Attivita::setCategoria(const QString &nuovaCategoria) { categoria = nuovaCategoria; }

bool Attivita::isScaduta() const
{
    return !completata && dataScadenza.isValid() && dataScadenza < QDate::currentDate();
}

void Attivita::toJson(QJsonObject &json) const
{
    Agenda::toJson(json);
    json["completata"] = completata;
    json["dataScadenza"] = dataScadenza.toString(Qt::ISODate);
    json["categoria"] = categoria;
}

void Attivita::fromJson(const QJsonObject &json)
{
    Agenda::fromJson(json);
    completata = json["completata"].toBool(false);
    dataScadenza = QDate::fromString(json["dataScadenza"].toString(), Qt::ISODate);
    if (!dataScadenza.isValid())
        dataScadenza = QDate::fromString(json["dataFine"].toString(), Qt::ISODate);
    categoria = json["categoria"].toString("Generale");
}
