#ifndef MOCKGENERATOR_H
#define MOCKGENERATOR_H

#include "core/AbstractGenerator.h"

class MockGenerator : public core::AbstractGenerator
{
    Q_OBJECT

public:
    MockGenerator();

    virtual void stop() override;
    virtual void setFrequency(double frequency) override;

};

#endif // MOCKGENERATOR_H
