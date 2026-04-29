#ifndef RESEARCH_H
#define RESEARCH_H

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

class Research : public QMainWindow
{
    Q_OBJECT
public:
    explicit Research(const RicercaConfig &config = RicercaConfig(),QWidget *parent = nullptr);

private:
    RicercaConfig rConfig;
    QPushButton *closeResearch;

private slots:
    void RitornaHome();
};

#endif // RESEARCH_H
