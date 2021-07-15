#include "principal.h"
#include "ui_principal.h"

#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#define EMAIL_RX "^[_a-z0-9]+(\\.[_a-z0-9-]+)@[a-z0-9-]+(\\.[a-z0-9-]+)(\\.[a-z]{2,4})$"
#include <QRegExpValidator>


Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    inicializarDatos();
    connect(ui->mnuSalir, SIGNAL(triggered(bool)),
           this, SLOT(close()));
    connect(ui->cmdFinalizar, SIGNAL(clicked(bool)),
            this, SLOT(finalizar()));
}

Principal::~Principal()
{
    delete ui;
}

void Principal::mostrarPrecio(int index)
{
    float precio = m_productos.at(index)->precio();
    ui->outPrecio->setText("$ " + QString::number(precio,'f',2));
}

void Principal::agregarProducto()
{
    int index = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(index);
    int cantidad = ui->inCantidad->value();

    if (cantidad == 0){
        ui->statusbar->showMessage("No se ha ingresado la cantidad del producto para poder agregar.", 6000);
        return;
    } else{
        ui->statusbar->clearMessage();
    }

    float subtotal = cantidad * p->precio();
    if (!buscar(p, cantidad)){
        int posicion = ui->outDetalle->rowCount();
        ui->outDetalle->insertRow(posicion);
        ui->outDetalle->setItem(posicion,0, new QTableWidgetItem(QString::number(cantidad)));
        ui->outDetalle->setItem(posicion,1, new QTableWidgetItem(p->nombre()));
        ui->outDetalle->setItem(posicion,2, new QTableWidgetItem(QString::number(subtotal,'f',2)));
    }
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();
    calcular (subtotal);
}

void Principal::validarcedula()
{
    int cedula,pares,impares,total,dec=0;
    int a,b,c,d,e,f,g,h,i,j;

    cedula  = ui->inCedula->text().toInt();
    a=cedula/1000000000;
    cedula=cedula-(a*1000000000);
    b=cedula/100000000;
    cedula=cedula-(b*100000000);
    c=cedula/10000000;
    cedula=cedula-(c*10000000);
    d=cedula/1000000;
    cedula=cedula-(d*1000000);
    e=cedula/100000;
    cedula=cedula-(e*100000);
    f=cedula/10000;
    cedula=cedula-(f*10000);
    g=cedula/1000;
    cedula=cedula-(g*1000);
    h=cedula/100;
    cedula=cedula-(h*100);
    i=cedula/10;
    cedula=cedula-(i*10);
    j=cedula/1;
    cedula=cedula-(j*1);

    if (cedula>240000000){
        ui->inCedula->setStyleSheet("color : red");
    }else{
        pares=b+d+f+h;
        a=a*2;
        if (a>9){
            a=a%10+a/10;
        }
        c=c*2;
        if (c>9){
            c=c%10+c/10;
        }
        e=e*2;
        if (e>9){
            e=e%10+e/10;
        }
        g=g*2;
        if (g>9){
            g=g%10+g/10;
        }
        i=i*2;
        if (i>9){
            i=i%10+i/10;
        }
        impares=a+c+e+g+i;
        total=pares+impares;
        while (dec-total!=j && dec<total+10){
            dec=dec+10;
        }
        if (dec-total==j){
            ui->inCedula->setStyleSheet("color : blue");
        }else {
            ui->inCedula->setStyleSheet("color : green");
        }
    }
}

void Principal::validaremail()
{
    QRegExp rx(EMAIL_RX);
    if(QRegExpValidator *valiEmail = new QRegExpValidator(rx,this)){
        ui->inEmail->setValidator(valiEmail);
        ui->inEmail->setStyleSheet("color : green");
    }else{
        ui->inEmail->setStyleSheet("color: red");
    }
}

void Principal::inicializarDatos()
{

    QFile archivo ("Productos.csv");
    if(archivo.open(QFile::ReadOnly)){
        QTextStream in(&archivo);
        while (!in.atEnd()){
            QString linea = in.readLine();
            QStringList datos = linea.split(';');
            int codigo = datos[0].toInt();
            float precio = datos[2].toFloat();
            m_productos.append(new Producto(codigo,datos[1],precio));
        }
    }else{
        qDebug () << "Error al abrir el archivo";
    }
    archivo.close();
    inicializarWidgets();
}


