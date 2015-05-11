#ifndef JOYSTICKCONNECTOR_H
#define JOYSTICKCONNECTOR_H

#include <QThread>

class JoystickConnector : public QThread
{
    Q_OBJECT

    public:
        explicit JoystickConnector(QObject* parent = 0L);
        virtual ~JoystickConnector();

    public slots:

    signals:

    protected:
       void    run(void) Q_DECL_OVERRIDE;
};

#endif // JOYSTICKCONNECTOR_H
