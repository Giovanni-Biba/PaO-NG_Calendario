#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include "searchbar.h"

class calendar : public QWidget
{
    Q_OBJECT
public:
    explicit calendar(QWidget *parent = nullptr);

private:
    QPushButton* Titolo;
    SearchBar *BarraRicerca;

private slots:
    //void visualizzaSelezione();
    //void cambiaSettimanaGriglia();
    void closeCalendar();};

#endif // CALENDAR_H
