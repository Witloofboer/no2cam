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

signals:

private:
    DoubleLineEdit *wavelength1Edit;
    DoubleLineEdit *wavelength2Edit;
    IntLineEdit    *exposureEdit;
    IntLineEdit    *snapPerObsEdit;
    IntLineEdit    *cooldownEdit;
    LineEdit       *sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // OBSERVATIONPANE_H
