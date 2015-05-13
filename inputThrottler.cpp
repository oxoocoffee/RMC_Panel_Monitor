#include "inputThrottler.h"
#include <QDebug>

InputThrottler::InputThrottler(QObject* parent)
    : QThread(parent), _updated(false), _sleepRate(20)
{

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
