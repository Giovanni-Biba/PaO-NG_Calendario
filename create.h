#ifndef CREATE_H
#define CREATE_H

#include <QWidget>

#include <memory>

class QRadioButton;
class QLineEdit;
class QTextEdit;
class QDateEdit;
class QTimeEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QSpinBox;
class QCheckBox;

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
    QRadioButton *rbFestivita;
    QRadioButton *rbConsegna;

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
    QLineEdit *categoriaEdit;
    QCheckBox *completataCheck;
    QLineEdit *organizzatoreEdit;
    QSpinBox *capienzaSpin;
    QSpinBox *costoSpin;
    QCheckBox *pubblicoCheck;
    QLineEdit *partecipantiEdit;
    QComboBox *modalitaBox;
    QLineEdit *linkOnlineEdit;
    QCheckBox *confermatoCheck;
    QCheckBox *ricorrenzaCheck;
    QCheckBox *nonLavorativoCheck;
    QLineEdit *nomeUfficialeEdit;
    QLineEdit *materiaEdit;
    QLineEdit *destinatarioEdit;
    QComboBox *formatoBox;
    QLineEdit *piattaformaEdit;
    QCheckBox *consegnataCheck;

    // Label
    QLabel *descrizioneLabel;
    QLabel *prioritaLabel;
    QLabel *dataFineLabel;
    QLabel *oraFineLabel;
    QLabel *durataLabel;
    QLabel *luogoLabel;
    QLabel *statoLabel;
    QLabel *categoriaLabel;
    QLabel *organizzatoreLabel;
    QLabel *capienzaLabel;
    QLabel *costoLabel;
    QLabel *partecipantiLabel;
    QLabel *modalitaLabel;
    QLabel *linkOnlineLabel;
    QLabel *nomeUfficialeLabel;
    QLabel *materiaLabel;
    QLabel *destinatarioLabel;
    QLabel *formatoLabel;
    QLabel *piattaformaLabel;
    QPushButton *creaButton;
    QPushButton *indietroButton;

    bool validaCampi();
    void resetCampi();
    std::shared_ptr<class Agenda> creaImpegnoDaForm() const;

signals:
    void tornaIndietro();
};

#endif
