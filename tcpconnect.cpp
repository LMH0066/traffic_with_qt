#include "tcpconnect.h"
#include <QBuffer>
#include <QImageReader>

TcpConnect::TcpConnect() {
    tcpSocketConnection = new QTcpSocket();
    tcpServer = new QTcpServer();
    ip = "10.16.148.210";
    port = "7777";
}

void TcpConnect::sendMessage(QImage pic) {
    newConnect();
    QByteArray Data;
    QBuffer  buffer;
    buffer.open(QIODevice::ReadWrite);
    pic.save(&buffer,"jpg");
    quint32 pic_len = (quint32)buffer.data().size();
    Data.append(buffer.data());
    tcpSocketConnection->write(Data);
}

QImage TcpConnect::readMessage() {
    QDataStream in(tcpSocketConnection);
    in.setVersion(QDataStream::Qt_5_12);
    QByteArray array;
    while (tcpSocketConnection->waitForReadyRead(100)) {
        array.append((QByteArray)tcpSocketConnection->readAll());
    }
    QBuffer buffer(&array);
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer,"JPG");
    QImage img = reader.read();
}

void TcpConnect::newConnect() {
    tcpSocketConnection->abort();
    tcpSocketConnection->connectToHost(ip, port.toInt());
    if (tcpSocketConnection->waitForConnected(100)) {
        cout << "connect success~" << endl;
    }
    connect(tcpSocketConnection,SIGNAL(readyRead()),this,SLOT(readMessage()));//readyRead()表示服务端发送数据过来即发动信号
}
