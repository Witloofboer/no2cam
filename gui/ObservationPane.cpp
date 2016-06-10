#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "tooling.h"

ObservationPane::ObservationPane(QWidget *parent)
    : AbstractMainPane(parent)
    , mWavelength1(new_WavelengthEdit())
    , mWavelength2(new_WavelengthEdit())
    , mEET(new_EetEdit())
    , mSnapshotsPerObs(new_NumberEdit("09"))
    , mCoolDown(new_NumberEdit("09.9"))
    , mSessionName(new QLineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(mWavelength1, mParameters, row++, tr("Wavelength 1"), "nm");
    putInGrid(mWavelength2, mParameters, row++, tr("Wavelength 2"), "nm");
    putInGrid(mEET, mParameters, row++, tr("Exposure"), "ms");
    putInGrid(mSnapshotsPerObs, mParameters, row++, tr("# Snap/obs"), "");
    putInGrid(mCoolDown, mParameters, row++, tr("Cooldown"), "ms");

    mParameters->addWidget(new QLabel("Filename head:"), row, 0);
    mParameters->addWidget(mSessionName, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    mSnapshotBox->setTitle(tr("Observation"));
}
