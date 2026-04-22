#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include "search.h"

class calendar : public QWidget
{
    Q_OBJECT
public:
    explicit calendar(QWidget *parent = nullptr);

private:
    QPushButton* Titolo;
    Search *BarraRicerca;

private slots:
    //void visualizzaSelezione();
    //void cambiaSettimanaGriglia();
    void closeCalendar();};

#endif // CALENDAR_H
