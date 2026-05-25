#include "modify.h"

#include <QPushButton>
#include <QVBoxLayout>

modify::modify(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(12);

    QPushButton *indietro = new QPushButton("← Torna al dettaglio", this);
    indietro->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; border-radius: 5px; padding: 8px; font-weight: bold; }");
    mainLayout->addWidget(indietro);
    mainLayout->addStretch();

    connect(indietro, &QPushButton::clicked, this, &modify::tornaIndietro);
}

void modify::caricaElemento(const QJsonObject &elemento)
{
    Q_UNUSED(elemento);
}
