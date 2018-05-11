#ifndef SWEEPPARAMETERPANE_H
#define SWEEPPARAMETERPANE_H

#include "BaseParameterPane.h"

//------------------------------------------------------------------------------

namespace gui {

class DoubleLineEdit;
class PosIntLineEdit;
class MainWindow;

//------------------------------------------------------------------------------

class SweepParameterPane : public BaseParameterPane
{
    Q_OBJECT
public:
    explicit SweepParameterPane(MainWindow *mainWindow);
    void updateState(bool isAppReady);
    void persiste() const;

    void start(bool burst,
               bool record,
               double refWavelength,
               double exposureFactor,
               double stabilisationTime,
               const QString &session,
               const QString &dataFolder) override;

signals:
    void sweepRequested(double wavelength1,
                        double wavelength2,
                        double wavelengthStep,
                        int    blackSnapshotRate,
                        double exposure,
                        double cooldownTime,
                        double refWavelength,
                        double exposureFactor,
                        double stabilisationTime,
                        bool burst,
                        bool record,
                        QString dataFolder,
                        QString session);

protected:
    bool areParametersValid() const override;

private:
    void restore();

    DoubleLineEdit *_wavelength1Edit;
    DoubleLineEdit *_wavelength2Edit;
    DoubleLineEdit *_wavelengthStepEdit;
    PosIntLineEdit *_blckSnapRateEdit;
};

//------------------------------------------------------------------------------

}

#endif // SWEEPPARAMETERPANE_H
