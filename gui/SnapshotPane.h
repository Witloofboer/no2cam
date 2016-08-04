#ifndef SNAPSHOTPANE_H
#define SNAPSHOTPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace core {
class Crystal;
}

#include "CameraBtnBox.h"

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
                  const core::Crystal& _crystal);

public slots:
    void recomputeParams();

private slots:
    void switchMode();
    void refreshBtns();
    void start(bool burst, bool record);

private:
    void restore();
    QRadioButton   *_wavelengthBtn;
    QRadioButton   *_acousticBtn;
    DoubleLineEdit *_wavelengthEdit;
    DoubleLineEdit *_frequencyEdit;
    IntLineEdit    *_powerEdit;
    IntLineEdit    *_exposureEdit;
    IntLineEdit    *_cooldownEdit;
    LineEdit       *_sessionEdit;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTPANE_H
