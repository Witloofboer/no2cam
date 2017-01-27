#ifndef ABSTRACTDRIVER_H
#define ABSTRACTDRIVER_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT AbstractDriver : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDriver();
    virtual void setPower(double power)=0;
};

//------------------------------------------------------------------------------

class DriverProxy
{
public:
    explicit DriverProxy(AbstractDriver* driver);
    bool setPower(double power);

private:
    AbstractDriver *_driver;
    double _power;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTDRIVER_H
