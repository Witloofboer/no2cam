#ifndef MOCKDRIVER_H
#define MOCKDRIVER_H

//------------------------------------------------------------------------------

#include "core/AbstractDriver.h"

//------------------------------------------------------------------------------

class MockDriver : public core::AbstractDriver
{
    Q_OBJECT

public:
    MockDriver();
};

//------------------------------------------------------------------------------

#endif // MOCKDRIVER_H
