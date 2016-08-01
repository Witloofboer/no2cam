#ifndef SWEEPPANE_H
#define SWEEPPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

namespace gui {

class DoubleLineEdit;
class IntLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class SweepPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SweepPane(const core::Crystal &crystal);

signals:

public slots:

private:
    DoubleLineEdit *wavelength1Edit;
    DoubleLineEdit *wavelength2Edit;
    DoubleLineEdit *wavelengthIncEdit;
    IntLineEdit    *exposureEdit;
    IntLineEdit    *cooldownEdit;
    LineEdit       *sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // SWEEPPANE_H
