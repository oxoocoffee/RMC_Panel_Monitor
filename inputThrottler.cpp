#include "inputThrottler.h"
#include <QBitArray>
#include <QDebug>

InputThrottler::InputThrottler(QObject* parent)
    : QThread(parent), _updated(false), _sleepRate(20)
{
    _byteArray.reserve(2);
}

InputThrottler::~InputThrottler()
{

}

void    InputThrottler::run(void)
{
    emit StatusUpdate( eOK, QString("Input Throttler thread initialized"));

    while( QThread::currentThread()->isInterruptionRequested() == false )
    {
        _lock.lock();

        if(_updated )
        {
            _updated = false;
            _lock.unlock();

            _byteArray.clear();

            _byteArray[0] = (_state.AxisLeft().Y() / JOY_PER_MSG_SCALAR);
            _byteArray[1] = (_state.AxisLeft().X() / JOY_PER_MSG_SCALAR);

            PrintBits();

            //qDebug() << "AxisLeft X " << (_state.AxisLeft().X() / JOY_PER_MSG_SCALAR);
            //qDebug() << "AxisLeft Y " << (_state.AxisLeft().Y() / JOY_PER_MSG_SCALAR);

            emit PublishMessage(_byteArray);
        }
        else
            _lock.unlock();

        QThread::msleep(_sleepRate);
    }

    emit StatusUpdate( eOK, QString("Input Throttler thread terminated"));
}

void InputThrottler::DeviceUpdate(const InputUpdate& state)
{
    _lock.lock();

    _state   = state;
    _updated = true;

    _lock.unlock();
}

void    InputThrottler::UpdateRateChanged(unsigned int ms)
{
    if( ms > 1000)
        _sleepRate = 1000;
    else if (ms < 1 )
        _sleepRate = 1;
    else
        _sleepRate = ms;
}

void    InputThrottler::PrintBits()
{
    QBitArray bits(_byteArray.count()*8);

    // Convert from QByteArray to QBitArray
    for(int i=0; i < _byteArray.count(); ++i)
    {
        for(int b=0; b<8;b)
        {
            bits.setBit( i*8+b, _byteArray.at(i)&(1<<(7-b)) );
        }
    }
}
