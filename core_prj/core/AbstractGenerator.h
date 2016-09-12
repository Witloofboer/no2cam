#ifndef ABSTRACTGENERATOR_H
#define ABSTRACTGENERATOR_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT AbstractGenerator : public QObject
{
    Q_OBJECT

public:
    explicit AbstractGenerator();
    virtual void stop()=0;
    virtual void setFrequency(double frequency)=0;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTGENERATOR_H