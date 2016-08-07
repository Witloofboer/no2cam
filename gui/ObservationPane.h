#ifndef OBSERVATIONPANE_H
#define OBSERVATIONPANE_H

#include "AbstractMainPane.h"
#include "../core/Core.h"

//------------------------------------------------------------------------------

namespace gui {

class IntLineEdit;
class DoubleLineEdit;
class MainWindow;

//------------------------------------------------------------------------------

class ObservationPane : public AbstractMainPane
{
    Q_OBJECT
public:
    explicit ObservationPane(MainWindow *mainWindow);
    void persiste() const;

signals:
    void observationRequested(double wavelength1,
                 double wavelength2,
                 double exposure,
                 int snapshotPerObs,
                 double cooldown,
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
    IntLineEdit    *_exposureEdit;
    IntLineEdit    *_snapPerObsEdit;
    IntLineEdit    *_cooldownEdit;
};

//------------------------------------------------------------------------------

}

#endif // OBSERVATIONPANE_H
