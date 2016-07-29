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

    QRadioButton   *wavelengthBtn_;
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
