#include "SweepPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

SweepingPane::SweepingPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wavelength1Edit_(new DoubleLineEdit)
    , wavelength2Edit_(new DoubleLineEdit)
    , wavelengthIncEdit_(new DoubleLineEdit)
    , exposureEdit_(new IntLineEdit)
    , cooldownEdit_(new IntLineEdit)
    , sessionEdit_(new LineEdit)
{
    // Parameter box -----------------------------------------------------------
    int row=0;
    putInGrid(wavelength1Edit_, paramBoxLayout_, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wavelength2Edit_, paramBoxLayout_, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(wavelengthIncEdit_, paramBoxLayout_, row++, tr("Increment"), "[nm]");
    putInGrid(exposureEdit_, paramBoxLayout_, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldownEdit_, paramBoxLayout_, row++, tr("Cooldown"), "[ms]");

    paramBoxLayout_->addWidget(new QLabel("Session:"), row, 0);
    paramBoxLayout_->addWidget(sessionEdit_, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    snapshotBox_->setTitle(tr("Snapshot (Sweep mode)"));
}

//------------------------------------------------------------------------------

}
