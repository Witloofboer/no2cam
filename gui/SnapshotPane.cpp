#include "SnapshotPane.h"

#include <Qdebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QSettings>

#include "gui_lib.h"
#include "tooling.h"
#include "MainWindow.h"
#include "../core/Crystal.h"

namespace gui {

//------------------------------------------------------------------------------

SnapshotPane::SnapshotPane(const core::Crystal& crystal)
    : AbstractMainPane(crystal)
    , wavelengthBtn(new QRadioButton(tr("Optic")))
    , acousticBtn(new QRadioButton(tr("Acoustic")))
    , wavelengthEdit(new DoubleLineEdit)
    , frequencyEdit(new DoubleLineEdit(9, 3, 3))
    , powerEdit(new IntLineEdit(9, 4))
    , exposureEdit(new IntLineEdit)
    , cooldownEdit(new IntLineEdit)
    , sessionEdit(new LineEdit)
{
    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(wavelengthBtn);
    modeLayout->addWidget(acousticBtn);

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(wavelengthBtn, QRadioButton::toggled, this, switchParamMode);

    // Parameter box ------------------------------------------------------------

    int row=0;

    putInGrid(frequencyEdit, paramBoxLayout, row++, tr("Frequency"), "[MHz]");
    putInGrid(powerEdit, paramBoxLayout, row++, tr("Power"), "[mW]");
    putInGrid(wavelengthEdit, paramBoxLayout, row++, tr("Wavelength"), "[nm]");
    putInGrid(exposureEdit, paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldownEdit, paramBoxLayout, row++, tr("Cooldown"), "[ms]");

    paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    paramBoxLayout->addWidget(sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    leftLayout->insertWidget(0, modeBox);
    snapshotBox->setTitle(tr("Snapshot"));

    // Connectors
    connect(wavelengthEdit, LineEdit::focusLost, this, refreshParameters);
    connect(frequencyEdit, LineEdit::focusLost, this, refreshParameters);

    // Restoring
    restore();
    switchParamMode();
}

//------------------------------------------------------------------------------

void SnapshotPane::switchParamMode()
{
    if (wavelengthBtn->isChecked())
    {
        wavelengthEdit->setEnabled(true);
        frequencyEdit->setEnabled(false);
        powerEdit->setEnabled(false);
    }
    else
    {
        wavelengthEdit->setEnabled(false);
        frequencyEdit->setEnabled(true);
        powerEdit->setEnabled(true);
    }

    emit refreshParameters();
}

//------------------------------------------------------------------------------

void SnapshotPane::refreshParameters()
{
    if (wavelengthBtn->isChecked())
    {
        if (wavelengthEdit->isValid())
        {
            frequencyEdit->setValue(crystal.frequency(wavelengthEdit->value(), 20)); //TODO temperature
            powerEdit->setValue(crystal.power(wavelengthEdit->value(), 20)); //TODO temperature
        } else {
            frequencyEdit->setText("");
            powerEdit->setText("");
        }
    } else {
        if (frequencyEdit->isValid())
        {
            wavelengthEdit->setValue(crystal.wavelength(frequencyEdit->value(), 20)); //TODO temperature
        } else {
            wavelengthEdit->setText("");
        }
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
    settings.setValue(wavelengthModeLbl, wavelengthBtn->isChecked());
    if (wavelengthEdit->isValid())
        settings.setValue(wavelengthLbl, wavelengthEdit->text());
    if (frequencyEdit->isValid())
        settings.setValue(frequencyLbl, frequencyEdit->text());
    if (powerEdit->isValid())
        settings.setValue(powerLbl, powerEdit->text());
    if (exposureEdit->isValid())
        settings.setValue(exposureLbl, exposureEdit->text());
    if (cooldownEdit->isValid())
        settings.setValue(cooldownLbl, cooldownEdit->text());
    settings.endGroup();
}

//------------------------------------------------------------------------------

void SnapshotPane::restore()
{
    qInfo("Restoring snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");
    if (settings.value(wavelengthModeLbl, true).toBool())
        wavelengthBtn->setChecked(true);
    else
        acousticBtn->setChecked(true);

    wavelengthEdit->setText(settings.value(wavelengthLbl).toString());
    frequencyEdit->setText(settings.value(frequencyLbl).toString());
    powerEdit->setText(settings.value(powerLbl).toString());
    exposureEdit->setText(settings.value(exposureLbl).toString());
    cooldownEdit->setText(settings.value(cooldownLbl).toString());
    settings.endGroup();

    emit refreshParameters();
}

//------------------------------------------------------------------------------

}
