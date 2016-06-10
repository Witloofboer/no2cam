#include "SweepingPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "tooling.h"

SweepingPane::SweepingPane(QWidget *parent)
    : AbstractMainPane(parent)
    , mWavelength1(new_WavelengthEdit())
    , mWavelength2(new_WavelengthEdit())
    , mWavelengthDelta(new_WavelengthEdit())
    , mEET(new_EetEdit())
    , mCoolDown(new_NumberEdit("09.9"))
    , mSessionName(new QLineEdit)

{
    // Parameter box -----------------------------------------------------------

    int row=0;
    putInGrid(mWavelength1, mParameters, row++, tr("Wavelength 1"), "nm");
    putInGrid(mWavelength2, mParameters, row++, tr("Wavelength 2"), "nm");
    putInGrid(mWavelengthDelta, mParameters, row++, tr("Increment"), "nm");
    putInGrid(mEET, mParameters, row++, tr("Exposure"), "ms");
    putInGrid(mCoolDown, mParameters, row++, tr("Cooldown"), "ms");

    mParameters->addWidget(new QLabel("Filename head:"), row, 0);
    mParameters->addWidget(mSessionName, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    mSnapshotBox->setTitle(tr("Snapshot (Sweep mode)"));
}
