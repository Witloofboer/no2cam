#include "SnapshotPane.h"

#include <Qdebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QSettings>

#include "gui_lib.h"
#include "tooling.h"
#include "CameraBtnBox.h"
#include "MainWindow.h"
#include "../core/Crystal.h"
#include "../core/Core.h"
#include "../core/core_lib.h"

namespace gui {

//------------------------------------------------------------------------------

SnapshotPane::SnapshotPane(const core::Crystal& crystal)
    : AbstractMainPane(crystal)
    , _wavelengthBtn(new QRadioButton(tr("Optic")))
    , _acousticBtn(new QRadioButton(tr("Acoustic")))
    , _wavelengthEdit(new DoubleLineEdit)
    , _frequencyEdit(new DoubleLineEdit(9, 3, 3))
    , _powerEdit(new IntLineEdit(9, 4))
    , _exposureEdit(new IntLineEdit)
    , _cooldownEdit(new IntLineEdit)
{
    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(_wavelengthBtn);
    modeLayout->addWidget(_acousticBtn);

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(_wavelengthBtn, QRadioButton::toggled, this, switchMode);

    // Parameter box ------------------------------------------------------------

    int row=0;

    putInGrid(_frequencyEdit, _paramBoxLayout, row++, tr("Frequency"), "[MHz]");
    putInGrid(_powerEdit, _paramBoxLayout, row++, tr("Power"), "[mW]");
    putInGrid(_wavelengthEdit, _paramBoxLayout, row++, tr("Wavelength"), "[nm]");
    putInGrid(_exposureEdit, _paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(_cooldownEdit, _paramBoxLayout, row++, tr("Cooldown"), "[ms]");

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    _leftLayout->insertWidget(0, modeBox);
    _snapshotBox->setTitle(tr("Snapshot"));

    // Connectors
    connect(_wavelengthEdit, LineEdit::focusLost, this, recomputeParams);
    connect(_frequencyEdit, LineEdit::focusLost, this, recomputeParams);

    connect(_wavelengthBtn, QRadioButton::toggled, this, refreshBtns);
    connect(_frequencyEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_powerEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelengthEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_frequencyEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_exposureEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cooldownEdit, LineEdit::textChanged, this, refreshBtns);

    connect(this, snapshotRequested,
            core::singleton(), core::Core::startSnapshot);

    // Restoring
    restore();
    switchMode();
}

//------------------------------------------------------------------------------

void SnapshotPane::switchMode()
{
    if (_wavelengthBtn->isChecked())
    {
        _wavelengthEdit->setEnabled(true);
        _frequencyEdit->setEnabled(false);
        _powerEdit->setEnabled(false);
    }
    else
    {
        _wavelengthEdit->setEnabled(false);
        _frequencyEdit->setEnabled(true);
        _powerEdit->setEnabled(true);
    }

    recomputeParams();
}

//------------------------------------------------------------------------------

void SnapshotPane::recomputeParams()
{
    if (_wavelengthBtn->isChecked())
    {
        if (_wavelengthEdit->isValid())
        {
            _frequencyEdit->setValue(_crystal.frequency(_wavelengthEdit->value(), 20)); //TODO temperature
            _powerEdit->setValue(_crystal.power(_wavelengthEdit->value(), 20)); //TODO temperature
        } else {
            _frequencyEdit->setText("");
            _powerEdit->setText("");
        }
    } else {
        if (_frequencyEdit->isValid())
        {
            _wavelengthEdit->setValue(_crystal.wavelength(_frequencyEdit->value(), 20)); //TODO temperature
        } else {
            _wavelengthEdit->setText("");
        }
    }
}

//------------------------------------------------------------------------------

bool SnapshotPane::areParametersValid() const
{
    return _frequencyEdit->isValid() &&
           _powerEdit->isValid() &&
           _exposureEdit->isValid() &&
           _cooldownEdit->isValid();
}

//------------------------------------------------------------------------------

void SnapshotPane::start(bool burst, bool record)
{
     emit snapshotRequested(_wavelengthEdit->value(),
                            _frequencyEdit->value(),
                            _powerEdit->value(),
                            _exposureEdit->value(),
                            _cooldownEdit->value(),
                            burst,
                            record ? _sessionEdit->text() : "",
                            _crystal);
}

//------------------------------------------------------------------------------

static const char *wavelengthModeLbl = "Wavelength mode";
static const char *wavelengthLbl = "wavelength [nm]";
static const char *frequencyLbl = "frequency [MHz]";
static const char *powerLbl = "power [mW]";
static const char *exposureLbl = "exposure [ms]";
static const char *cooldownLbl = "cooldown [ms]";

void SnapshotPane::persiste() const
{
    qInfo("Persisting snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");
    settings.setValue(wavelengthModeLbl, _wavelengthBtn->isChecked());
    if (_wavelengthEdit->isValid())
        settings.setValue(wavelengthLbl, _wavelengthEdit->text());
    if (_frequencyEdit->isValid())
        settings.setValue(frequencyLbl, _frequencyEdit->text());
    if (_powerEdit->isValid())
        settings.setValue(powerLbl, _powerEdit->text());
    if (_exposureEdit->isValid())
        settings.setValue(exposureLbl, _exposureEdit->text());
    if (_cooldownEdit->isValid())
        settings.setValue(cooldownLbl, _cooldownEdit->text());
    settings.endGroup();
}

//------------------------------------------------------------------------------

void SnapshotPane::restore()
{
    qInfo("Restoring snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");
    if (settings.value(wavelengthModeLbl, true).toBool())
        _wavelengthBtn->setChecked(true);
    else
        _acousticBtn->setChecked(true);

    _wavelengthEdit->setText(settings.value(wavelengthLbl).toString());
    _frequencyEdit->setText(settings.value(frequencyLbl).toString());
    _powerEdit->setText(settings.value(powerLbl).toString());
    _exposureEdit->setText(settings.value(exposureLbl).toString());
    _cooldownEdit->setText(settings.value(cooldownLbl).toString());
    settings.endGroup();

    recomputeParams();
    refreshBtns();
}

//------------------------------------------------------------------------------

}
