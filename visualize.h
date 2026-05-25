#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <QWidget>
#include <QJsonObject>

class QLabel;
class QVBoxLayout;
class QPushButton;

class visualize : public QWidget
{
    Q_OBJECT
public:
    explicit visualize(QWidget *parent = nullptr);
    void caricaElemento(const QJsonObject &elemento);
    void caricaDaChiave(const QString &titolo, const QString &data, const QString &ora);

signals:
    void tornaIndietro();
    void richiestaModifica(const QJsonObject &elemento);
    void elementoEliminato();

private slots:
    void eliminaElemento();

private:
    bool cercaElemento(const QString &titolo, const QString &data, const QString &ora);
    bool eliminaDaJson();
    bool eliminaDaXml();
    void aggiornaVista();
    void aggiungiRiga(const QString &nome, const QString &valore);
    bool elementiUguali(const QJsonObject &a, const QJsonObject &b) const;
    QString tipoNormalizzato() const;

    QLabel *titoloPagina;
    QVBoxLayout *dettagliLayout;
    QPushButton *buttonIndietro;
    QPushButton *buttonModifica;
    QPushButton *buttonElimina;
    QJsonObject elementoCorrente;
};

#endif // VISUALIZE_H
