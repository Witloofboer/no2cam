#ifndef DOASPARAMETERPANE_H
#define DOASPARAMETERPANE_H

#include "BaseParameterPane.h"

#include <QVector>

//------------------------------------------------------------------------------

class QPushButton;

namespace gui {

class LineEdit;
class IntLineEdit;
class DoubleLineEdit;
class PosIntLineEdit;
class MainWindow;

//------------------------------------------------------------------------------

class DoasParameterPane : public BaseParameterPane
{
    Q_OBJECT
public:
    explicit DoasParameterPane(MainWindow *mainWindow);
    void updateState(bool isAppReady);
    void persiste() const;

    void start(bool burst,
               bool record,
               double stabilisationTime,
               const QString &session,
               const QString &dataFolder) override;

signals:
    void doasRequested(QVector<double> wavelengths,
                       int snapshotPerObs,
                       double exposure,
                       double cooldownTime,
                       double stabilisationTime,
                       bool burst,
                       bool record,
                       QString dataFolder,
                       QString session);

private slots:
    void selectWavelengthFile();

protected:
    bool areParametersValid() const override;

private:
    void restore();

    IntLineEdit *_snapPerObsEdit;
    LineEdit *_wlFileEdit;
    QPushButton *_wlFileBtn;
};

//------------------------------------------------------------------------------

}

#endif // DOASPARAMETERPANE_H
