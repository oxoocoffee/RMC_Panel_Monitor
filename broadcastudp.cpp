#include "broadcastudp.h"
#include <QDebug>


BroadcastUDP::BroadcastUDP(QObject *parent) : QObject(parent),
    _qHostAddress("0.0.0.0"), _qHostPort(0)
{
    qDebug() << "qHostAddress: " << _qHostAddress.toString();
}

void BroadcastUDP::setupConnection()
{
  socket = new QUdpSocket(this);

//  socket->bind(_qHostAddress, _qHostPort);

  //socket->bind();
  qDebug() << "Initialized Socket";
  qDebug() << socket->peerAddress().toString();

  QByteArray Data;
  Data.append("Hello from UDP");

//  socket->writeDatagram(Data, _qHostAddress, _qHostPort);

    socket->writeDatagram(Data, QHostAddress::LocalHost, 5824);
  //connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void BroadcastUDP::setHostAddress(QString hostAddress)
{

    _qHostAddress.setAddress(hostAddress);
    qDebug() << "qHostAddress initialized, setting address";
    qDebug() << "qHostAddress: " << _qHostAddress.toString();

}

void BroadcastUDP::setHostPort(quint16 hostPort)
{
  _qHostPort = hostPort;
  qDebug() << "qHostPort = " << _qHostPort;
  qDebug() << "qHostPort = " << _qHostPort;
}
