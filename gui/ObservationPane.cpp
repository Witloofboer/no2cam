#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "tooling.h"

ObservationPane::ObservationPane(QWidget *parent)
    : AbstractMainPane(parent)
    , wl1_(new_WavelengthEdit())
    , wl2_(new_WavelengthEdit())
    , eet_(new_EetEdit())
    , cooldown_(new_CooldownEdit())
    , shots_(new_NumberEdit("00099"))
    , session_(new QLineEdit)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    putInGrid(wl1_, paramBox_, row++, tr("Wavelength 1"), "[nm]");
    putInGrid(wl2_, paramBox_, row++, tr("Wavelength 2"), "[nm]");
    putInGrid(eet_, paramBox_, row++, tr("Exposure"), "[ms]");
    putInGrid(cooldown_, paramBox_, row++, tr("Cooldown"), "[ms]");
    putInGrid(shots_, paramBox_, row++, tr("# Snap/obs"), "");

    paramBox_->addWidget(new QLabel("Session:"), row, 0);
    paramBox_->addWidget(session_, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    snapshotBox_->setTitle(tr("Observation"));
}
