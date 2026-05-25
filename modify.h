#ifndef MODIFY_H
#define MODIFY_H

#include <QWidget>
#include <QJsonObject>

class modify : public QWidget
{
    Q_OBJECT
public:
    explicit modify(QWidget *parent = nullptr);
    void caricaElemento(const QJsonObject &elemento);

signals:
    void tornaIndietro();

};

#endif // MODIFY_H
