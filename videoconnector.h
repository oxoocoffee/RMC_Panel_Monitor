#ifndef VIDEOCONNECTOR_H
#define VIDEOCONNECTOR_H

#include <QThread>

class VideoConnector : public QThread
{
    Q_OBJECT

    public:
        explicit VideoConnector(QObject* parent = 0L);
        virtual ~VideoConnector();

    public slots:

    signals:

    protected:
       void    run(void) Q_DECL_OVERRIDE;
};

#endif // VIDEOCONNECTOR_H
