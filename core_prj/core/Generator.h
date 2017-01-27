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
    virtual void setFrequency(double frequency)=0;
};

//------------------------------------------------------------------------------

class GeneratorProxy
{
public:
    explicit GeneratorProxy(AbstractGenerator* generator);
    bool setFrequency(double frequency);

private:
    AbstractGenerator *_generator;
    double _frequency;
};

//------------------------------------------------------------------------------

}

#endif // ABSTRACTGENERATOR_H
