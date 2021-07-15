#include "productos.h"
#include "ui_productos.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include<QMessageBox>

Productos::Productos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Productos)
{
    ui->setupUi(this);

    connect(ui->cmdAgregar, SIGNAL(clicked(bool)), this, SLOT(Agregar()));
    connect(ui->cmdEliminar, SIGNAL(clicked(bool)), this, SLOT(Eliminar()));
    connect(ui->cmdGuardar, SIGNAL(clicked(bool)), this, SLOT(Guardar()));
    QStringList inicio = {"Codigo", "Producto", "Precio"};
            ui->outProductos->setColumnCount(3);
            ui->outProductos->setHorizontalHeaderLabels(inicio);

}

Productos::~Productos()
{
    delete ui;
}

void Productos::setIni(QStringList inicio)
{
    ui->outProductos->setHorizontalHeaderLabels(inicio);
}

void Productos::setProductos(int codigo, QString producto, float precio)
{
    int posicion = ui->outProductos->rowCount();
        ui->outProductos->insertRow(posicion);
        ui->outProductos->setItem(posicion,0,new QTableWidgetItem(QString::number(codigo)));
        ui->outProductos->setItem(posicion,1,new QTableWidgetItem(producto));
        ui->outProductos->setItem(posicion,2,new QTableWidgetItem(QString::number(precio)));
}

void Productos::Agregar()

{
    int ing = ui->outProductos->rowCount();
        ui->outProductos->insertRow(ing);
}

void Productos::Eliminar()
{
    int eli = ui->outProductos->currentRow();
           ui->outProductos->removeRow(eli);
}

void Productos::Guardar()
{
    QMessageBox mensajeBox;
            QString fileName = "Productos.csv";
                if (fileName.isEmpty()) {
                    return;
                }

                QFile file(fileName);
                if (!file.open(QIODevice::WriteOnly |   QFile::Text)) {
                    QMessageBox::information(this, tr("No se puede abrir el archivo"),
                    file.errorString());
                    return;
                }

                QTextStream out(&file);
                int rowCount = ui->outProductos->rowCount();
                int colCount = ui->outProductos->columnCount();

                for (int i = 0; i < rowCount; i++) {

                    for (int j = 0; j < colCount; j++) {
                        if (j > 0) {
                            out << ";";
                        }
                        QTableWidgetItem* item = ui->outProductos->item(i,j);
                        out << item->data(Qt::DisplayRole).toString();
                    }
                    out << "\n";
                }
                mensajeBox.setText("Se guardó en el archivo:" + fileName);
                mensajeBox.exec();
}

void Productos::salir()
{
    this->close();
}


void Productos::on_retornar_accepted()
{
    this->salir();
}
