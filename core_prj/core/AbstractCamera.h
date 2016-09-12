#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

const int snapSize = 2048;
typedef double Snapshot[snapSize][snapSize] ;

class CORESHARED_EXPORT AbstractCamera : public QObject
{
    Q_OBJECT

public:
    explicit AbstractCamera();

    void setExposure(int exposure);

    virtual void stop()=0;

    virtual void copySnapshot(core::Snapshot &buffer)=0;

signals:
    void snapshotAvailable();

public slots:
    virtual void takeSnapshot()=0;

protected:
    int _exposure;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTCAMERA_H
