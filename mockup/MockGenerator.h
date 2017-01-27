#ifndef MOCKGENERATOR_H
#define MOCKGENERATOR_H

#include "core/Generator.h"

class MockGenerator : public core::AbstractGenerator
{
    Q_OBJECT

public:
    MockGenerator();

    virtual void setFrequency(double frequency) override;

};

#endif // MOCKGENERATOR_H
