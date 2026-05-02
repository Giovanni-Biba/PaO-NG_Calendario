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

private:
    QLineEdit *TitoloInput;
    QDateEdit *DataInput;
    QTimeEdit *OraInput;
    QComboBox *PrioritaInput;

signals:
    void cercaClicked();
};

#endif // SEARCHBAR_H
