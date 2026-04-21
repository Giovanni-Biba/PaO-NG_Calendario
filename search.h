#ifndef SEARCH_H
#define SEARCH_H

#include "ricerca.h"
#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class Search : public QWidget
{
    Q_OBJECT
public:
    explicit Search(QWidget *parent = nullptr);
    QPushButton *CercaButton;

private:
    QLineEdit *TitoloInput;
    QDateEdit *DataInput;
    QTimeEdit *OraInput;
    QComboBox *PrioritaInput;


private slots:
    void Research();
};

#endif // SEARCH_H