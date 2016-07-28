#ifndef OBSERVATIONWIDGET_H
#define OBSERVATIONWIDGET_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

namespace gui {

class IntLineEdit;
class DoubleLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class ObservationPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit ObservationPane(QWidget *parent = 0);

signals:

private:
    DoubleLineEdit *wavelength1Edit_;
    DoubleLineEdit *wavelength2Edit_;
    IntLineEdit    *exposureEdit_;
    IntLineEdit    *snapPerObsEdit_;
    IntLineEdit    *cooldownEdit_;
    LineEdit       *sessionEdit_;
};

//------------------------------------------------------------------------------

}

#endif // OBSERVATIONWIDGET_H
