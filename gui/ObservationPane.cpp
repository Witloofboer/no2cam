#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ObservationPane::ObservationPane(const core::Crystal &crystal)
    : AbstractMainPane(crystal)
    , wavelength1Edit(new DoubleLineEdit)
    , wavelength2Edit(new DoubleLineEdit)
    , exposureEdit(new IntLineEdit())
    , snapPerObsEdit(new IntLineEdit)
    , cooldownEdit(new IntLineEdit())
    , sessionEdit(new LineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(wavelength1Edit, paramBoxLayout, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wavelength2Edit, paramBoxLayout, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(exposureEdit, paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(snapPerObsEdit, paramBoxLayout, row++, tr("Snapshots/obs"), "");
    putInGrid(cooldownEdit, paramBoxLayout, row++, tr("Cooldown/obs"), "[ms]");

    paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    paramBoxLayout->addWidget(sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    snapshotBox->setTitle(tr("Observation"));
}

//------------------------------------------------------------------------------

}
