#ifndef SNAPSHOTPANE_H
#define SNAPSHOTPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace core {
class Crystal;
}

namespace gui {

class DoubleLineEdit;
class IntLineEdit;
class LineEdit;

//------------------------------------------------------------------------------

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(const core::Crystal& crystal);
    void persiste() const;

signals:

public slots:
    void refreshParameters();

private slots:
    void switchParamMode();

private:

    void restore();
    QRadioButton   *wavelengthBtn;
    QRadioButton   *acousticBtn;
    DoubleLineEdit *wavelengthEdit;
    DoubleLineEdit *frequencyEdit;
    IntLineEdit    *powerEdit;
    IntLineEdit    *exposureEdit;
    IntLineEdit    *cooldownEdit;
    LineEdit       *sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTPANE_H
