#include "SweepPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

SweepingPane::SweepingPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wavelength1Edit(new DoubleLineEdit)
    , wavelength2Edit(new DoubleLineEdit)
    , wavelengthIncEdit(new DoubleLineEdit)
    , exposureEdit(new IntLineEdit)
    , cooldownEdit(new IntLineEdit)
    , sessionEdit(new LineEdit)
{
    // Parameter box -----------------------------------------------------------
    int row=0;
    putInGrid(wavelength1Edit, paramBoxLayout, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wavelength2Edit, paramBoxLayout, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(wavelengthIncEdit, paramBoxLayout, row++, tr("Increment"), "[nm]");
    putInGrid(exposureEdit, paramBoxLayout, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldownEdit, paramBoxLayout, row++, tr("Cooldown"), "[ms]");

    paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    paramBoxLayout->addWidget(sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base instance
    snapshotBox->setTitle(tr("Snapshot (Sweep mode)"));
}

//------------------------------------------------------------------------------

}
