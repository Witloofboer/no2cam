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

namespace gui {

//------------------------------------------------------------------------------

SnapshotPane::SnapshotPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wavelengthBtn(new QRadioButton(tr("Optic")))
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
    modeLayout->addWidget(new QRadioButton(tr("Acoustic")));

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(wavelengthBtn, QRadioButton::toggled, this, switchParamMode);
    wavelengthBtn->setChecked(true);

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
    connect(wavelengthEdit, LineEdit::focusLost, this, recomputeParams);
    connect(frequencyEdit, LineEdit::focusLost, this, recomputeParams);
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

    emit recomputeParams();
}

//------------------------------------------------------------------------------

void SnapshotPane::recomputeParams()
{
    //TODO Not fully implemented
    if (wavelengthBtn->isChecked())
    {
        if (wavelengthEdit->isValid())
        {
            frequencyEdit->setText("xxx.xxx");
            powerEdit->setText("xxxx");
        } else {
            frequencyEdit->setText("");
            powerEdit->setText("");
        }
    } else {
        if (frequencyEdit->isValid())
        {
            wavelengthEdit->setText("xxx.x");
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

void SnapshotPane::persisteParams() const
{
    qInfo("Persisting snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");
    settings.setValue(wavelengthModeLbl, wavelengthBtn->isChecked());
    settings.setValue(wavelengthLbl, wavelengthEdit->value());
    settings.setValue(frequencyLbl, frequencyEdit->value());
    settings.setValue(powerLbl, powerEdit->value());
    settings.setValue(exposureLbl, exposureEdit->value());
    settings.setValue(cooldownLbl, cooldownEdit->value());
    settings.endGroup();
}

//------------------------------------------------------------------------------

void SnapshotPane::restoreParams()
{
    qInfo("Retreiving snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");
    wavelengthBtn->setChecked(settings.value(wavelengthModeLbl).toBool());
    wavelengthEdit->setValue(settings.value(wavelengthLbl).toDouble());
    frequencyEdit->setValue(settings.value(frequencyLbl).toDouble());
    powerEdit->setValue(settings.value(powerLbl).toDouble());
    exposureEdit->setValue(settings.value(exposureLbl).toInt());
    cooldownEdit->setValue(settings.value(cooldownLbl).toInt());
    settings.endGroup();

    emit recomputeParams();
}

//------------------------------------------------------------------------------

}
