#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ObservationPane::ObservationPane(const core::Crystal &crystal)
    : AbstractMainPane(crystal)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _exposureEdit(new IntLineEdit())
    , _snapPerObsEdit(new IntLineEdit)
    , _cooldownEdit(new IntLineEdit())
    , _sessionEdit(new LineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(_wavelength1Edit, _paramBoxLayout, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(_wavelength2Edit, _paramBoxLayout, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(_exposureEdit, _paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(_snapPerObsEdit, _paramBoxLayout, row++, tr("Snapshots/obs"), "");
    putInGrid(_cooldownEdit, _paramBoxLayout, row++, tr("Cooldown/obs"), "[ms]");

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    _snapshotBox->setTitle(tr("Observation"));
}

//------------------------------------------------------------------------------

}
