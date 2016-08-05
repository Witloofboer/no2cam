#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "tooling.h"
#include "../core/Core.h"
#include "../core/core_lib.h"

namespace gui {

//------------------------------------------------------------------------------

ObservationPane::ObservationPane(const core::Crystal &crystal)
    : AbstractMainPane(crystal)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _exposureEdit(new IntLineEdit())
    , _snapPerObsEdit(new IntLineEdit)
    , _cooldownEdit(new IntLineEdit())
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(_wavelength1Edit, _paramBoxLayout, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(_wavelength2Edit, _paramBoxLayout, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(_exposureEdit, _paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(_snapPerObsEdit, _paramBoxLayout, row++, tr("Snapshots/obs"), "");
    putInGrid(_cooldownEdit, _paramBoxLayout, row++, tr("Cooldown/obs"), "[ms]");

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    _snapshotBox->setTitle(tr("Observation"));

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelength2Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_exposureEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_snapPerObsEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cooldownEdit, LineEdit::textChanged, this, refreshBtns);

    connect(this, observationRequested,
            core::singleton(), core::Core::startObservation);

    restore();
}

//------------------------------------------------------------------------------

void ObservationPane::start(bool burst, bool record)
{
     emit observationRequested(_wavelength1Edit->value(),
                               _wavelength2Edit->value(),
                               _exposureEdit->value(),
                               _snapPerObsEdit->value(),
                               _cooldownEdit->value(),
                               burst,
                               record ? _sessionEdit->text() : "",
                               _crystal);
}

//------------------------------------------------------------------------------

bool ObservationPane::areParametersValid() const
{
    return _wavelength1Edit->isValid() &&
           _wavelength2Edit->isValid() &&
           _snapPerObsEdit->isValid() &&
           _exposureEdit->isValid() &&
           _cooldownEdit->isValid();
}

//------------------------------------------------------------------------------

static const char *wavelength1Lbl = "Wavelength 1 [nm]";
static const char *wavelength2Lbl = "Wavelength 2 [nm]";
static const char *snapPerObsLbl = "Snapshops per observation [nm]";
static const char *exposureLbl = "exposure [ms]";
static const char *cooldownLbl = "cooldown [ms]";

void ObservationPane::persiste() const
{
    qInfo("Persisting observation parameters");

    QSettings settings;

    settings.beginGroup("Observations");
    if (_wavelength1Edit->isValid())
        settings.setValue(wavelength1Lbl, _wavelength1Edit->text());
    if (_wavelength2Edit->isValid())
        settings.setValue(wavelength2Lbl, _wavelength2Edit->text());
    if (_snapPerObsEdit->isValid())
        settings.setValue(snapPerObsLbl, _snapPerObsEdit->text());
    if (_exposureEdit->isValid())
        settings.setValue(exposureLbl, _exposureEdit->text());
    if (_cooldownEdit->isValid())
        settings.setValue(cooldownLbl, _cooldownEdit->text());
    settings.endGroup();

}

//------------------------------------------------------------------------------

void ObservationPane::restore()
{
    qInfo("Restoring observation parameters");

    QSettings settings;

    settings.beginGroup("Observations");
    _wavelength1Edit->setText(settings.value(wavelength1Lbl).toString());
    _wavelength2Edit->setText(settings.value(wavelength2Lbl).toString());
    _snapPerObsEdit->setText(settings.value(snapPerObsLbl).toString());
    _exposureEdit->setText(settings.value(exposureLbl).toString());
    _cooldownEdit->setText(settings.value(cooldownLbl).toString());
    settings.endGroup();

    refreshBtns();
}

//------------------------------------------------------------------------------

}
