#include "SweepParameterPane.h"

#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

SweepParameterPane::SweepParameterPane()
    : BaseParameterPane()
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _wavelengthStepEdit(new DoubleLineEdit)
    , _blckSnapRateEdit(new PosIntLineEdit)
{
    // Parameter box -----------------------------------------------------------
    _parameterBox->setTitle(tr("Sweep mode"));

    int row=0;
    gui::putInGrid(_wavelength1Edit, _paramBoxLayout, row, tr("Wavelength 1"), "[nm]");
    gui::putInGrid(_wavelength2Edit, _paramBoxLayout, row, tr("Wavelength 2"), "[nm]");
    gui::putInGrid(_wavelengthStepEdit, _paramBoxLayout, row, tr("Increment"), "[nm]");
    gui::putInGrid(_blckSnapRateEdit, _paramBoxLayout, row, tr("Black snapshot rate"), "");

    BaseParameterPane::putInGrid(row);

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, parametersChanged);
    connect(_wavelength2Edit, LineEdit::textChanged, this, parametersChanged);
    connect(_wavelengthStepEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_blckSnapRateEdit, LineEdit::textChanged, this, parametersChanged);

    restore();
}

//------------------------------------------------------------------------------

void SweepParameterPane::updateState(bool isAppReady)
{
    BaseParameterPane::updateState(isAppReady);

    _wavelength1Edit->setEnabled(isAppReady);
    _wavelength2Edit->setEnabled(isAppReady);
    _wavelengthStepEdit->setEnabled(isAppReady);
    _blckSnapRateEdit->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

void SweepParameterPane::start(bool burst,
                               bool record,
                               double stabilisationTime,
                               const QString &session,
                               const QString &dataFolder)
{
    emit sweepRequested(_wavelength1Edit->value(),
                        _wavelength2Edit->value(),
                        _wavelengthStepEdit->value(),
                        _blckSnapRateEdit->value(),
                        _exposureEdit->value(),
                        _cooldownTimeEdit->value(),
                        _cooldownPwrEdit->value(),
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
           _blckSnapRateEdit->isValid() &&
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
    if (_blckSnapRateEdit->isValid())
        settings.setValue(blkSnapRateLbl, _blckSnapRateEdit->text());

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
    _blckSnapRateEdit->setText(settings.value(blkSnapRateLbl).toString());
    settings.endGroup();

    emit parametersChanged();
}

//------------------------------------------------------------------------------

}
