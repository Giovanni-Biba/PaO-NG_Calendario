#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QPushButton* btnValuta;
    QRadioButton* radioSi;
    QRadioButton* radioNo;
    QPushButton* btnInvia;
    QLabel* labelRisultato;
    QPushButton* btnClose;

private slots:
    void abilitaValutazione();
    void sbloccaBtnInvia();
    void inviaValutazione();
    void chiudiValutazione();
//prova prova
};
#endif // MAINWINDOW_H
