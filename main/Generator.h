#ifndef GENERATOR_H
#define GENERATOR_H

#include "core/drivers.h"

class Generator : public core::FrequencyDriver
{
    Q_OBJECT

public:
    Generator();

    void setFrequency(double frequency) override;

    double getFrequency() const {
        return _frequency;
    }

private:
    double _frequency;
};

#endif // GENERATOR_H
