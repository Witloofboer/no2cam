#ifndef SNAPSHOTWIDGET_H
#define SNAPSHOTWIDGET_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace gui {

class DoubleLineEdit;
class IntLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(QWidget *parent = 0);

signals:

private slots:
    void parameterModeSwitch();

private:
    QRadioButton   *selectorBtn_;
    DoubleLineEdit *wavelengthEdit_;
    DoubleLineEdit *frequencyEdit_;
    IntLineEdit    *powerEdit_;
    IntLineEdit    *exposureEdit_;
    IntLineEdit    *cooldownEdit_;
    LineEdit       *sessionEdit_;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTWIDGET_H
