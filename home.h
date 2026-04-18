#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>

class Home : public QMainWindow
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);

private:
    QPushButton* Titolo;

private slots:
    //void ricaricaHome();
};

#endif // HOME_H
