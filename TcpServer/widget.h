#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpServer>
#include"mycombobox.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT



public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QTcpServer* server;

public slots:
    void my_newClient_connect();
    void my_readyRead_handle();
    void my_disconnected_handle();
//    void my_socketState_changed(QAbstractSocket::SocketState socketState);
    void my_combobox_refresh();

private slots:
    void on_btnListen_clicked();


    void on_btnSend_clicked();

    void on_comboBoxChildren_activated(int index);

    void on_btnStopListen_clicked();

private:
    Ui::Widget *ui;
    int childIndex;
};

#endif // WIDGET_H
