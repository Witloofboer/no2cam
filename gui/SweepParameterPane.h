#ifndef SWEEPPARAMETERPANE_H
#define SWEEPPARAMETERPANE_H

#include "BaseParameterPane.h"

//------------------------------------------------------------------------------

namespace gui {

class DoubleLineEdit;

//------------------------------------------------------------------------------

class SweepParameterPane : public BaseParameterPane
{
    Q_OBJECT
public:
    explicit SweepParameterPane();
    void updateState(bool isAppReady);
    void persiste() const;

    void start(bool burst,
               bool record,
               double stabilisationTime,
               QString session) override;

signals:
    void sweepRequested(double wavelength1,
                        double wavelength2,
                        double wavelengthStep,
                        double exposure,
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
    DoubleLineEdit *_wavelengthStepEdit;
};

//------------------------------------------------------------------------------

}

#endif // SWEEPPARAMETERPANE_H
