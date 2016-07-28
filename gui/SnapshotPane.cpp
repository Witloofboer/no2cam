#include "SnapshotPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>

#include "tooling.h"

SnapshotPane::SnapshotPane(QWidget *parent)
    : AbstractMainPane(parent)
    , selectorBtn_(new QRadioButton(tr("Optic")))
    , wavelengthEdit_(new DoubleLineEdit)
    , frequencyEdit_(new DoubleLineEdit(9, 3, 3))
    , powerEdit_(new IntLineEdit(7, 4))
    , exposureEdit_(new IntLineEdit)
    , cooldownEdit_(new IntLineEdit)
    , sessionEdit_(new LineEdit)
{
    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(selectorBtn_);
    modeLayout->addWidget(new QRadioButton(tr("Acoustic")));

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(selectorBtn_, QRadioButton::toggled, this, SnapshotPane::parameterModeSwitch);
    selectorBtn_->setChecked(true);

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
}

void SnapshotPane::parameterModeSwitch()
{
    if (selectorBtn_->isChecked())
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
}
