#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include <QObject>

#include "core_global.h"

namespace core
{

class CORESHARED_EXPORT AbstractCamera : public QObject
{
    Q_OBJECT
public:
    AbstractCamera();

    void setExposure(int exposure);

    virtual void takeSnapshot()=0;
    virtual void stop()=0;

signals:
    void snapshotAvailable();

public slots:

protected:
    int _exposure;
};

}
#endif // ABSTRACTCAMERA_H
