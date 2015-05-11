#ifndef BROADCASTUDP_H
#define BROADCASTUDP_H

#include <QObject>
#include <QUdpSocket>

class BroadcastUDP : public QObject
{
  Q_OBJECT
public:
  explicit  BroadcastUDP(QObject *parent = 0);
  void      setupConnection();
  void      setHostAddress(QString hostAddress);
  void      setHostPort(quint16 hostPort);

signals:

public slots:

private:
  QUdpSocket* socket;
  QHostAddress _qHostAddress;
  quint16      _qHostPort;
};

#endif // BROADCASTUDP_H
