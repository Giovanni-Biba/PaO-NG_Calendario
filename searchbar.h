#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

class QTimer;

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);
    QPushButton *CercaButton;

    QString getTestoTitolo() const { return TitoloInput->text(); }
    QDate getValoreData() const {
        return (DataCheck && DataCheck->isChecked()) ? DataInput->date() : QDate();
    }

    QString getTestoTipo() const { return TipoInput->currentText(); }
    QString getTestoPriorita() const { return PrioritaInput->currentText(); }
    // modifiche seconda consegna: permette di riusare la barra anche nella pagina risultati.
    void impostaFiltri(const QString &titolo, const QDate &data, const QString &tipo, const QString &priorita);

private:
    QLineEdit *TitoloInput;
    QCheckBox *DataCheck;
    QDateEdit *DataInput;
    QComboBox *TipoInput;
    QComboBox *PrioritaInput;
    QTimer *timerRicerca;

signals:
    void cercaClicked();
    // modifiche seconda consegna: segnale emesso quando cambia un filtro della ricerca.
    void filtriCambiati();
};

#endif
