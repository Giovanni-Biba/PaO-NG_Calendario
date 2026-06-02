#ifndef MODIFY_H
#define MODIFY_H

#include "agenda.h"

#include <QWidget>

class QCheckBox;
class QComboBox;
class QDateEdit;
class QLabel;
class QLineEdit;
class QSpinBox;
class QTextEdit;
class QTimeEdit;

class modify : public QWidget
{
    Q_OBJECT
public:
    explicit modify(QWidget *parent = nullptr);
    void caricaElemento(std::shared_ptr<Agenda> elemento);

signals:
    void tornaIndietro();
    void salvataggioCompletato(std::shared_ptr<Agenda> nuovoElemento);

private:
    QLineEdit *editTitolo;
    QDateEdit *editData;
    QTimeEdit *editOra;
    QSpinBox *spinDurata;
    QTextEdit *editDescrizione;

    QLabel *labelPriorita;
    QComboBox *comboPriorita;
    QLabel *labelCategoria;
    QLineEdit *editCategoria;
    QCheckBox *checkCompletata;

    QLabel *labelLuogo;
    QLineEdit *editLuogo;
    QLabel *labelOrganizzatore;
    QLineEdit *editOrganizzatore;
    QLabel *labelStato;
    QComboBox *comboStato;
    QCheckBox *checkConfermato;

    QLabel *labelMateria;
    QLineEdit *editMateria;
    QLabel *labelDestinatario;
    QLineEdit *editDestinatario;
    QLabel *labelFormato;
    QComboBox *comboFormato;
    QLabel *labelPiattaforma;
    QLineEdit *editPiattaforma;
    QCheckBox *checkConsegnata;

    QLabel *titoloPagina;
    std::shared_ptr<Agenda> elementoCorrente;

    void aggiornaVisibilita();
    std::shared_ptr<Agenda> creaModificato() const;
};

#endif
