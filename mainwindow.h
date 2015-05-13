#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include "joystickconnector.h"
#include "inputThrottler.h"
#include "broadcastudp.h"

namespace Ui {
class MainWindow;
}

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
        void DeviceUpdate(const InputUpdate& state);

        void NetworkMessageTrace(const BroadcastUDP::eDirection dir,
                                 const QString& message);

        void StatusUpdate(const eStatus& status,
                          const QString& message);

        void on_pushButtonConnect_clicked();
        void on_horizontalRateSlider_sliderReleased();
        void on_horizontalRateSlider_valueChanged(int value);

        void on_pushButtonLog_clicked();

    private:
        void LogTrace(const QString& message);
        void CloseConnectors(void);
        void OpenNetworkConnection(void);
        void CloseNetworkConnection(void);

    private:
        Ui::MainWindow*     _ui;
        QLabel*             _labelHost;
        QLabel*             _labelHostName;
        QLabel*             _labelDevice;
        QLabel*             _labelDeviceName;
        QFile*              _logger;
        QTextStream*        _textStreamLogger;
        InputThrottler*     _inputThrottler;
        VideoConnector*     _videoConnector;
        JoystickConnector*  _joystickConnector;
        BroadcastUDP*       _udpBroadcaster;
};

#endif // MAINWINDOW_H
