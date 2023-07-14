#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimerEvent>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);
 // tcpConnect();


  connect(ui->pushButtonPut_4,
          SIGNAL(clicked(bool)),
          this,
          SLOT(inicia_temp()));

  connect(ui->conectado,
          SIGNAL(clicked()),
          this,
          SLOT(on_connect_clicked()));

  connect(ui->desconectado,
          SIGNAL(clicked()),
          this,
          SLOT(on_disconnect_clicked()));

  connect(ui->butaoparar,
          SIGNAL(clicked()),
          this,
          SLOT(parar_temp()));
}

void MainWindow::tcpConnect(){
  socket->connectToHost(ui->lineEdit->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
    ui->onoff->setText("Conectado");
  }
  else{
    qDebug() << "Disconnected";
    ui->onoff->setText("Desconectado");
  }
}


void MainWindow::putData(){

  QDateTime datetime;
  QString str;
  qint64 msecdate;

  int valor_min = ui->minimo->value();
  int valor_max = ui->maximo->value();

  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    str = "set "+ QString::number(msecdate) + " " +
          QString::number(valor_min+(rand()%(valor_max-valor_min)))+"\r\n";

      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str())
               << " bytes written";
      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote";
          }
      ui->textBrowser->append(str);

  }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
putData();
}

void MainWindow::inicia_temp()
{
    int tempo_seg= 1000*ui->horizontalSlider_temp->value();
    temporizador = startTimer(tempo_seg);
}

void MainWindow::parar_temp()
{
    killTimer(temporizador);
}


void MainWindow::on_connect_clicked(){
  tcpConnect();
}

void MainWindow::on_disconnect_clicked(){
  socket -> disconnectFromHost();
  ui->onoff->setText("Desconectado");
}



MainWindow::~MainWindow(){
  delete socket;
  delete ui;
}
