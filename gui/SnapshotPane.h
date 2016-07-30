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
    void persisteParams() const;

signals:

public slots:
    void recomputeParams();

private slots:
    void switchParamMode();

private:

    void restoreParams();

    QRadioButton   *wavelengthBtn;
    DoubleLineEdit *wavelengthEdit;
    DoubleLineEdit *frequencyEdit;
    IntLineEdit    *powerEdit;
    IntLineEdit    *exposureEdit;
    IntLineEdit    *cooldownEdit;
    LineEdit       *sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTWIDGET_H
