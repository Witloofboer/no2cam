#ifndef DRIVERS_H
#define DRIVERS_H

#include <QObject>

#include "core_global.h"
#include "Snapshot.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT FrequencyDriver : public QObject
{
    Q_OBJECT

public:
    FrequencyDriver();

    virtual void setFrequency(double frequency)=0;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT PowerDriver : public QObject
{
    Q_OBJECT

public:
    PowerDriver();

    virtual void setPower(double power)=0;
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

class CORESHARED_EXPORT ProbeDriver : public QObject
{
    Q_OBJECT

public:
    ProbeDriver();

    virtual double getTemperature()=0;
};

//------------------------------------------------------------------------------

}

#endif // DRIVERS_H
