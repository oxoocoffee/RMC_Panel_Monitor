#ifndef BROADCASTUDP_H
#define BROADCASTUDP_H

#include <QThread>
#include <QUdpSocket>

class BroadcastUDP : public QObject
{
        Q_OBJECT

        public:
            enum eDirection
            {
                eOut,
                eIn
            };

           explicit BroadcastUDP(QObject* parent = 0L);
           virtual ~BroadcastUDP();

           bool     IsConnected(void);
           void     Connect(const QString& host, quint16 port );
           void     Disconnect(void);
           void     Send(const QByteArray& buffer);

        public slots:

        signals:
           void    NetworkMessageTrace(const BroadcastUDP::eDirection dir,
                                       const QString& message);

        private:
           void    ReadPendingDatagrams(void);
           void    ProcessTheDatagram(const QByteArray& buffer);

        private:
           QUdpSocket*   _udpSocket;
};

#endif // BROADCASTUDP_H
