#include "agenda.h"

Agenda::Agenda()
    : durataOre(1), priorita(Media)
{
}

Agenda::Agenda(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre, Priorita priorita)
    : titolo(titolo), descrizione(descrizione), data(data), ora(ora), durataOre(durataOre > 0 ? durataOre : 1), priorita(priorita)
{
}

Agenda::~Agenda() {}

QString Agenda::getTitolo() const { return titolo; }
QString Agenda::getDescrizione() const { return descrizione; }
QDate Agenda::getData() const { return data; }
QTime Agenda::getOra() const { return ora; }
int Agenda::getDurataOre() const { return durataOre; }
Agenda::Priorita Agenda::getPriorita() const { return priorita; }

void Agenda::setTitolo(const QString &nuovoTitolo) { titolo = nuovoTitolo; }
void Agenda::setDescrizione(const QString &nuovaDescrizione) { descrizione = nuovaDescrizione; }
void Agenda::setData(const QDate &nuovaData) { data = nuovaData; }
void Agenda::setOra(const QTime &nuovaOra) { ora = nuovaOra; }
void Agenda::setDurataOre(int nuovaDurata) { durataOre = nuovaDurata > 0 ? nuovaDurata : 1; }
void Agenda::setPriorita(Priorita nuovaPriorita) { priorita = nuovaPriorita; }

bool Agenda::usaRigaFestivita() const
{
    return false;
}

bool Agenda::matchesFiltro(const QString &testo, const QDate &dataFiltro, const QString &tipo, const QString &priorita) const
{
    const QString testoPulito = testo.trimmed().toLower();
    const bool matchTestoOData = !testoPulito.isEmpty() ? titolo.toLower().contains(testoPulito) : data == dataFiltro;
    const bool matchTipo = tipo == "Tutte" || getTipo().compare(tipo, Qt::CaseInsensitive) == 0;
    const bool matchPriorita = priorita == "Tutte" || (!usaRigaFestivita() && prioritaToString().compare(priorita, Qt::CaseInsensitive) == 0);

    return matchTestoOData && matchTipo && matchPriorita;
}

void Agenda::toJson(QJsonObject &json) const
{
    json["tipo"] = getTipo();
    json["titolo"] = titolo;
    json["descrizione"] = descrizione;
    json["data"] = data.toString(Qt::ISODate);
    json["ora"] = ora.toString("HH:mm");
    json["durata_ore"] = durataOre;
    if (!usaRigaFestivita())
        json["priorita"] = prioritaToString();
}

void Agenda::fromJson(const QJsonObject &json)
{
    titolo = json["titolo"].toString();
    descrizione = json["descrizione"].toString();
    data = QDate::fromString(json["data"].toString(), Qt::ISODate);
    ora = QTime::fromString(json["ora"].toString(), "HH:mm");
    if (!ora.isValid())
        ora = QTime::fromString(json["ora"].toString(), "HH:mm:ss");
    durataOre = json.contains("durata_ore") ? json["durata_ore"].toVariant().toInt() : json["durata"].toVariant().toInt();
    if (durataOre <= 0)
        durataOre = 1;
    priorita = prioritaFromString(json["priorita"].toString());
}

bool Agenda::stessaChiave(const Agenda &altro) const
{
    return getTipo().compare(altro.getTipo(), Qt::CaseInsensitive) == 0 &&
           titolo == altro.titolo &&
           data == altro.data &&
           ora == altro.ora;
}

QString Agenda::prioritaToString() const
{
    if (priorita == Alta)
        return "Alta";
    if (priorita == Bassa)
        return "Bassa";
    return "Media";
}

Agenda::Priorita Agenda::prioritaFromString(const QString &testo)
{
    if (testo.compare("Alta", Qt::CaseInsensitive) == 0)
        return Alta;
    if (testo.compare("Bassa", Qt::CaseInsensitive) == 0)
        return Bassa;
    return Media;
}
