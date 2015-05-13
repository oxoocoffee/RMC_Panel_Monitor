#ifndef INPUTTHROTTLER_H
#define INPUTTHROTTLER_H

#include <joystickconnector.h>
#include <QString>
#include <QMutex>

#define JOY_RANGE               32767
#define BITS_PER_MSG_WHEELE     4               // 3 + signe bit
#define JOY_PER_MSG_SCALAR      (short)(JOY_RANGE / 7)

class InputThrottler : public QThread
{
    Q_OBJECT
    public:
        explicit InputThrottler(QObject* parent = 0L);
        virtual ~InputThrottler();

    public slots:
        void    DeviceUpdate(const InputUpdate& state);
        void    UpdateRateChanged(unsigned int ms);

    signals:
        void    StatusUpdate(const eStatus& status, const QString& message);
        void    PublishMessage(const QByteArray& buffer);

    private:
        void    PrintBits();

    private:
        InputUpdate _state;
        QByteArray  _byteArray;
        QMutex      _lock;
        bool        _updated;
        int         _sleepRate;

    protected:
       void    run(void) Q_DECL_OVERRIDE;
};

#endif // INPUTTHROTTLER_H
