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
    DoubleLineEdit *_wavelength1Edit;
    DoubleLineEdit *_wavelength2Edit;
    DoubleLineEdit *_wavelengthIncEdit;
    IntLineEdit    *_exposureEdit;
    IntLineEdit    *_cooldownEdit;
    LineEdit       *_sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // SWEEPPANE_H
