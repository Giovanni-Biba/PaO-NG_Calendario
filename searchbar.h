#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);
    QPushButton *CercaButton;
    QString getTestoTitolo() const { return TitoloInput->text(); }
    QDate getValoreData() const { return DataInput->date(); }
    QTime getValoreOra() const { return OraInput->time(); }
    QString getTestoPriorita() const { return PrioritaInput->currentText(); }

private:
    QLineEdit *TitoloInput;
    QDateEdit *DataInput;
    QTimeEdit *OraInput;
    QComboBox *PrioritaInput;


signals:
    void cercaClicked();
};

#endif // SEARCHBAR_H
