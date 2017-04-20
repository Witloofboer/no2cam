#ifndef DRIVERS_H
#define DRIVERS_H

#include <QObject>

#include "core_global.h"
#include "Snapshot.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseAcousticDriver : public QObject
{
    Q_OBJECT

public:
    BaseAcousticDriver();
    virtual ~BaseAcousticDriver() {}

    virtual void set(double frequency, double power)=0;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseCameraDriver : public QObject
{
    Q_OBJECT

public:
    BaseCameraDriver();
    virtual ~BaseCameraDriver() {}

    virtual void setExposure(int exposure)=0;
    virtual void takeSnapshot()=0;
    virtual void stop()=0;

signals:
    void snapshotAvailable(const Snapshot &buffer);
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseThermometerDriver : public QObject
{
    Q_OBJECT

public:
    BaseThermometerDriver();
    virtual ~BaseThermometerDriver() {}

    virtual double getTemperature()=0;
};

//------------------------------------------------------------------------------

}

#endif // DRIVERS_H
