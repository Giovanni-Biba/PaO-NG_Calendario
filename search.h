#ifndef SEARCH_H
#define SEARCH_H

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

private:
    QLineEdit *TitoloInput;
    QDateEdit *DataInput;
    QTimeEdit *OraInput;
    QComboBox *PrioritaInput;
    QPushButton *CercaButton;
};

#endif // SEARCH_H