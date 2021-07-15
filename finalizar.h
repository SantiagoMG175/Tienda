#ifndef FINALIZAR_H
#define FINALIZAR_H

#include <QDialog>

namespace Ui {
class Finalizar;
}

class Finalizar : public QDialog
{
    Q_OBJECT

public:
    explicit Finalizar(QWidget *parent = nullptr);
    ~Finalizar();
    void fecha();
    void finalizar();
    void productos(QString productos);
    void datos (QString datos);
    void setIva(QString iva);
    void setSubtotal(QString subtotal);
    void setTotal(QString total);

public slots:
    void limpiarOk();

private:
    Ui::Finalizar *ui;


};

#endif // FINALIZAR_H
