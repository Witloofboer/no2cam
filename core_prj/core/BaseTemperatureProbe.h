#ifndef BASETEMPERATUREPPROBE_H
#define BASETEMPERATUREPPROBE_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseTemperatureProbe : public QObject
{
    Q_OBJECT

public:
    explicit BaseTemperatureProbe();
    double getTemperature();

protected:
    virtual void readProbe() = 0;
    double _temperature;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockTemperatureProbe : public BaseTemperatureProbe
{
public:
    MockTemperatureProbe();

protected:
    void readProbe() override;

private:
    double _delta;
};

//------------------------------------------------------------------------------

}

#endif // BASETEMPERATUREPPROBE_H
