#ifndef MODIFY_H
#define MODIFY_H

#include <QWidget>
#include <QJsonObject>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>

class modify : public QWidget
{
    Q_OBJECT
public:
    explicit modify(QWidget *parent = nullptr);
    void caricaElemento(const QJsonObject &elemento);

signals:
    void tornaIndietro();
    void salvataggioCompletato(QJsonObject nuovoElemento);

private:
    //input
    QLineEdit *editTitolo;
    QDateEdit *editData;
    QTimeEdit *editOra;
    QSpinBox  *spinDurata;
    QComboBox *comboPriorita;
    QTextEdit *editDescrizione;
    // Campi extra
    QLineEdit *editLuogo;
    QLineEdit *editStato;
    QLabel *labelLuogo;
    QLabel *labelStato;

    QLabel *titoloPagina;
    QJsonObject elementoCorrente; // Memorizza i dati originali per la ricerca nel database

    // Funzioni per il salvataggio fisico su file
    bool salvaSuJson(const QJsonObject &nuovo);
    bool salvaSuXml(const QJsonObject &nuovo);
    bool elementiUguali(const QJsonObject &a, const QJsonObject &b) const;
};


#endif // MODIFY_H