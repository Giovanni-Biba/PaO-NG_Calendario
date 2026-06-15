#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <QWidget>

#include "agenda.h"

class QLabel;
class QVBoxLayout;
class QPushButton;

class visualize : public QWidget
{
    Q_OBJECT
public:
    explicit visualize(QWidget *parent = nullptr);
    void caricaElemento(std::shared_ptr<Agenda> elemento);

signals:
    void tornaIndietro();
    void richiestaModifica(std::shared_ptr<Agenda> elemento);
    void elementoEliminato();

private slots:
    void eliminaElemento();

private:
    void aggiornaVista();
    void aggiungiRiga(const QString &nome, const QString &valore);

    QLabel *titoloPagina;
    QVBoxLayout *dettagliLayout;
    QPushButton *buttonIndietro;
    QPushButton *buttonModifica;
    QPushButton *buttonElimina;
    std::shared_ptr<Agenda> elementoCorrente;
};

#endif
