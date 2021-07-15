#include "finalizar.h"
#include "ui_finalizar.h"
#include <QDateTime>
#include <QDate>
#include "principal.h"

Finalizar::Finalizar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Finalizar)
{
    ui->setupUi(this);
    fecha();
    close();
    connect( ui->OK, SIGNAL(accepted()),this,SLOT(limpiarOk()));

}

Finalizar::~Finalizar()
{
    delete ui;
}

void Finalizar::fecha()
{
    QDate fecha = QDate::currentDate();
    QString Date = fecha.toString(Qt::DefaultLocaleLongDate);
    ui->outFecha->setText(Date);
}
void Finalizar::productos(QString productos)
{
    ui->outResultado2->setPlainText(productos);
}
void Finalizar::datos(QString datos)
{
    ui->outResultado->setPlainText(datos);
}


void Finalizar::setIva(QString iva)
{
    ui->outIva->setText(iva);
}

void Finalizar::setSubtotal(QString subtotal)
{
    ui->outSubtotal->setText(subtotal);
}

void Finalizar::setTotal(QString total)
{
    ui->outTotal->setText(total);
}

void Finalizar::limpiarOk()
{
    Principal *principal = new Principal(this);
    principal->limpiar();
    principal->show();
}

