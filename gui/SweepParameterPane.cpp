#include "SweepParameterPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "tooling.h"
#include "../core/Core.h"

namespace gui {

//------------------------------------------------------------------------------

SweepParameterPane::SweepParameterPane(MainWindow* mainWindow,
                                       const double &stabilisationTime)
    : BaseParameterPane(mainWindow)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _wavelengthStepEdit(new DoubleLineEdit)
    , _stabilisationTime(stabilisationTime)
{
    // Parameter box -----------------------------------------------------------
    _parameterBox->setTitle(tr("Sweep mode"));

    int row=0;
    gui::putInGrid(_wavelength1Edit, _paramBoxLayout, row, tr("Wavelength 1"), "[nm]");
    gui::putInGrid(_wavelength2Edit, _paramBoxLayout, row, tr("Wavelength 2"), "[nm]");
    gui::putInGrid(_wavelengthStepEdit, _paramBoxLayout, row, tr("Increment"), "[nm]");

    BaseParameterPane::putInGrid(row);

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelength2Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelengthStepEdit, LineEdit::textChanged, this, refreshBtns);

    restore();
}

//------------------------------------------------------------------------------

void SweepParameterPane::updateState(bool isAppReady)
{
    BaseParameterPane::updateState(isAppReady);

    _wavelength1Edit->setEnabled(isAppReady);
    _wavelength2Edit->setEnabled(isAppReady);
    _wavelengthStepEdit->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

void SweepParameterPane::start(bool burst, bool record)
{
    emit sweepRequested(_wavelength1Edit->value(),
                        _wavelength2Edit->value(),
                        _wavelengthStepEdit->value(),
                        _exposureEdit->value(),
                        _cooldownTimeEdit->value(),
                        _cooldownPwrEdit->value(),
                        _stabilisationTime,
                        burst,
                        record ? _sessionEdit->text() : "");
}

//------------------------------------------------------------------------------

bool SweepParameterPane::areParametersValid() const
{
    return _wavelength1Edit->isValid() &&
           _wavelength2Edit->isValid() &&
           _wavelengthStepEdit->isValid() &&
           BaseParameterPane::areParametersValid();
}

//------------------------------------------------------------------------------

static const char *wavelength1Lbl = "Start wavelength [nm]";
static const char *wavelength2Lbl = "End wavelength [nm]";
static const char *wavelengthStepLbl = "Step [nm]";

void SweepParameterPane::persiste() const
{
    qInfo("Persisting sweep parameters");

    QSettings settings;

    settings.beginGroup("Sweep");

    BaseParameterPane::persiste(settings);

    if (_wavelength1Edit->isValid())
        settings.setValue(wavelength1Lbl, _wavelength1Edit->text());
    if (_wavelength2Edit->isValid())
        settings.setValue(wavelength2Lbl, _wavelength2Edit->text());
    if (_wavelengthStepEdit->isValid())
        settings.setValue(wavelengthStepLbl, _wavelengthStepEdit->text());

    settings.endGroup();

}

//------------------------------------------------------------------------------

void SweepParameterPane::restore()
{
    qInfo("Restoring sweep parameters");

    QSettings settings;

    settings.beginGroup("Sweep");

    BaseParameterPane::restore(settings);

    _wavelength1Edit->setText(settings.value(wavelength1Lbl).toString());
    _wavelength2Edit->setText(settings.value(wavelength2Lbl).toString());
    _wavelengthStepEdit->setText(settings.value(wavelengthStepLbl).toString());
    settings.endGroup();

    refreshBtns();
}

//------------------------------------------------------------------------------

}
