#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QLabel>

class calendar;
class Create;
class Research;
class visualize;
class modify;

class Home : public QMainWindow
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);

private:
    QStackedWidget *stackHome;

    calendar *calendarPage;
    Create *createPage;
    Research *researchPage;
    visualize *visualizePage;
    modify *modifyPage;
    QWidget *paginaPrimaDiVisualize;
};

#endif // HOME_H
