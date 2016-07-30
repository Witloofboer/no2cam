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
    DoubleLineEdit *wavelength1Edit;
    DoubleLineEdit *wavelength2Edit;
    IntLineEdit    *exposureEdit;
    IntLineEdit    *snapPerObsEdit;
    IntLineEdit    *cooldownEdit;
    LineEdit       *sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // OBSERVATIONWIDGET_H
