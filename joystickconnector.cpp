#include "joystickconnector.h"
#include <QDebug>

InputUpdate::eState    InputUpdate::BtnState(unsigned char buttonID) const
{
    switch(buttonID)
    {
        case 1:
            return static_cast<eState>( _btnState & 0x01);
        case 2:
            return static_cast<eState>( _btnState & 0x02);
        case 3:
            return static_cast<eState>( _btnState & 0x04);
        case 4:
            return static_cast<eState>( _btnState & 0x08);
        case 5:
            return static_cast<eState>( _btnState & 0x10);
        case 6:
            return static_cast<eState>( _btnState & 0x20);
        case 7:
            return static_cast<eState>( _btnState & 0x40);
        case 8:
            return static_cast<eState>( _btnState & 0x80);

        default:
            return eOFF;
    }
}

JoystickConnector::JoystickConnector(QObject* parent)
    : QThread(parent), DEAD_ZONE(8000)
{

}

JoystickConnector::~JoystickConnector()
{

}

void    JoystickConnector::Quit()
{
    requestInterruption();

    // Push event to wake up so thread can exit
    SDL_Event sdlevent;
    sdlevent.type = SDL_KEYDOWN;
    sdlevent.key.keysym.sym = SDLK_1;

    SDL_PushEvent(&sdlevent);

    wait();
}

void    JoystickConnector::run(void)
{
    emit StatusUpdate( eOK, QString("Joystick thread initialized."));

    while( QThread::currentThread()->isInterruptionRequested() == false )
    {
        Initialize();

        SDL_Joystick* pGameController = SelectController();

        if( pGameController )
        {
            HandleController();

            SDL_JoystickClose( pGameController );
            pGameController = 0L;

            emit DeviceDisconnected();
            QThread::msleep(100);
        }
        else
        {
            QThread::msleep(200);
        }

        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }

    emit StatusUpdate( eOK, QString("Joystick thread terminated."));
}

void    JoystickConnector::Initialize(void)
{
    bool logOnce(true);

    while( QThread::currentThread()->isInterruptionRequested() == false)
    {
        if( SDL_InitSubSystem( SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER ) < 0 )
        {
            if( logOnce )
            {
                logOnce = false;
                emit StatusUpdate( eERROR, QString("SDL init failed. SDL Error: %1")
                                                         .arg( SDL_GetError()));
            }

            QThread::msleep(200);
            continue;
        }

        if( QThread::currentThread()->isInterruptionRequested() == false )
        {
            emit StatusUpdate( eOK, QString("Input system initialized."));
            break;
        }
    }
}

SDL_Joystick* JoystickConnector::SelectController(void)
{
    //Check for joysticks
    if( SDL_NumJoysticks() > 0 )
    {
        for( int Idx(0); Idx < SDL_NumJoysticks(); ++Idx)
        {
            SDL_Joystick* pGameController = SDL_JoystickOpen( Idx );
            //Load joystick
            if( pGameController != 0L )
            {
                emit DeviceConnected( SDL_JoystickName(pGameController));
                return pGameController;
            }
        }

        emit StatusUpdate( eERROR, QString("Unable to open game controlle. Found: %s").arg(SDL_NumJoysticks()) );
    }
    else
        emit StatusUpdate( eERROR, QString("Warning: No joysticks connected. Scanning") );

    return 0L;
}

void    JoystickConnector::HandleController(void)
{
    //Event handler
    SDL_Event event;

    while( QThread::currentThread()->isInterruptionRequested() == false )
    {
        //if( SDL_PollEvent( &event ) )
        if( SDL_WaitEvent(&event ) )
        {
            switch( event.type )
            {
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    OnControllerButtonEvent( event.cbutton );
                    break;

                case SDL_CONTROLLERAXISMOTION:
                    OnControllerAxisEvent( event.caxis );
                    break;

                case SDL_JOYAXISMOTION:
                    OnJoystickAxisEvent(event.jaxis);
                    break;

                case SDL_JOYBUTTONUP:
                case SDL_JOYBUTTONDOWN:
                    OnJoystickButtonEvent(event.jbutton);
                    break;

                case SDL_CONTROLLERDEVICEADDED:
                    AddControllerEvent( event.cdevice );
                    break;

                case SDL_CONTROLLERDEVICEREMOVED:
                    RemoveControllerEvent( event.cdevice );
                    break;

                case SDL_JOYDEVICEADDED:
                    AddJoystickEvent( event.jdevice );
                    break;

                case SDL_JOYDEVICEREMOVED:
                    RemoveJoystickEvent( event.jdevice );
                    break;

                default:
                    break;
            }
        }
    }
}

void    JoystickConnector::OnControllerButtonEvent( const SDL_ControllerButtonEvent& event )
{
    //qDebug() << "OnControllerButtonEvent";
}

void    JoystickConnector::OnControllerAxisEvent( const SDL_ControllerAxisEvent& event )
{
    //qDebug() << "OnControllerAxisEvent";
}

void    JoystickConnector::OnJoystickAxisEvent( const SDL_JoyAxisEvent& event)
{
    if( event.axis == SDL_CONTROLLER_AXIS_LEFTX )
    {
        if( qAbs<int>(event.value) < DEAD_ZONE)
            _currentState._axisLeft._x = 0;
        else
            _currentState._axisLeft._x = event.value;
    }
    else if( event.axis == SDL_CONTROLLER_AXIS_LEFTY )
    {
        if( qAbs<int>(event.value) < DEAD_ZONE)
            _currentState._axisLeft._y = 0;
        else
            _currentState._axisLeft._y = -event.value;
    }
    else if( event.axis == SDL_CONTROLLER_AXIS_RIGHTX )
    {
        if( qAbs<int>(event.value) < DEAD_ZONE)
            _currentState._axisRight._x = 0;
        else
            _currentState._axisRight._x = event.value;
    }
    else if(event.axis == SDL_CONTROLLER_AXIS_RIGHTY)
    {
        if( qAbs<int>(event.value) < DEAD_ZONE)
            _currentState._axisRight._y = 0;
        else
            _currentState._axisRight._y = -event.value;
    }

    emit DeviceUpdate( _currentState );
}

void    JoystickConnector::OnJoystickButtonEvent( const SDL_JoyButtonEvent& event)
{
    //qDebug() << "OnJoystickButtonEvent";
}

void    JoystickConnector::AddControllerEvent( const SDL_ControllerDeviceEvent& event )
{
    //qDebug() << "AddControllerEvent";
}

void    JoystickConnector::RemoveControllerEvent( const SDL_ControllerDeviceEvent& event )
{
    //qDebug() << "RemoveControllerEvent";
}

void    JoystickConnector::AddJoystickEvent( const SDL_JoyDeviceEvent& event)
{
    //qDebug() << "AddJoystickEvent";
}

void    JoystickConnector::RemoveJoystickEvent( const SDL_JoyDeviceEvent& event)
{
    //qDebug() << "RemoveJoystickEvent";
}

