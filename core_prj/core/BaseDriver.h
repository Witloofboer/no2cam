#ifndef BASEDRIVER_H
#define BASEDRIVER_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseDriver : public QObject
{
    Q_OBJECT

public:
    explicit BaseDriver();
    bool setPower(double power);

protected:
    virtual void updatePower()=0;
    double _power;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockDriver : public BaseDriver
{
    Q_OBJECT

public:
    explicit MockDriver();

protected:
    virtual void updatePower() override;
};

//------------------------------------------------------------------------------

}

#endif // BASEDRIVER_H
