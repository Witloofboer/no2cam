#include "SnapshotPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

#include "tooling.h"

SnapshotPane::SnapshotPane(QWidget *parent)
    : AbstractMainPane(parent)
    , opticBtn_(new QRadioButton(tr("Optic")))
    , wl_(new_WavelengthEdit())
    , aF_(new DoubleLineEdit("009.999"))
    , aP_(new DoubleLineEdit(" 0009"))
    , eet_(new_EetEdit())
    , cooldown_(new_CooldownEdit())
    , session_(new QLineEdit)
{
    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(opticBtn_);
    modeLayout->addWidget(new QRadioButton(tr("Acoustic")));

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(opticBtn_, QRadioButton::toggled, this, SnapshotPane::parameterModeSwitch);
    opticBtn_->setChecked(true);

    // Parameter box ------------------------------------------------------------

    int row=0;

    putInGrid(aF_, paramBox_, row++, tr("Frequency"), "[MHz]");
    putInGrid(aP_, paramBox_, row++, tr("Power"), "[mW]");
    putInGrid(wl_, paramBox_, row++, tr("Wavelength"), "[nm]");
    putInGrid(eet_, paramBox_, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldown_, paramBox_, row++, tr("Cooldown"), "[ms]");

    paramBox_->addWidget(new QLabel("Session:"), row, 0);
    paramBox_->addWidget(session_, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    leftLayout_->insertWidget(0, modeBox);
    snapshotBox_->setTitle(tr("Snapshot"));
}

void SnapshotPane::parameterModeSwitch()
{
    if (opticBtn_->isChecked())
    {
        wl_->setEnabled(true);
        aF_->setEnabled(false);
        aP_->setEnabled(false);
    }
    else
    {
        wl_->setEnabled(false);
        aF_->setEnabled(true);
        aP_->setEnabled(true);
    }
}
