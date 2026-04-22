#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QLabel>

class calendar;

class Home : public QMainWindow
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);

private:
    QStackedWidget *stackHome;

    QWidget *calendarPage;
};

#endif // HOME_H
