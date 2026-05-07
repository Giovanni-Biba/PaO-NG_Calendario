#ifndef CREATE_H
#define CREATE_H

#include <QWidget>

class QRadioButton;
class QLineEdit;
class QTextEdit;
class QDateEdit;
class QTimeEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QSpinBox;

class Create : public QWidget
{
    Q_OBJECT

public:
    explicit Create(QWidget *parent = nullptr);

private slots:
    void aggiornaVisibilita();
    void salvaDati();

private:
    // Radio
    QRadioButton *rbAttivita;
    QRadioButton *rbEvento;
    QRadioButton *rbAppuntamento;

    // Campi comuni
    QLineEdit *titoloEdit;
    QTextEdit *descrizioneEdit;

    QDateEdit *dataEdit;
    QTimeEdit *oraEdit;

    QComboBox *prioritaBox;

    // Attività
    QDateEdit *dataFineEdit;
    QTimeEdit *oraFineEdit;

    // Evento/Appuntamento
    QSpinBox *durataSpin;

    // Evento
    QLineEdit *luogoEdit;

    // Appuntamento
    QComboBox *statoBox;

    // Label
    QLabel *descrizioneLabel;

    QLabel *prioritaLabel;

    QLabel *dataFineLabel;
    QLabel *oraFineLabel;

    QLabel *durataLabel;

    QLabel *luogoLabel;

    QLabel *statoLabel;

    QPushButton *creaButton;
    QPushButton *indietroButton;

    // Funzioni helper
    bool validaCampi();
    bool salvaJSON();
    bool salvaXML();

signals:
    void tornaIndietro();
};

#endif