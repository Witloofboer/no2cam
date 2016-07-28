#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ObservationPane::ObservationPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wavelength1Edit_(new DoubleLineEdit)
    , wavelength2Edit_(new DoubleLineEdit)
    , exposureEdit_(new IntLineEdit())
    , snapPerObsEdit_(new IntLineEdit)
    , cooldownEdit_(new IntLineEdit())
    , sessionEdit_(new LineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(wavelength1Edit_, paramBoxLayout_, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wavelength2Edit_, paramBoxLayout_, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(exposureEdit_, paramBoxLayout_, row++, tr("Exposure"), "[ms]");
    putInGrid(snapPerObsEdit_, paramBoxLayout_, row++, tr("Snapshots/obs"), "");
    putInGrid(cooldownEdit_, paramBoxLayout_, row++, tr("Cooldown/obs"), "[ms]");

    paramBoxLayout_->addWidget(new QLabel("Session:"), row, 0);
    paramBoxLayout_->addWidget(sessionEdit_, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    snapshotBox_->setTitle(tr("Observation"));
}

//------------------------------------------------------------------------------

}
