#include "broadcastudp.h"
#include <QDebug>
#include <stdexcept>

BroadcastUDP::BroadcastUDP(QObject* parent) : QObject(parent)
{
    _udpSocket = new QUdpSocket(this);
}

BroadcastUDP::~BroadcastUDP()
{

}

bool BroadcastUDP::IsConnected(void)
{
    return true;
}

void BroadcastUDP::Connect(const QString& host, quint16 port )
{
    if( _udpSocket->isOpen() )
        return;

    connect(_udpSocket, SIGNAL(readyRead()),
            this, SLOT(ReadPendingDatagrams()));

    _udpSocket->connectToHost(host, port);

    qDebug() << "BroadcastUDP Connect: " <<
                host << "  " << _udpSocket->peerAddress().toString();
}

void BroadcastUDP::Disconnect(void)
{
    if( _udpSocket->isOpen() == false )
        return;

    _udpSocket->disconnectFromHost();

    qDebug() << "BroadcastUDP Disconnect";

    disconnect(_udpSocket, SIGNAL(readyRead()),
               this, SLOT(ReadPendingDatagrams()));
}

void    BroadcastUDP::Send(const QByteArray& buffer)
{
    if( _udpSocket->isOpen() == false )
        return;

    emit NetworkMessageTrace(eOut, QString(buffer.toHex()));

    _udpSocket->write(buffer);
}

void    BroadcastUDP::ReadPendingDatagrams()
{
    qDebug() << "BroadcastUDP Reader Created";

    QByteArray   datagram;
    QHostAddress host;
    quint16      port;

    while (_udpSocket->hasPendingDatagrams())
    {
        datagram.resize(_udpSocket->pendingDatagramSize());

        _udpSocket->readDatagram(datagram.data(), datagram.size(), &host, &port );

        if( _udpSocket->peerAddress() != host && _udpSocket->peerPort() != port )
        {
            ProcessTheDatagram(datagram);
        }

        datagram.clear();
    }

    qDebug() << "BroadcastUDP Reader Finished";
}

void    BroadcastUDP::ProcessTheDatagram(const QByteArray& buffer)
{
    emit NetworkMessageTrace(eIn, QString(buffer.toHex()));
}
