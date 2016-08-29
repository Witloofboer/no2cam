#include "SnapshotPane.h"

#include <Qdebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QSettings>

#include "tooling.h"
#include "CameraBtnBox.h"
#include "MainWindow.h"
#include "ConfigurationDlg.h"

#include "../core/Crystal.h"
#include "../core/Core.h"

namespace gui {

//------------------------------------------------------------------------------

SnapshotPane::SnapshotPane(MainWindow* mainWindow,
                           const core::Crystal *crystal,
                           core::AbstractCrysTempProbe *crysTempProbe,
                           const double &stabilisationTime)
    : AbstractMainPane(mainWindow)
    , _crystal(crystal)
    , _crystalTempProbe(crysTempProbe)
    , _spectralBtn(new QRadioButton(tr("Optic")))
    , _acousticBtn(new QRadioButton(tr("Acoustic")))
    , _wavelengthEdit(new DoubleLineEdit)
    , _frequencyEdit(new DoubleLineEdit(9, 3, 3))
    , _powerEdit(new IntLineEdit(9, 4))
    , _exposureEdit(new IntLineEdit)
    , _cooldownEdit(new IntLineEdit)
    , _stabilisationTime(stabilisationTime)
{
    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(_spectralBtn);
    modeLayout->addWidget(_acousticBtn);

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(_spectralBtn, QRadioButton::toggled, this, switchMode);

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

    connect(_spectralBtn, QRadioButton::toggled, this, refreshBtns);
    connect(_frequencyEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_powerEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelengthEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_frequencyEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_exposureEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cooldownEdit, LineEdit::textChanged, this, refreshBtns);

    // Restoring
    restore();
    switchMode();
}

//------------------------------------------------------------------------------

void SnapshotPane::switchMode()
{
    if (_spectralBtn->isChecked())
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
    if (_spectralBtn->isChecked())
    {
        if (_wavelengthEdit->isValid())
        {
            double freq, power;
            _crystal->computeFreqPow(_wavelengthEdit->value(), 20, freq, power); // TODO temperature;

            _frequencyEdit->setValue(freq);
            _powerEdit->setValue(power);
        } else {
            _frequencyEdit->setText("");
            _powerEdit->setText("");
        }
    } else {
        if (_frequencyEdit->isValid())
        {
            _wavelengthEdit->setValue(_crystal->wavelength(_frequencyEdit->value(), 20)); //TODO temperature
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
    if (_spectralBtn->isChecked())
    {
        emit spectralSnapshot(_wavelengthEdit->value(),
                              _exposureEdit->value(),
                              _cooldownEdit->value(),
                              _stabilisationTime,
                              burst,
                              record ? _sessionEdit->text() : "");
    } else {
        emit acousticSnapshot(_frequencyEdit->value(),
                              _powerEdit->value(),
                              _exposureEdit->value(),
                              _cooldownEdit->value(),
                              _stabilisationTime,
                              burst,
                              record ? _sessionEdit->text() : "");
    }
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
    settings.setValue(wavelengthModeLbl, _spectralBtn->isChecked());
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
        _spectralBtn->setChecked(true);
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
