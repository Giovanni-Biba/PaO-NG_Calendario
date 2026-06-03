#ifndef AGENDA_H
#define AGENDA_H

#include <QColor>
#include <QDate>
#include <QJsonObject>
#include <QMap>
#include <QMetaType>
#include <QString>
#include <QTime>

#include <memory>

class Agenda
{
protected:
    QString titolo;
    QString descrizione;
    QDate data;
    QTime ora;
    int durataOre;

public:
    enum Priorita { Bassa, Media, Alta };

    Agenda();
    Agenda(const QString &titolo, const QString &descrizione, const QDate &data, const QTime &ora, int durataOre, Priorita priorita = Media);
    virtual ~Agenda();

    QString getTitolo() const;
    QString getDescrizione() const;
    QDate getData() const;
    QTime getOra() const;
    int getDurataOre() const;
    Priorita getPriorita() const;

    void setTitolo(const QString &nuovoTitolo);
    void setDescrizione(const QString &nuovaDescrizione);
    void setData(const QDate &nuovaData);
    void setOra(const QTime &nuovaOra);
    void setDurataOre(int nuovaDurata);
    void setPriorita(Priorita nuovaPriorita);

    virtual QString getTipo() const = 0;
    virtual QString riepilogo() const = 0;
    virtual QMap<QString, QString> campiSpecifici() const = 0;
    virtual QColor coloreCalendario() const = 0;
    virtual bool usaRigaFestivita() const;
    virtual bool matchesFiltro(const QString &testo, const QDate &dataFiltro, const QString &tipo, const QString &priorita) const;

    virtual void toJson(QJsonObject &json) const;
    virtual void fromJson(const QJsonObject &json);

    bool stessaChiave(const Agenda &altro) const;
    QString prioritaToString() const;
    static Priorita prioritaFromString(const QString &testo);

private:
    Priorita priorita;
};

Q_DECLARE_METATYPE(std::shared_ptr<Agenda>)

#endif
