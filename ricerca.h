#ifndef RICERCA_H
#define RICERCA_H

#include <QMainWindow>
#include <QDate>
#include <QTime>
#include <QPushButton>

struct RicercaConfig {
    QString testo;
    //QDateTime periodo;
    QDate data;
    QTime orario;
    QString priorita;
};

class ricerca : public QMainWindow
{
    Q_OBJECT
public:
    explicit ricerca(const RicercaConfig &config = RicercaConfig(),QWidget *parent = nullptr);

private:
    RicercaConfig rConfig;
    QPushButton *closeResearch;

private slots:
    void RitornaHome();
};

#endif // RICERCA_H
