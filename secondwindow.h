#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

class SecondWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SecondWindow(QWidget *parent = nullptr);

private:
    QPushButton* btnValutazione;
    QLabel* Titolo;
    QLabel* Messaggio;

private slots:
    void valutazione();
};

#endif // SECONDWINDOW_H
