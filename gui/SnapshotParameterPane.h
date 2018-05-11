#ifndef SNAPSHOTPARAMETERPANE_H
#define SNAPSHOTPARAMETERPANE_H

#include "BaseParameterPane.h"

//------------------------------------------------------------------------------

class QRadioButton;

namespace core {
class Crystal;
class BaseTemperatureProbe; // should be set via a signal
}

namespace gui {

class DoubleLineEdit;
class IntLineEdit;
class MainWindow;

//------------------------------------------------------------------------------

class SnapshotParameterPane : public BaseParameterPane
{
    Q_OBJECT
public:
    explicit SnapshotParameterPane(MainWindow *mainWindow,
                                   const core::Crystal *crystal);
    void updateState(bool isAppReady);
    void persiste() const;

    void start(bool burst,
               bool record,
               double refWavelength,
               double exposureFactor,
               double stabilisationTime,
               const QString& session,
               const QString& dataFolder) override;

    void updateTemperature(double temperature);

signals:
    void opticalSnapshot(double wavelength,
                         double exposure,
                         double cooldownTime,
                         double refWavelength,
                         double exposureFactor,
                         double stabilisationTime,
                         bool burst,
                         bool record,
                         QString dataFolder,
                         QString session);

    void acousticSnapshot(double frequency,
                          double power,
                          double exposure,
                          double cooldownTime,
                          double stabilisationTime,
                          bool burst,
                          bool record,
                          QString dataFolder,
                          QString session);
public slots:
    void recomputeParams();

protected:
    bool areParametersValid() const override;

private slots:
    void enableFieldsWrtMode();

private:
    void restore();

    const core::Crystal *_crystal;
    const core::BaseTemperatureProbe *_crystalTempProbe;

    double _temperature;
    QRadioButton   *_spectralBtn;
    QRadioButton   *_acousticBtn;
    DoubleLineEdit *_wavelengthEdit;
    DoubleLineEdit *_frequencyEdit;
    IntLineEdit    *_powerEdit;
};

//------------------------------------------------------------------------------

}

#endif // SNAPSHOTPARAMETERPANE_H
