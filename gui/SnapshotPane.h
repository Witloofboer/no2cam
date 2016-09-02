#ifndef SNAPSHOTPANE_H
#define SNAPSHOTPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace core {
class Crystal;
class AbstractCrysTempProbe;
}

namespace gui {

class DoubleLineEdit;
class IntLineEdit;

//------------------------------------------------------------------------------

class SnapshotPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SnapshotPane(MainWindow* mainWindow,
                          const core::Crystal *crystal,
                          core::AbstractCrysTempProbe *crysTempProbe,
                          const double &stabilisationTime);
    void persiste() const;

signals:
    void spectralSnapshot(double wavelength,
                          double exposure,
                          double cooldownTime,
                          double cooldownPwr,
                          double stabilisationTime,
                          bool burst,
                          const QString& session);

    void acousticSnapshot(double frequency,
                          double power,
                          double exposure,
                          double cooldownTime,
                          double cooldownPwr,
                          double stabilisationTime,
                          bool burst,
                          const QString& session);
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

    const core::Crystal *_crystal;
    const core::AbstractCrysTempProbe *_crystalTempProbe;

    QRadioButton   *_spectralBtn;
    QRadioButton   *_acousticBtn;
    DoubleLineEdit *_wavelengthEdit;
    DoubleLineEdit *_frequencyEdit;
    IntLineEdit    *_powerEdit;

    const double &_stabilisationTime;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTPANE_H
