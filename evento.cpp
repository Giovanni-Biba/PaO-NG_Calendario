#include "evento.h"

Evento::Evento()
    : Agenda(), capienzaMassima(0), costoBiglietto(0.0), pubblico(false)
{
}

Evento::Evento(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre,
               const QString &luogo, const QString &organizzatore, int capienzaMassima, double costoBiglietto, bool pubblico)
    : Agenda(titolo, descrizione, data, ora, durataOre),
      luogo(luogo),
      organizzatore(organizzatore),
      capienzaMassima(capienzaMassima),
      costoBiglietto(costoBiglietto),
      pubblico(pubblico)
{
}

QString Evento::getTipo() const { return "Evento"; }

QString Evento::riepilogo() const
{
    return QString("%1 presso %2").arg(QString(pubblico ? "Evento pubblico" : "Evento privato"), luogo);
}

QMap<QString, QString> Evento::campiSpecifici() const
{
    return {
        {"Luogo", luogo},
        {"Organizzatore", organizzatore},
        {"Capienza", QString::number(capienzaMassima)},
        {"Costo", QString::number(costoBiglietto, 'f', 2)},
        {"Pubblico", pubblico ? "Si" : "No"}
    };
}

QColor Evento::coloreCalendario() const
{
    return pubblico ? QColor::fromRgb(174, 214, 241) : QColor::fromRgb(214, 234, 248);
}

QString Evento::getLuogo() const { return luogo; }
QString Evento::getOrganizzatore() const { return organizzatore; }
int Evento::getCapienzaMassima() const { return capienzaMassima; }
double Evento::getCostoBiglietto() const { return costoBiglietto; }
bool Evento::isPubblico() const { return pubblico; }

void Evento::setLuogo(const QString &nuovoLuogo) { luogo = nuovoLuogo; }
void Evento::setOrganizzatore(const QString &nuovoOrganizzatore) { organizzatore = nuovoOrganizzatore; }
void Evento::setCapienzaMassima(int nuovaCapienza) { capienzaMassima = nuovaCapienza; }
void Evento::setCostoBiglietto(double nuovoCosto) { costoBiglietto = nuovoCosto; }
void Evento::setPubblico(bool nuovoPubblico) { pubblico = nuovoPubblico; }

void Evento::toJson(QJsonObject &json) const
{
    Agenda::toJson(json);
    json["luogo"] = luogo;
    json["organizzatore"] = organizzatore;
    json["capienzaMassima"] = capienzaMassima;
    json["costoBiglietto"] = costoBiglietto;
    json["pubblico"] = pubblico;
}

void Evento::fromJson(const QJsonObject &json)
{
    Agenda::fromJson(json);
    luogo = json["luogo"].toString();
    organizzatore = json["organizzatore"].toString();
    capienzaMassima = json["capienzaMassima"].toVariant().toInt();
    costoBiglietto = json["costoBiglietto"].toVariant().toDouble();
    pubblico = json["pubblico"].toVariant().toBool();
}
