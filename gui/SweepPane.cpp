#include "SweepPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "tooling.h"
#include "../core/Core.h"

namespace gui {

//------------------------------------------------------------------------------

SweepPane::SweepPane(MainWindow* mainWindow)
    : AbstractMainPane(mainWindow)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _wavelengthStepEdit(new DoubleLineEdit)
    , _exposureEdit(new IntLineEdit)
    , _cooldownEdit(new IntLineEdit)
{
    // Parameter box -----------------------------------------------------------
    int row=0;
    putInGrid(_wavelength1Edit, _paramBoxLayout, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(_wavelength2Edit, _paramBoxLayout, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(_wavelengthStepEdit, _paramBoxLayout, row++, tr("Increment"), "[nm]");
    putInGrid(_exposureEdit, _paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(_cooldownEdit, _paramBoxLayout, row++, tr("Cooldown"), "[ms]");

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    _snapshotBox->setTitle(tr("Snapshot (Sweep mode)"));

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelength2Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelengthStepEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_exposureEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cooldownEdit, LineEdit::textChanged, this, refreshBtns);

    restore();
}

//------------------------------------------------------------------------------

void SweepPane::start(bool burst, bool record)
{
    emit sweepRequested(_wavelength1Edit->value(),
                        _wavelength2Edit->value(),
                        _wavelengthStepEdit->value(),
                        _exposureEdit->value(),
                        _cooldownEdit->value(),
                        0, // TODO relaxtime
                        burst,
                        record ? _sessionEdit->text() : "");
}

//------------------------------------------------------------------------------

bool SweepPane::areParametersValid() const
{
    return _wavelength1Edit->isValid() &&
           _wavelength2Edit->isValid() &&
           _wavelengthStepEdit->isValid() &&
           _exposureEdit->isValid() &&
           _cooldownEdit->isValid();
}

//------------------------------------------------------------------------------

static const char *wavelength1Lbl = "Start wavelength [nm]";
static const char *wavelength2Lbl = "End wavelength [nm]";
static const char *wavelengthStepLbl = "Step [nm]";
static const char *exposureLbl = "exposure [ms]";
static const char *cooldownLbl = "cooldown [ms]";

void SweepPane::persiste() const
{
    qInfo("Persisting sweep parameters");

    QSettings settings;

    settings.beginGroup("Sweep");
    if (_wavelength1Edit->isValid())
        settings.setValue(wavelength1Lbl, _wavelength1Edit->text());
    if (_wavelength2Edit->isValid())
        settings.setValue(wavelength2Lbl, _wavelength2Edit->text());
    if (_wavelengthStepEdit->isValid())
        settings.setValue(wavelengthStepLbl, _wavelengthStepEdit->text());
    if (_exposureEdit->isValid())
        settings.setValue(exposureLbl, _exposureEdit->text());
    if (_cooldownEdit->isValid())
        settings.setValue(cooldownLbl, _cooldownEdit->text());
    settings.endGroup();

}

//------------------------------------------------------------------------------

void SweepPane::restore()
{
    qInfo("Restoring sweep parameters");

    QSettings settings;

    settings.beginGroup("Sweep");
    _wavelength1Edit->setText(settings.value(wavelength1Lbl).toString());
    _wavelength2Edit->setText(settings.value(wavelength2Lbl).toString());
    _wavelengthStepEdit->setText(settings.value(wavelengthStepLbl).toString());
    _exposureEdit->setText(settings.value(exposureLbl).toString());
    _cooldownEdit->setText(settings.value(cooldownLbl).toString());
    settings.endGroup();

    refreshBtns();
}

//------------------------------------------------------------------------------

}
