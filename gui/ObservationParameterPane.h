#ifndef OBSERVATIONPARAMETERPANE_H
#define OBSERVATIONPARAMETERPANE_H

#include "BaseParameterPane.h"

//------------------------------------------------------------------------------

namespace gui {

class IntLineEdit;
class DoubleLineEdit;

//------------------------------------------------------------------------------

class ObservationParameterPane : public BaseParameterPane
{
    Q_OBJECT
public:
    explicit ObservationParameterPane();
    void updateState(bool isAppReady);
    void persiste() const;

    void start(bool burst,
               bool record,
               double stabilisationTime,
               QString session) override;

signals:
    void observationRequested(double wavelength1,
                              double wavelength2,
                              double exposure,
                              int snapshotPerObs,
                              double cooldownTime,
                              double cooldownPwr,
                              double stabilisationTime,
                              bool burst,
                              bool record,
                              const QString& session);

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
