#ifndef SWEEPPANE_H
#define SWEEPPANE_H

#include "AbstractMainPane.h"

//------------------------------------------------------------------------------

namespace gui {

class DoubleLineEdit;
class IntLineEdit;

//------------------------------------------------------------------------------

class SweepPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit SweepPane(MainWindow *mainWindow,
                       const double &stabilisationTime);
    void persiste() const;

signals:
    void sweepRequested(double wavelength1,
                        double wavelength2,
                        double wavelengthStep,
                        double exposure,
                        double cooldown,
                        double stabilisationTime,
                        bool burst,
                        const QString& session);

protected slots:
    void start(bool burst, bool record) override;

protected:
    bool areParametersValid() const override;

private:
    void restore();

    DoubleLineEdit *_wavelength1Edit;
    DoubleLineEdit *_wavelength2Edit;
    DoubleLineEdit *_wavelengthStepEdit;
    IntLineEdit    *_exposureEdit;
    IntLineEdit    *_cooldownEdit;

    const double &_stabilisationTime;
};

//------------------------------------------------------------------------------

}

#endif // SWEEPPANE_H
