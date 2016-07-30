#ifndef SWEEPWIDGET_H
#define SWEEPWIDGET_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

namespace gui {

class DoubleLineEdit;
class IntLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class SweepingPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SweepingPane(QWidget *parent = 0);

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

#endif // SWEEPWIDGET_H
