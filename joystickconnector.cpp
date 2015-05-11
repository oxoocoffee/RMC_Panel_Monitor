#include "joystickconnector.h"

JoystickConnector::JoystickConnector(QObject* parent)
  : QThread(parent)
{

}

JoystickConnector::~JoystickConnector()
{

}

void    JoystickConnector::run(void)
{
    while( QThread::currentThread()->isInterruptionRequested() == false )
    {

    }
}
