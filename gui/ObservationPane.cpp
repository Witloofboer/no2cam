#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "tooling.h"

ObservationPane::ObservationPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wl1(new_WavelengthEdit())
    , wl2(new_WavelengthEdit())
    , eet(new_EetEdit())
    , cooldown(new_CooldownEdit())
    , shots(new_NumberEdit("00099"))
    , session(new QLineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(wl1, paramBox, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wl2, paramBox, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(eet, paramBox, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldown, paramBox, row++, tr("Cooldown"), "[ms]");
    putInGrid(shots, paramBox, row++, tr("# Snap/obs"), "");

    paramBox->addWidget(new QLabel("Session:"), row, 0);
    paramBox->addWidget(session, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    snapshotBox->setTitle(tr("Observation"));
}
