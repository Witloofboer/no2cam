#include "SweepingPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "tooling.h"

SweepingPane::SweepingPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wl1_(new_WavelengthEdit())
    , wl2_(new_WavelengthEdit())
    , wlDelta_(new_WavelengthEdit())
    , eet_(new_EetEdit())
    , cooldown_(new_CooldownEdit())
    , session_(new QLineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;
    putInGrid(wl1_, paramBox_, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wl2_, paramBox_, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(wlDelta_, paramBox_, row++, tr("Increment"), "[nm]");
    putInGrid(eet_, paramBox_, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldown_, paramBox_, row++, tr("Cooldown"), "[ms]");

    paramBox_->addWidget(new QLabel("Session:"), row, 0);
    paramBox_->addWidget(session_, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    snapshotBox_->setTitle(tr("Snapshot (Sweep mode)"));
}
