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
    DoubleLineEdit *wavelength1Edit_;
    DoubleLineEdit *wavelength2Edit_;
    DoubleLineEdit *wavelengthIncEdit_;
    IntLineEdit    *exposureEdit_;
    IntLineEdit    *cooldownEdit_;
    LineEdit       *sessionEdit_;
};

//------------------------------------------------------------------------------

}

#endif // SWEEPWIDGET_H
