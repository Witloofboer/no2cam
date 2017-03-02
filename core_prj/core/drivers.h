#ifndef DRIVERS_H
#define DRIVERS_H

#include <QObject>

#include "core_global.h"
#include "Snapshot.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT AcousticDriver : public QObject
{
    Q_OBJECT

public:
    AcousticDriver();

    virtual void set(double frequency, double power)=0;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT CameraDriver : public QObject
{
    Q_OBJECT

public:
    CameraDriver();

    virtual void setExposure(int exposure)=0;
    virtual void takeSnapshot()=0;
    virtual void stop()=0;
    virtual void getSnapshot(Snapshot &buffer)=0;

signals:
    void snapshotAvailable();
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT ThermometerDriver : public QObject
{
    Q_OBJECT

public:
    ThermometerDriver();

    virtual double getTemperature()=0;
};

//------------------------------------------------------------------------------

}

#endif // DRIVERS_H
