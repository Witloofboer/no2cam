#ifndef SNAPSHOTPANE_H
#define SNAPSHOTPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace gui {

class DoubleLineEdit;
class IntLineEdit;

//------------------------------------------------------------------------------

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(MainWindow* mainWindow,
                          const core::Crystal& crystal);
    void persiste() const;

signals:
    void snapshotRequested(double wavelength,
                           double frequency,
                           double power,
                           double exposure,
                           double cooldown,
                           bool burst,
                           const QString& session,
                           const core::Crystal& _crystal);
public slots:
    void recomputeParams();

protected slots:
    void start(bool burst, bool record) override;

protected:
    bool areParametersValid() const override;

private slots:
    void switchMode();

private:
    void restore();

    QRadioButton   *_wavelengthBtn;
    QRadioButton   *_acousticBtn;
    DoubleLineEdit *_wavelengthEdit;
    DoubleLineEdit *_frequencyEdit;
    IntLineEdit    *_powerEdit;
    IntLineEdit    *_exposureEdit;
    IntLineEdit    *_cooldownEdit;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTPANE_H
