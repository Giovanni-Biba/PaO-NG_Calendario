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
    SearchBar* getSearchBar() const { return BarraRicerca; }

private:
    QPushButton* Titolo;
    SearchBar *BarraRicerca;
    QPushButton* Crea;


private slots:
    //void visualizzaSelezione();
    //void cambiaSettimanaGriglia();
    void closeCalendar();

signals:
    void richiestaCrea();
    void richiestaCerca();
};

#endif // CALENDAR_H
