#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QTcpServer>
#include <QTcpSocket>
#include "overall.h"

class TcpConnect : public QObject {
  public:
    TcpConnect();
    QTcpSocket *tcpSocketConnection;
  public slots:
    void sendMessage(QImage pic); //发送数据
    QImage readMessage(); //读取数据
    void newConnect();
  private:
    QString ip, port;
    QTcpServer *tcpServer;
};

#endif // TCPCONNECT_H
