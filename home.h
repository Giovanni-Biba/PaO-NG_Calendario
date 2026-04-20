#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include "search.h"

class Home : public QMainWindow
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);

private:
    QPushButton* Titolo;
    Search *BarraRicerca;

private slots:
    //void ricaricaHome();
};

#endif // HOME_H
