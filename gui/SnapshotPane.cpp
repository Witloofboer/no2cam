#include "SnapshotPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

#include "tooling.h"

SnapshotPane::SnapshotPane(QWidget *parent)
    : AbstractMainPane(parent)
    , mOpticBtn(new QRadioButton(tr("Optic")))
    , mWavelength(new_WavelengthEdit())
    , mAccousticFreq(new_NumberEdit("009.999"))
    , mAccousticPower(new_NumberEdit("009"))
    , mEET(new_EetEdit())
    , mCoolDown(new_CooldownEdit())
    , mSessionName(new QLineEdit)
{
    // Optic/accoustic ----------------------------------------------------------

    auto modeLayout = new QVBoxLayout;
    modeLayout->addWidget(mOpticBtn);
    modeLayout->addWidget(new QRadioButton(tr("Acoustic")));

    auto modeBox = new QGroupBox(tr("Parameter mode"));
    modeBox->setLayout(modeLayout);

    connect(mOpticBtn, QRadioButton::toggled, this, SnapshotPane::parameterModeSwitch);
    mOpticBtn->setChecked(true);

    // Parameter box ------------------------------------------------------------

    int row=0;

    putInGrid(mWavelength, mParameters, row++, tr("Wavelength"), "nm");
    putInGrid(mAccousticFreq, mParameters, row++, tr("Frequency"), "MHz");
    putInGrid(mAccousticPower, mParameters, row++, tr("Power"), "mW");
    putInGrid(mEET, mParameters, row++, tr("Exposure"), "ms");
    putInGrid(mCoolDown, mParameters, row++, tr("Cooldown"), "ms");

    mParameters->addWidget(new QLabel("Filename head:"), row, 0);
    mParameters->addWidget(mSessionName, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    mLeftLayout->insertWidget(0, modeBox);
    mSnapshotBox->setTitle(tr("Snapshot"));
}

void SnapshotPane::parameterModeSwitch()
{
    if (mOpticBtn->isChecked())
    {
        mWavelength->setEnabled(true);
        mAccousticFreq->setEnabled(false);
        mAccousticPower->setEnabled(false);
    }
    else
    {
        mWavelength->setEnabled(false);
        mAccousticFreq->setEnabled(true);
        mAccousticPower->setEnabled(true);
    }
}
