#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoconnector.h"
#include "joystickconnector.h"
#include <QDateTime>
#include <QStandardItemModel>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _ui(new Ui::MainWindow),
    _videoConnector(0L), _joystickConnector(0L), _udpBroadcaster(0L)
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

    _ui->pushButtonConnect->setStyleSheet("color: green");
}

MainWindow::~MainWindow()
{
    CloseConnectors();

    if( _ui )
        delete _ui;
}

void MainWindow::Initialize()
{
    if( _joystickConnector )
        return;

    qRegisterMetaType<JoystickConnector::eStatus>("JoystickConnector::eStatus");
    qRegisterMetaType<InputUpdate>("InputUpdate");

    _logger = new QFile("EDTPanel.log");
    _ui->pushButtonLog->setStyleSheet("color: green");

    _udpBroadcaster  = new BroadcastUDP(this);

    connect(_udpBroadcaster, SIGNAL(NetworkMessageTrace(const BroadcastUDP::eDirection, const QString&)),
                                      this, SLOT(NetworkMessageTrace(const BroadcastUDP::eDirection, const QString&)));

    _joystickConnector = new JoystickConnector(this);

    connect(_joystickConnector, SIGNAL(DeviceConnected(const QString&)),
                                       this, SLOT(DeviceConnected(const QString&)));

    connect(_joystickConnector, SIGNAL(DeviceDisconnected(void)),
                                       this, SLOT(DeviceDisconnected(void)));

    connect(_joystickConnector, SIGNAL(DeviceStatusUpdate(const JoystickConnector::eStatus&, const QString&)),
                                       this, SLOT(DeviceStatusUpdate(const JoystickConnector::eStatus&, const QString&)));

    connect(_joystickConnector, SIGNAL(DeviceUpdate(const InputUpdate&)),
                                       this, SLOT(DeviceUpdate(const InputUpdate&)));

    connect( this, SIGNAL(UpdateRateChanged(unsigned int)), _joystickConnector,
                                       SLOT(UpdateRateChanged(unsigned int)));

    _joystickConnector->start();
}

void MainWindow::DeviceConnected(const QString& label)
{
    _labelDeviceName->setText( QString("<b>%1</b>").arg(label));
}

void MainWindow::DeviceDisconnected(void)
{
    _labelDeviceName->setText("<b>Scanning...</b>");
}

void MainWindow::DeviceStatusUpdate(const JoystickConnector::eStatus& status, const QString& message)
{
    QString msg = QDateTime::currentDateTime().toString("hh:mm:ss");

    if( status == JoystickConnector::eOK )
        msg += " OK  - ";
    else
        msg += " ERR - ";

    msg +=  message;

    LogTrace(msg);
}

void MainWindow::DeviceUpdate(const InputUpdate& state)
{
    _ui->labelJoyXLeftValue->setText(QString::number( state.AxisLeft().X()));
    _ui->labelJoyYLeftValue->setText(QString::number( state.AxisLeft().Y()));

    _ui->labelJoyXRightValue->setText(QString::number( state.AxisRight().X()));
    _ui->labelJoyYRightValue->setText(QString::number( state.AxisRight().Y()));
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
    try
    {
        if( _ui->pushButtonConnect->isChecked() )
            OpenNetworkConnection();
        else
            CloseNetworkConnection();
    }
    catch( std::runtime_error& err)
    {
        QMessageBox msgBox;
        msgBox.setText(err.what());
        msgBox.setInformativeText("ERROR!!!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
}

void MainWindow::OpenNetworkConnection()
{
    if( _udpBroadcaster->IsConnected() == false )
    {
        _udpBroadcaster->Connect(_ui->hostAddressTextBox->text(), (quint16)_ui->spinBoxControlPort->value() );
        _ui->pushButtonConnect->setStyleSheet("color: red");
    }
}

void MainWindow::CloseNetworkConnection()
{
    if( _udpBroadcaster->IsConnected() )
    {
        _udpBroadcaster->Disconnect();
        _ui->pushButtonConnect->setStyleSheet("color: green");
    }
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

     if( _udpBroadcaster )
         _udpBroadcaster->Disconnect();
}

void    MainWindow::LogTrace(const QString& message)
{
    if( _logger->isOpen() == false )
        return;

    QTextStream textStreamLogger(_logger);

    textStreamLogger << "-- " << message;
}

void    MainWindow::NetworkMessageTrace(const BroadcastUDP::eDirection dir,
                                        const QString& message)
{
    if( _logger->isOpen() == false )
        return;

    QTextStream textStreamLogger(_logger);

    if( dir == BroadcastUDP::eIn )
        textStreamLogger << "<- " << message;
    else
        textStreamLogger << "-> " << message;
}

void MainWindow::on_pushButtonLog_clicked()
{
    if( _ui->pushButtonLog->isChecked() )
    {
        _ui->pushButtonLog->setStyleSheet("color: red");
        _ui->pushButtonLog->setText("Trace On");
        _logger->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

         QTextStream textStreamLogger(_logger);
         textStreamLogger << "++++ Opened ++++\n";
    }
    else
    {
        QTextStream textStreamLogger(_logger);
        textStreamLogger << "---- Closed ----\n";
        _logger->close();
        _ui->pushButtonLog->setText("Trace Off");
        _ui->pushButtonLog->setStyleSheet("color: green");
    }
}
