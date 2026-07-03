#include "consegna.h"
#include "agendavisitor.h"

Consegna::Consegna()
    : Agenda(), formato(Altro), consegnata(false)
{
}

Consegna::Consegna(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
                   const QString &materiaOProgetto, const QString &destinatario, Formato formato,
                   const QString &piattaforma, bool consegnata)
    : Agenda(titolo, descrizione, data, ora, durataOre),
      materiaOProgetto(materiaOProgetto),
      destinatario(destinatario),
      formato(formato),
      piattaforma(piattaforma),
      consegnata(consegnata)
{
}

QString Consegna::getTipo() const { return "Consegna"; }

QString Consegna::riepilogo() const
{
    return QString("%1 per %2 su %3").arg(formatoToString(), materiaOProgetto, piattaforma);
}

QColor Consegna::coloreCalendario() const
{
    if (consegnata)
        return QColor::fromRgb(213, 219, 219);
    if (isInRitardo())
        return QColor::fromRgb(236, 112, 99);
    return QColor::fromRgb(248, 196, 113);
}

// modifiche seconda consegna: visitor per Consegna.
void Consegna::accept(AgendaVisitor &visitor) const
{
    visitor.visit(*this);
}

QString Consegna::getMateriaOProgetto() const { return materiaOProgetto; }
QString Consegna::getDestinatario() const { return destinatario; }
Consegna::Formato Consegna::getFormato() const { return formato; }
QString Consegna::getPiattaforma() const { return piattaforma; }
bool Consegna::isConsegnata() const { return consegnata; }

void Consegna::setMateriaOProgetto(const QString &nuovaMateria) { materiaOProgetto = nuovaMateria; }
void Consegna::setDestinatario(const QString &nuovoDestinatario) { destinatario = nuovoDestinatario; }
void Consegna::setFormato(Formato nuovoFormato) { formato = nuovoFormato; }
void Consegna::setPiattaforma(const QString &nuovaPiattaforma) { piattaforma = nuovaPiattaforma; }
void Consegna::setConsegnata(bool nuovaConsegnata) { consegnata = nuovaConsegnata; }

QString Consegna::formatoToString() const
{
    if (formato == PDF)
        return "PDF";
    if (formato == ZIP)
        return "ZIP";
    if (formato == Codice)
        return "Codice";
    if (formato == Relazione)
        return "Relazione";
    return "Altro";
}

Consegna::Formato Consegna::formatoFromString(const QString &testo)
{
    if (testo.compare("PDF", Qt::CaseInsensitive) == 0)
        return PDF;
    if (testo.compare("ZIP", Qt::CaseInsensitive) == 0)
        return ZIP;
    if (testo.compare("Codice", Qt::CaseInsensitive) == 0)
        return Codice;
    if (testo.compare("Relazione", Qt::CaseInsensitive) == 0)
        return Relazione;
    return Altro;
}

bool Consegna::isInRitardo() const
{
    return !consegnata && data.isValid() && data < QDate::currentDate();
}

int Consegna::giorniRimanenti() const
{
    return QDate::currentDate().daysTo(data);
}

void Consegna::toJson(QJsonObject &json) const
{
    Agenda::toJson(json);
    json["materiaOProgetto"] = materiaOProgetto;
    json["destinatario"] = destinatario;
    json["formato"] = formatoToString();
    json["piattaforma"] = piattaforma;
    json["consegnata"] = consegnata;
}

void Consegna::fromJson(const QJsonObject &json)
{
    Agenda::fromJson(json);
    materiaOProgetto = json["materiaOProgetto"].toString();
    destinatario = json["destinatario"].toString();
    formato = formatoFromString(json["formato"].toString());
    piattaforma = json["piattaforma"].toString();
    consegnata = json["consegnata"].toVariant().toBool();
}
