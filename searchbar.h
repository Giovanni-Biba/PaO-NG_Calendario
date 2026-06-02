#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);
    QPushButton *CercaButton;

    QString getTestoTitolo() const { return TitoloInput->text(); }

    // Ritorna la data selezionata SOLO se il check è attivo, altrimenti ritorna QDate() nulla
    QDate getValoreData() const {
        return (DataCheck && DataCheck->isChecked()) ? DataInput->date() : QDate();
    }

    QString getTestoTipo() const { return TipoInput->currentText(); }
    QString getTestoPriorita() const { return PrioritaInput->currentText(); }

private:
    QLineEdit *TitoloInput;
    QCheckBox *DataCheck;
    QDateEdit *DataInput;
    QComboBox *TipoInput;
    QComboBox *PrioritaInput;

signals:
    void cercaClicked();
};

#endif // SEARCHBAR_H