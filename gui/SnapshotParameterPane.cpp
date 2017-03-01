#include "SnapshotParameterPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QSettings>

#include "tooling.h"

#include "../core/Crystal.h" // TODO temperature remove this

namespace gui {

//------------------------------------------------------------------------------

SnapshotParameterPane::SnapshotParameterPane(const core::Crystal *crystal)
    : BaseParameterPane()
    , _crystal(crystal)
    , _temperature(20.0) // Initial value, will be updated
    , _spectralBtn(new QRadioButton(tr("Optic")))
    , _acousticBtn(new QRadioButton(tr("Acoustic")))
    , _wavelengthEdit(new DoubleLineEdit)
    , _frequencyEdit(new DoubleLineEdit(9, 3, 3))
    , _powerEdit(new IntLineEdit(9, 4))
{

    // Parameter box ------------------------------------------------------------
    _parameterBox->setTitle(tr("Snapshot mode"));

    int row=0;
    _paramBoxLayout->addWidget(_spectralBtn, row, 0);
    _paramBoxLayout->addWidget(_acousticBtn, row, 1, 1, 2);
    ++row;

    gui::putInGrid(_frequencyEdit, _paramBoxLayout, row, tr("Frequency"), "[MHz]");
    gui::putInGrid(_powerEdit, _paramBoxLayout, row, tr("Power"), "[mW]");
    gui::putInGrid(_wavelengthEdit, _paramBoxLayout, row, tr("Wavelength"), "[nm]");

    BaseParameterPane::putInGrid(row);

    // Connectors
    connect(_wavelengthEdit, LineEdit::focusLost, this, recomputeParams);
    connect(_frequencyEdit, LineEdit::focusLost, this, recomputeParams);

    connect(_spectralBtn, QRadioButton::toggled, this, enableFieldsWrtMode);
    connect(_spectralBtn, QRadioButton::toggled, this, parametersChanged);
    connect(_frequencyEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_powerEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_wavelengthEdit, LineEdit::textChanged, this, parametersChanged);

    // Restoring
    restore();
    enableFieldsWrtMode();
}

//------------------------------------------------------------------------------

void SnapshotParameterPane::updateState(bool isAppReady)
{
    BaseParameterPane::updateState(isAppReady);

    if (isAppReady)
    {
        enableFieldsWrtMode();
    } else {
        _wavelengthEdit->setEnabled(false);
        _frequencyEdit->setEnabled(false);
        _powerEdit->setEnabled(false);
    }
}

//------------------------------------------------------------------------------

void SnapshotParameterPane::enableFieldsWrtMode()
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

void SnapshotParameterPane::recomputeParams()
{
    if (_spectralBtn->isChecked())
    {
        if (_wavelengthEdit->isValid())
        {
            double freq, power;
            _crystal->computeFreqPow(_wavelengthEdit->value(),
                                     _temperature, freq, power);

            _frequencyEdit->setValue(freq);
            _powerEdit->setValue(power);
        } else {
            _frequencyEdit->setText("");
            _powerEdit->setText("");
        }
    } else {
        if (_frequencyEdit->isValid())
        {
            _wavelengthEdit->setValue(_crystal->wavelength(_frequencyEdit->value(),
                                      _temperature));
        } else {
            _wavelengthEdit->setText("");
        }
    }
}

//------------------------------------------------------------------------------

void SnapshotParameterPane::updateTemperature(double temperature)
{
    _temperature = temperature;
    recomputeParams();
}

//------------------------------------------------------------------------------

bool SnapshotParameterPane::areParametersValid() const
{
    return _frequencyEdit->isValid() &&
           _powerEdit->isValid() &&
           BaseParameterPane::areParametersValid();
}

//------------------------------------------------------------------------------

void SnapshotParameterPane::start(bool burst,
                                  bool record,
                                  double stabilisationTime,
                                  const QString &session,
                                  const QString &dataFolder)
{
    if (_spectralBtn->isChecked())
    {
        emit opticalSnapshot(_wavelengthEdit->value(),
                              _exposureEdit->value(),
                              _cooldownTimeEdit->value(),
                              stabilisationTime,
                              burst,
                              record,
                              dataFolder,
                              session);

    } else {
        emit acousticSnapshot(_frequencyEdit->value(),
                              _powerEdit->value(),
                              _exposureEdit->value(),
                              _cooldownTimeEdit->value(),
                              stabilisationTime,
                              burst,
                              record,
                              dataFolder,
                              session);
    }
}

//------------------------------------------------------------------------------

static const char *wavelengthModeLbl = "Wavelength mode";
static const char *wavelengthLbl = "wavelength [nm]";
static const char *frequencyLbl = "frequency [MHz]";
static const char *powerLbl = "power [mW]";

void SnapshotParameterPane::persiste() const
{
    qDebug("Persisting snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");

    BaseParameterPane::persiste(settings);

    settings.setValue(wavelengthModeLbl, _spectralBtn->isChecked());
    if (_wavelengthEdit->isValid())
        settings.setValue(wavelengthLbl, _wavelengthEdit->text());
    if (_frequencyEdit->isValid())
        settings.setValue(frequencyLbl, _frequencyEdit->text());
    if (_powerEdit->isValid())
        settings.setValue(powerLbl, _powerEdit->text());

    settings.endGroup();
}

//------------------------------------------------------------------------------

void SnapshotParameterPane::restore()
{
    qDebug("Restoring snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");

    BaseParameterPane::restore(settings);

    if (settings.value(wavelengthModeLbl, true).toBool())
        _spectralBtn->setChecked(true);
    else
        _acousticBtn->setChecked(true);

    _wavelengthEdit->setText(settings.value(wavelengthLbl).toString());
    _frequencyEdit->setText(settings.value(frequencyLbl).toString());
    _powerEdit->setText(settings.value(powerLbl).toString());

    settings.endGroup();

    recomputeParams();
    emit parametersChanged();
}

//------------------------------------------------------------------------------

}
