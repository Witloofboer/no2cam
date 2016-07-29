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
    , wavelengthBtn_(new QRadioButton(tr("Optic")))
    , wavelengthEdit_(new DoubleLineEdit)
    , frequencyEdit_(new DoubleLineEdit(9, 3, 3))
    , powerEdit_(new IntLineEdit(9, 4))
    , exposureEdit_(new IntLineEdit)
    , cooldownEdit_(new IntLineEdit)
    , sessionEdit_(new LineEdit)
{

    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(wavelengthBtn_);
    modeLayout->addWidget(new QRadioButton(tr("Acoustic")));

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(wavelengthBtn_, QRadioButton::toggled, this, switchParamMode);
    wavelengthBtn_->setChecked(true);

    // Parameter box ------------------------------------------------------------

    int row=0;

    putInGrid(frequencyEdit_, paramBoxLayout_, row++, tr("Frequency"), "[MHz]");
    putInGrid(powerEdit_, paramBoxLayout_, row++, tr("Power"), "[mW]");
    putInGrid(wavelengthEdit_, paramBoxLayout_, row++, tr("Wavelength"), "[nm]");
    putInGrid(exposureEdit_, paramBoxLayout_, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldownEdit_, paramBoxLayout_, row++, tr("Cooldown"), "[ms]");

    paramBoxLayout_->addWidget(new QLabel("Session:"), row, 0);
    paramBoxLayout_->addWidget(sessionEdit_, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    leftLayout_->insertWidget(0, modeBox);
    snapshotBox_->setTitle(tr("Snapshot"));

    // Connectors
    connect(wavelengthEdit_, LineEdit::focusLost, this, recomputeParams);
    connect(frequencyEdit_, LineEdit::focusLost, this, recomputeParams);
}

//------------------------------------------------------------------------------

void SnapshotPane::switchParamMode()
{
    if (wavelengthBtn_->isChecked())
    {
        wavelengthEdit_->setEnabled(true);
        frequencyEdit_->setEnabled(false);
        powerEdit_->setEnabled(false);
    }
    else
    {
        wavelengthEdit_->setEnabled(false);
        frequencyEdit_->setEnabled(true);
        powerEdit_->setEnabled(true);
    }

    emit recomputeParams();
}

//------------------------------------------------------------------------------

void SnapshotPane::recomputeParams()
{
    //TODO Not fully implemented
    if (wavelengthBtn_->isChecked())
    {
        if (wavelengthEdit_->isValid())
        {
            frequencyEdit_->setText("xxx.xxx");
            powerEdit_->setText("xxxx");
        } else {
            frequencyEdit_->setText("");
            powerEdit_->setText("");
        }
    } else {
        if (frequencyEdit_->isValid())
        {
            wavelengthEdit_->setText("xxx.x");
        } else {
            wavelengthEdit_->setText("");
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
    settings.setValue(wavelengthModeLbl, wavelengthBtn_->isChecked());
    settings.setValue(wavelengthLbl, wavelengthEdit_->value());
    settings.setValue(frequencyLbl, frequencyEdit_->value());
    settings.setValue(powerLbl, powerEdit_->value());
    settings.setValue(exposureLbl, exposureEdit_->value());
    settings.setValue(cooldownLbl, cooldownEdit_->value());
    settings.endGroup();
}

//------------------------------------------------------------------------------

void SnapshotPane::restoreParams()
{
    qInfo("Retreiving snapshot parameters");

    QSettings settings;

    settings.beginGroup("Snapshot");
    wavelengthBtn_->setChecked(settings.value(wavelengthModeLbl).toBool());
    wavelengthEdit_->setValue(settings.value(wavelengthLbl).toDouble());
    frequencyEdit_->setValue(settings.value(frequencyLbl).toDouble());
    powerEdit_->setValue(settings.value(powerLbl).toDouble());
    exposureEdit_->setValue(settings.value(exposureLbl).toInt());
    cooldownEdit_->setValue(settings.value(cooldownLbl).toInt());
    settings.endGroup();

    emit recomputeParams();
}

//------------------------------------------------------------------------------

}
