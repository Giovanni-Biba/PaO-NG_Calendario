#ifndef CREATE_H
#define CREATE_H

#include <QWidget>

class QRadioButton;
class QLineEdit;
class QDateEdit;
class QTimeEdit;
class QComboBox;
class QLabel;
class QPushButton;

class Create : public QWidget
{
    Q_OBJECT

public:
    explicit Create(QWidget *parent = nullptr);

private slots:
    void aggiornaVisibilita();

private:
    // Radio
    QRadioButton *rbAttivita;
    QRadioButton *rbEvento;
    QRadioButton *rbAppuntamento;

    // Campi
    QLineEdit *titoloEdit;
    QDateEdit *dataEdit;
    QTimeEdit *oraEdit;
    QComboBox *prioritaBox;
    QLineEdit *luogoEdit;
    QComboBox *statoBox;

    // Label
    QLabel *prioritaLabel;
    QLabel *luogoLabel;
    QLabel *statoLabel;

    // Bottone finale
    QPushButton *creaButton;
};

#endif