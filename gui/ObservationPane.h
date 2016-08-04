#ifndef OBSERVATIONPANE_H
#define OBSERVATIONPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

namespace core {
class Crystal;
}

namespace gui {

class IntLineEdit;
class DoubleLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class ObservationPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit ObservationPane(const core::Crystal& crystal);

private:
    DoubleLineEdit *_wavelength1Edit;
    DoubleLineEdit *_wavelength2Edit;
    IntLineEdit    *_exposureEdit;
    IntLineEdit    *_snapPerObsEdit;
    IntLineEdit    *_cooldownEdit;
    LineEdit       *_sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // OBSERVATIONPANE_H
