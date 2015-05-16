#include "statsMonitor.h"

StatsMonitor::StatsMonitor(QObject* parent)
: QThread(parent)
{

}

void    StatsMonitor::ResetStats()
{
    _mutex.lock();
        _stats.Resert();
    _mutex.unlock();
}

void    StatsMonitor::run(void)
{
    emit StatusUpdate( eOK, QString("Stats Monitor thread initialized"));

    while( QThread::currentThread()->isInterruptionRequested() == false )
    {

        emit StatsUpdate(_stats);

        QThread::msleep(1000);
    }

    emit StatusUpdate( eOK, QString("Stats Monitor thread terminated"));
}
