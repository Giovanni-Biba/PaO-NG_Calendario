#ifndef MODIFY_H
#define MODIFY_H

#include "agenda.h"
#include <QWidget>
#include <memory>

class QCheckBox;
class QComboBox;
class QDateEdit;
class QLabel;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
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
    // Widget Comuni
    QLabel *titoloPagina;
    QLineEdit *editTitolo;
    QDateEdit *editData;
    QLabel *labelOra;
    QTimeEdit *editOra;
    QLabel *labelDurata;
    QSpinBox *spinDurata;

    QTextEdit *editDescrizione;

    // Widget priorita / Attivita
    QLabel *labelPriorita;
    QComboBox *comboPriorita;
    QLabel *labelCategoria;
    QLineEdit *editCategoria;
    QCheckBox *checkCompletata;

    // Widget per Evento / Appuntamento
    QLabel *labelLuogo;
    QLineEdit *editLuogo;
    QLabel *labelOrganizzatore;
    QLineEdit *editOrganizzatore;

    // Specifici Evento
    QLabel *labelCapienza;
    QSpinBox *spinCapienza;
    QLabel *labelCosto;
    QDoubleSpinBox *spinCosto;
    QCheckBox *checkPubblico;

    // Specifici Appuntamento
    QLabel *labelModalita;
    QComboBox *comboModalita;
    QLabel *labelPartecipanti;
    QLineEdit *editPartecipanti;
    QLabel *labelLinkOnline;
    QLineEdit *editLinkOnline;
    QCheckBox *checkConfermato;

    // Widget per Festivita
    QLabel *labelStato;
    QComboBox *comboStato;
    QLabel *labelNomeUfficiale;
    QLineEdit *editNomeUfficiale;
    QCheckBox *checkRicorrenza;
    QCheckBox *checkLavorativo;

    // Widget per Consegna
    QLabel *labelMateria;
    QLineEdit *editMateria;
    QLabel *labelDestinatario;
    QLineEdit *editDestinatario;
    QLabel *labelFormato;
    QComboBox *comboFormato;
    QLabel *labelPiattaforma;
    QLineEdit *editPiattaforma;
    QCheckBox *checkConsegnata;

    std::shared_ptr<Agenda> elementoCorrente;

    void aggiornaVisibilita();
    std::shared_ptr<Agenda> creaModificato() const;
};

#endif
