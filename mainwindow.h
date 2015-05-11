#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "inputmonitor.h"
#include "broadcastudp.h"

namespace Ui {
class MainWindow;
}

class InputMonitor;
class VideoConnector;
class JoystickConnector;
class BroadcastUDP;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

        void    Initialize();

    signals:
        void    UpdateRateChanged(unsigned int ms) ;

    private slots:
        void DeviceConnected(const QString& label);
        void DeviceDisconnected(void);
        void DeviceStatusUpdate(const InputMonitor::eStatus& status, const QString& message);
        void DeviceUpdate(const InputUpdate& state);

        void on_pushButtonConnect_clicked();
        void on_horizontalRateSlider_sliderReleased();
        void on_horizontalRateSlider_valueChanged(int value);

        void on_startTimeButton_clicked();

private:
        void CloseConnectors(void);

    private:
        Ui::MainWindow*     _ui;
        QLabel*             _labelHost;
        QLabel*             _labelHostName;
        QLabel*             _labelDevice;
        QLabel*             _labelDeviceName;
        InputMonitor*       _monitor;
        VideoConnector*     _videoConnector;
        JoystickConnector*  _joystickConnector;
        BroadcastUDP*       _udpBroadcaster;
};

#endif // MAINWINDOW_H
