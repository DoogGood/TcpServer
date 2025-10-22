#include "widget.h"
#include "ui_widget.h"

#include <QNetworkInterface>
#include <QTcpSocket>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //初始化UI
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);

    //实例化一个TcpServer对象
    server= new QTcpServer(this);
    //连接server的信号与槽函数
    connect(server,&QTcpServer::newConnection,this,&Widget::my_newClient_connect);

    //连接MycomboBox的clicked信号
    connect(ui->comboBoxChildren,&MyComboBox::my_comboBox_clicked,this,&Widget::my_combobox_refresh);

    //获取本机所有IP地址,添加到combobox中
    QList<QHostAddress> ipList =QNetworkInterface::allAddresses();
    for(QHostAddress tmp:ipList){
        if(tmp.protocol()==QAbstractSocket::IPv4Protocol){
            ui->comboBoxAddr->addItem(tmp.toString());
        }
    }

}

Widget::~Widget()
{
    delete ui;
}

void Widget::my_newClient_connect()
{

    qDebug()<<"new connection";
    //有新连接后创建一个TcpSocket变量
    if(server->hasPendingConnections()){
        QTcpSocket* tcpSocket= server->nextPendingConnection();

        qDebug()<<tcpSocket->peerAddress()<<tcpSocket->peerPort();

        ui->textEditRev->insertPlainText("客户端地址:"+tcpSocket->peerAddress().toString()+"客户端端口:"+QString::number(tcpSocket->peerPort())+"\n");

        //连接readyRead信号
        connect(tcpSocket,&QIODevice::readyRead,this,&Widget::my_readyRead_handle,Qt::UniqueConnection);
        //连接disconnect信号
        connect(tcpSocket,&QAbstractSocket::disconnected,this,&Widget::my_disconnected_handle,Qt::UniqueConnection);
    }

}

void Widget::my_readyRead_handle()
{
    qDebug()<<"Rev Data";
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
    QByteArray revData= tcpSocket->readAll();
    ui->textEditRev->insertPlainText("客户端:"+revData+'\n');

}

void Widget::my_disconnected_handle()
{
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
    qDebug()<<"disconnected!";
    ui->textEditRev->insertPlainText("客户端断开\n");

    disconnect(tcpSocket,&QIODevice::readyRead,this,&Widget::my_readyRead_handle);
    disconnect(tcpSocket,&QAbstractSocket::disconnected,this,&Widget::my_disconnected_handle);
    tcpSocket->deleteLater();


}

void Widget::my_combobox_refresh()
{
    ui->comboBoxChildren->clear();
    QList<QTcpSocket*> tcpSockets = server->findChildren<QTcpSocket*>();
    for(QTcpSocket*tmp:tcpSockets){
        ui->comboBoxChildren->addItem(QString::number(tmp->peerPort()));
    }
    ui->comboBoxChildren->addItem("All");



}

void Widget::on_btnListen_clicked()
{
    quint16 port=ui->lineEditPort->text().toUShort();
    if(!server->listen(QHostAddress(ui->comboBoxAddr->currentText()),port)){
        qDebug()<<"Listen error";
        return;
    }
    qDebug()<<"Listening";
    ui->btnListen->setEnabled(false);
    ui->btnStopListen->setEnabled(true);

}



void Widget::on_btnSend_clicked()
{
    QList<QTcpSocket*>tcpSocketClients = server->findChildren<QTcpSocket*>();
    if(ui->comboBoxChildren->currentText()!="All"){
        tcpSocketClients[childIndex]->write(ui->textEditSend->toPlainText().toStdString().c_str());
    }else {
        for(QTcpSocket*tmp:tcpSocketClients){
            tmp->write(ui->textEditSend->toPlainText().toStdString().c_str());
        }
    }


}

void Widget::on_comboBoxChildren_activated(int index)
{
    childIndex=index;
}

void Widget::on_btnStopListen_clicked()
{
    QList<QTcpSocket*>tcpSockets=server->findChildren<QTcpSocket*>();
    for(QTcpSocket*tmp:tcpSockets){
        //回收所有socket
        tmp->deleteLater();
    }
    server->close();
    ui->btnListen->setEnabled(true);
    ui->btnStopListen->setEnabled(false);
}
