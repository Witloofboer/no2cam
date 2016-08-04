#include "SweepPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

SweepPane::SweepPane(const core::Crystal &crystal)
    : AbstractMainPane(crystal)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _wavelengthIncEdit(new DoubleLineEdit)
    , _exposureEdit(new IntLineEdit)
    , _cooldownEdit(new IntLineEdit)
    , _sessionEdit(new LineEdit)
{
    // Parameter box -----------------------------------------------------------
    int row=0;
    putInGrid(_wavelength1Edit, _paramBoxLayout, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(_wavelength2Edit, _paramBoxLayout, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(_wavelengthIncEdit, _paramBoxLayout, row++, tr("Increment"), "[nm]");
    putInGrid(_exposureEdit, _paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(_cooldownEdit, _paramBoxLayout, row++, tr("Cooldown"), "[ms]");

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    _snapshotBox->setTitle(tr("Snapshot (Sweep mode)"));
}

//------------------------------------------------------------------------------

}
