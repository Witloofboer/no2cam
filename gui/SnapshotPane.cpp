#include "SnapshotPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

#include "tooling.h"

SnapshotPane::SnapshotPane(QWidget *parent)
    : AbstractMainPane(parent)
    , opticBtn(new QRadioButton(tr("Optic")))
    , wl(new_WavelengthEdit())
    , aF(new_NumberEdit("009.999"))
    , aP(new_NumberEdit(" 0009"))
    , eet(new_EetEdit())
    , cooldown(new_CooldownEdit())
    , session(new QLineEdit)
{
    // Optic/accoustic ---------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(opticBtn);
    modeLayout->addWidget(new QRadioButton(tr("Acoustic")));

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(opticBtn, QRadioButton::toggled, this, SnapshotPane::parameterModeSwitch);
    opticBtn->setChecked(true);

    // Parameter box ------------------------------------------------------------

    int row=0;

    putInGrid(aF, paramBox, row++, tr("Frequency"), "[MHz]");
    putInGrid(aP, paramBox, row++, tr("Power"), "[mW]");
    putInGrid(wl, paramBox, row++, tr("Wavelength"), "[nm]");
    putInGrid(eet, paramBox, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldown, paramBox, row++, tr("Cooldown"), "[ms]");

    paramBox->addWidget(new QLabel("Session:"), row, 0);
    paramBox->addWidget(session, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    leftLayout->insertWidget(0, modeBox);
    snapshotBox->setTitle(tr("Snapshot"));
}

void SnapshotPane::parameterModeSwitch()
{
    if (opticBtn->isChecked())
    {
        wl->setEnabled(true);
        aF->setEnabled(false);
        aP->setEnabled(false);
    }
    else
    {
        wl->setEnabled(false);
        aF->setEnabled(true);
        aP->setEnabled(true);
    }
}
