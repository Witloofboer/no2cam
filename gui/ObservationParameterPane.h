#ifndef OBSERVATIONPARAMETERPANE_H
#define OBSERVATIONPARAMETERPANE_H

#include "BaseParameterPane.h"

//------------------------------------------------------------------------------

namespace gui {

class IntLineEdit;
class DoubleLineEdit;
class MainWindow;

//------------------------------------------------------------------------------

class ObservationParameterPane : public BaseParameterPane
{
    Q_OBJECT
public:
    explicit ObservationParameterPane(MainWindow *mainWindow);
    void updateState(bool isAppReady);
    void persiste() const;

    void start(bool burst,
               bool record,
               double stabilisationTime,
               const QString& session,
               const QString& dataFolder) override;

signals:
    void observationRequested(double wavelength1,
                              double wavelength2,
                              double exposure,
                              int snapshotPerObs,
                              double cooldownTime,
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
    IntLineEdit    *_snapPerObsEdit;
};

//------------------------------------------------------------------------------

}

#endif // OBSERVATIONPARAMETERPANE_H