void Principal::inicializarWidgets()
{
    for (int i = 0; i < m_productos.length(); ++i) {
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    }

    QStringList cabecera = {"Cantidad", "Producto", "Sub Total"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    connect(ui->inProducto, SIGNAL(currentIndexChanged(int)),
            this, SLOT(mostrarPrecio(int)));
    connect(ui->cmdAgregar, SIGNAL(clicked(bool)),
            this, SLOT(agregarProducto()));
    connect(ui->inCedula, SIGNAL(editingFinished()),
            this, SLOT(validarcedula()));
    connect(ui->inEmail, SIGNAL(editingFinished()),
            this, SLOT(validaremail()));

    mostrarPrecio(0);
}

void Principal::calcular(float subtotal)
{
    m_subtotal += subtotal;
    float iva = m_subtotal * 0.12;
    float total = m_subtotal + iva;

    ui->outSubtotal->setText(QString::number(m_subtotal,'f',2));
    ui->outIva->setText(QString::number(iva,'f',2));
    ui->outTotal->setText(QString::number(total,'f',2));
}

bool Principal::buscar(Producto *producto, int cantidad)
{
    int numfilas = ui->outDetalle->rowCount();
    for (int i = 0; i < numfilas; ++i) {
        QTableWidgetItem *item = ui->outDetalle->item(i,1);
        QString dato = item->data(Qt::DisplayRole).toString();
        if (dato == producto->nombre()){
            QTableWidgetItem *item = ui->outDetalle->item(i,0);
            int cantidadActual = item->data(Qt::DisplayRole).toInt();
            int cantidadNueva = cantidadActual + cantidad;
            float subtotal = cantidadNueva *producto->precio();
            ui->outDetalle->setItem(i,0, new QTableWidgetItem(QString::number(cantidadNueva)));
            ui->outDetalle->setItem(i,2, new QTableWidgetItem(QString::number(subtotal)));
            return true;
        }
    }
    return false;
}
void Principal::finalizar()
{
    Finalizar *dlgFinalizar = new Finalizar();

    QStringList Lista;
    QString productos;
    for( int fila = 0; fila < ui->outDetalle->rowCount(); ++fila)
    {
        for( int columna = 0; columna < ui->outDetalle->columnCount(); ++columna){
            Lista << ui->outDetalle->item(fila, columna)->text();
            QTableWidgetItem* item = ui->outDetalle->item(fila,columna);
            if (!item || item->text().isEmpty()){
                ui->outDetalle->setItem(fila,columna,new QTableWidgetItem("0"));
            }
            productos = Lista.join( " " )+" ";
            Lista<<"                   ";
        }
        Lista << " ";
        dlgFinalizar->productos(productos);
    }

    dlgFinalizar->datos(getDatos());
    dlgFinalizar->setSubtotal(ui->outSubtotal->text());
    dlgFinalizar->setIva(ui->outIva->text());
    dlgFinalizar->setTotal(ui->outTotal->text());
    dlgFinalizar->show();
}

void Principal::limpiar()
{

    ui->inNombre->setText("");
    ui->inCedula->setText("");
    ui->inEmail->setText("");
    ui->inCantidad->setValue(0);
    ui->inTelefono->setText("");
    ui->inDireccion->setPlainText("");
    ui->outDetalle->setRowCount(0);
}


QString Principal::getDatos()
{
    return  "Cédula: " + ui->inCedula->displayText() + "\n" +
            "\nNombre: " + ui->inNombre->displayText() + "\n" +
            "\nTeléfono: " + ui->inTelefono->displayText() + "\n" +
            "\nE-mail: " + ui->inEmail->displayText() + "\n" +
            "\nDirección: " + ui->inDireccion->toPlainText()+ "\n"+
            "\n****************************************************** \n"+
            "\nCantidad             Producto                Subtotal\n"+
            "\n****************************************************** ";
}


void Principal::on_mnuProductos_triggered()
{
    Productos *productos = new Productos(this);
    productos->setModal(true);

    QFile archivo ("Productos.csv");
    if(archivo.open(QFile::ReadOnly)){
        QTextStream in(&archivo);
        while (!in.atEnd()){
            QString linea = in.readLine();
            QStringList datos = linea.split(';');
            int codigo = datos[0].toInt();
            QString producto = datos[1];
            float precio = datos[2].toFloat();
            qDebug() << codigo << producto << precio;
            productos->setProductos(codigo,producto,precio);
        }
    }else{
        qDebug () << "Error al abrir el archivo";
    }
    archivo.close();
    productos->show();
}
