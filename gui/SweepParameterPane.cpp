#include "SweepParameterPane.h"
#include "MainWindow.h"

#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

SweepParameterPane::SweepParameterPane(MainWindow *mainWindow)
    : BaseParameterPane(mainWindow)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _wavelengthStepEdit(new DoubleLineEdit)
    , _snapshotPerObs(new PosIntLineEdit)
{
    // Parameter box -----------------------------------------------------------
    _parameterBox->setTitle(tr("Sweep mode"));

    int row=0;
    gui::putInGrid(_wavelength1Edit, _paramBoxLayout, row, tr("Wavelength 1"), "[nm]");
    gui::putInGrid(_wavelength2Edit, _paramBoxLayout, row, tr("Wavelength 2"), "[nm]");
    gui::putInGrid(_wavelengthStepEdit, _paramBoxLayout, row, tr("Increment"), "[nm]");
    gui::putInGrid(_snapshotPerObs, _paramBoxLayout, row, tr("Snapshots/obs"), "");

    BaseParameterPane::putInGrid(row);

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, parametersChanged);
    connect(_wavelength2Edit, LineEdit::textChanged, this, parametersChanged);
    connect(_wavelengthStepEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_snapshotPerObs, LineEdit::textChanged, this, parametersChanged);

    restore();
}

//------------------------------------------------------------------------------

void SweepParameterPane::updateState(bool isAppReady)
{
    BaseParameterPane::updateState(isAppReady);

    _wavelength1Edit->setEnabled(isAppReady);
    _wavelength2Edit->setEnabled(isAppReady);
    _wavelengthStepEdit->setEnabled(isAppReady);
    _snapshotPerObs->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

void SweepParameterPane::start(bool burst,
                               bool record,
                               double stabilisationTime,
                               const QString &session,
                               const QString &dataFolder)
{
    const double wl1 = _wavelength1Edit->value();
    const double wl2 = _wavelength2Edit->value();
    const double dwl = _wavelengthStepEdit->value();

    if (wl1 >= wl2)
    {
        _mainWindow->onDisplayWarning(QString("<p><b>Error</b>: the 1st wavelength is not smaller than the 2nd one.</p>"));
        _mainWindow->onUpdateApplicationReadiness(true);
        return;
    }

    if (dwl <= 0)
    {
        _mainWindow->onDisplayWarning(QString("<p><b>Error</b>: the wavelength increment must be positive.</p>"));
        _mainWindow->onUpdateApplicationReadiness(true);
        return;
    }


    emit sweepRequested(wl1, wl2, dwl,
                        _snapshotPerObs->value(),
                        _exposureEdit->value(),
                        _cooldownTimeEdit->value(),
                        stabilisationTime,
                        burst,
                        record,
                        dataFolder,
                        session);
}

//------------------------------------------------------------------------------

bool SweepParameterPane::areParametersValid() const
{
    return _wavelength1Edit->isValid() &&
           _wavelength2Edit->isValid() &&
           _wavelengthStepEdit->isValid() &&
           _snapshotPerObs->isValid() &&
           BaseParameterPane::areParametersValid();
}

//------------------------------------------------------------------------------

static const char *wavelength1Lbl = "Start wavelength [nm]";
static const char *wavelength2Lbl = "End wavelength [nm]";
static const char *wavelengthStepLbl = "Step [nm]";
static const char *blkSnapRateLbl = "Black Snapshot rate";

void SweepParameterPane::persiste() const
{
    qDebug("Persisting sweep parameters");

    QSettings settings;

    settings.beginGroup("Sweep");

    BaseParameterPane::persiste(settings);

    if (_wavelength1Edit->isValid())
        settings.setValue(wavelength1Lbl, _wavelength1Edit->text());
    if (_wavelength2Edit->isValid())
        settings.setValue(wavelength2Lbl, _wavelength2Edit->text());
    if (_wavelengthStepEdit->isValid())
        settings.setValue(wavelengthStepLbl, _wavelengthStepEdit->text());
    if (_snapshotPerObs->isValid())
        settings.setValue(blkSnapRateLbl, _snapshotPerObs->text());

    settings.endGroup();

}

//------------------------------------------------------------------------------

void SweepParameterPane::restore()
{
    qDebug("Restoring sweep parameters");

    QSettings settings;

    settings.beginGroup("Sweep");

    BaseParameterPane::restore(settings);

    _wavelength1Edit->setText(settings.value(wavelength1Lbl).toString());
    _wavelength2Edit->setText(settings.value(wavelength2Lbl).toString());
    _wavelengthStepEdit->setText(settings.value(wavelengthStepLbl).toString());
    _snapshotPerObs->setText(settings.value(blkSnapRateLbl).toString());
    settings.endGroup();

    emit parametersChanged();
}

//------------------------------------------------------------------------------

}
