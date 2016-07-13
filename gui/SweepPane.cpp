#include "SweepingPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "tooling.h"

SweepingPane::SweepingPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wl1(new_WavelengthEdit())
    , wl2(new_WavelengthEdit())
    , wlDelta(new_WavelengthEdit())
    , eet(new_EetEdit())
    , cooldown(new_CooldownEdit())
    , session(new QLineEdit)

{
    // Parameter box -----------------------------------------------------------

    int row=0;
    putInGrid(wl1, paramBox, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wl2, paramBox, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(wlDelta, paramBox, row++, tr("Increment"), "[nm]");
    putInGrid(eet, paramBox, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldown, paramBox, row++, tr("Cooldown"), "[ms]");

    paramBox->addWidget(new QLabel("Session:"), row, 0);
    paramBox->addWidget(session, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    snapshotBox->setTitle(tr("Snapshot (Sweep mode)"));
}
