#ifndef SNAPSHOTPANE_H
#define SNAPSHOTPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace core {
class Crystal;
}

#include "CameraButtonBox.h"

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
    void snapshot(double wavelength,
                  double frequency,
                  double power,
                  double exposure,
                  double cooldown,
                  bool burst,
                  const QString& session,
                  const core::Crystal& crystal);
    void stop();

public slots:
    void refreshParameters();

private slots:
    void switchParamMode();
    void refreshButtonsStatus();
    void snapshotRequested(bool burst, bool record);

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
