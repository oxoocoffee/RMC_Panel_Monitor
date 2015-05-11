#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoconnector.h"
#include "joystickconnector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _monitor(0L), _videoConnector(0L), _joystickConnector(0L), _udpBroadcaster(0L)
{
    _ui->setupUi(this);

    _labelHost     = new QLabel("Host: ", this);
    _labelHostName = new QLabel("<b>Disconnected</b>", this);

    _labelDevice     = new QLabel(" Device: ", this);
    _labelDeviceName = new QLabel("<b>Scanning...</b>", this);

    _ui->statusBar->addPermanentWidget(_labelHost);
    _ui->statusBar->addPermanentWidget(_labelHostName);
    _ui->statusBar->addPermanentWidget(_labelDevice);
    _ui->statusBar->addPermanentWidget(_labelDeviceName, 1);

    _ui->lcdRateNumber->display( _ui->horizontalRateSlider->value());

    _ui->pushButtonConnect->setStyleSheet("color: red");
}

MainWindow::~MainWindow()
{
    if( _monitor )
    {
        _monitor->requestInterruption();
        _monitor->wait();
        delete _monitor;
    }

    CloseConnectors();

    if( _ui )
        delete _ui;
}

void MainWindow::Initialize()
{
    if( _monitor )
        return;

    qRegisterMetaType<InputMonitor::eStatus>("InputMonitor::eStatus");
    qRegisterMetaType<InputUpdate>("InputUpdate");

    _monitor = new InputMonitor(this);
    _udpBroadcaster = new BroadcastUDP();

    connect(_monitor, SIGNAL(DeviceConnected(const QString&)), this, SLOT(DeviceConnected(const QString&)));
    connect(_monitor, SIGNAL(DeviceDisconnected(void)), this, SLOT(DeviceDisconnected(void)));
    connect(_monitor, SIGNAL(DeviceStatusUpdate(const InputMonitor::eStatus&, const QString&)), this, SLOT(DeviceStatusUpdate(const InputMonitor::eStatus&, const QString&)));
    connect(_monitor, SIGNAL(DeviceUpdate(const InputUpdate&)), this, SLOT(DeviceUpdate(const InputUpdate&)));

    connect( this, SIGNAL(UpdateRateChanged(unsigned int)), _monitor, SLOT(UpdateRateChanged(unsigned int)));

    _monitor->start();
}

void MainWindow::DeviceConnected(const QString& label)
{
    _labelDeviceName->setText( QString("<b>%1</b>").arg(label));
}

void MainWindow::DeviceDisconnected(void)
{
    _labelDeviceName->setText("<b>Scanning...</b>");
}

void MainWindow::DeviceStatusUpdate(const InputMonitor::eStatus& status, const QString& message)
{

}

void MainWindow::DeviceUpdate(const InputUpdate& state)
{
    _ui->labelJoyXLeftValue->setText(QString::number( state.AxisLeft().X()));
    _ui->labelJoyYLeftValue->setText(QString::number( state.AxisLeft().Y()));
}

void MainWindow::on_horizontalRateSlider_sliderReleased()
{
    _ui->lcdRateNumber->display( _ui->horizontalRateSlider->value());
}

void MainWindow::on_horizontalRateSlider_valueChanged(int value)
{
    _ui->lcdRateNumber->display(value);

    emit UpdateRateChanged(value);
}

void MainWindow::on_pushButtonConnect_clicked()
{
    QString host_ip   = _ui->hostAddressTextBox->text();
    QString host_port = _ui->spinBoxControlPort->text();

    _udpBroadcaster->setHostAddress(host_ip);
    _udpBroadcaster->setHostPort(host_port.toInt());
//    _udpBroadcaster->setupConnection(QHostAddress::QHostAddress(host_ip), host_port.toInt());
    _udpBroadcaster->setupConnection();


    _ui->pushButtonConnect->setStyleSheet("color: red");
    _ui->pushButtonConnect->setText("Disconnect");
    _ui->pushButtonConnect->setStyleSheet("color: green");

    _labelHostName->setText("<b>Attempting to connect...</b>");
    _ui->statusBar->update();
}

void MainWindow::CloseConnectors(void)
{
     if( _videoConnector )
     {
         _videoConnector->requestInterruption();
         _videoConnector->wait();
         delete _videoConnector;
     }

     if( _joystickConnector )
     {
         _joystickConnector->requestInterruption();
         _joystickConnector->wait();
         delete _joystickConnector;
     }
}


// Function for Timer start button
void MainWindow::on_startTimeButton_clicked()
{

}
