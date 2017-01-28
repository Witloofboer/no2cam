#ifndef BASEGENERATOR_H
#define BASEGENERATOR_H

#include <QObject>

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core
{

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BaseGenerator : public QObject
{
    Q_OBJECT

public:
    explicit BaseGenerator();
    bool setFrequency(double frequency);

protected:
    virtual void updateFrequency()=0;
    double _frequency;
};

//------------------------------------------------------------------------------

class CORESHARED_EXPORT MockGenerator : public BaseGenerator
{
    Q_OBJECT

public:
    explicit MockGenerator();

protected:
    void updateFrequency() override;
};

//------------------------------------------------------------------------------

}

#endif // BASEGENERATOR_H
