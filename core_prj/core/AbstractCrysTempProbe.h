#ifndef ABSTRACTCRYSTEMPPROBE_H
#define ABSTRACTCRYSTEMPPROBE_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT AbstractCrysTempProbe : public QObject
{
    Q_OBJECT

public:
    explicit AbstractCrysTempProbe();
    virtual double getTemperature()=0;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTCRYSTEMPPROBE_H
