#ifndef GENERATOR_H
#define GENERATOR_H

#include "core/AbstractGenerator.h"

class Generator : public core::AbstractGenerator
{
    Q_OBJECT

public:
    Generator();

    virtual void stop() override;
    virtual void setFrequency(double frequency) override;

    double getFrequency() const {return _frequency;}

private:
    double _frequency;
};

#endif // GENERATOR_H
