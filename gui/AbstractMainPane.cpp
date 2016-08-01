#include <QLabel>
#include <QVBoxLayout>

#include "AbstractMainPane.h"
#include "CameraButtonBox.h"
#include "DatagramBox.h"

namespace gui {

//------------------------------------------------------------------------------

AbstractMainPane::AbstractMainPane(const core::Crystal& crystal)
    : QWidget()
    , leftLayout(new QVBoxLayout)
    , paramBoxLayout(new QGridLayout)
    , snapshotBox(new QGroupBox)
    , crystal(crystal)
{
    // Parameter box -----------------------------------------------------------

    auto parameterBox = new QGroupBox(tr("Parameters"));
    parameterBox->setLayout(paramBoxLayout);
    paramBoxLayout->setColumnMinimumWidth(0, 75);

    // Intensity datagram ------------------------------------------------------

    auto datagramBox = new DatagramBox;

    // Button box --------------------------------------------------------------

    auto cameraButtonBox = new CameraButtonBox;


    // Left Layout -------------------------------------------------------------

    leftLayout->addWidget(parameterBox);
    leftLayout->addWidget(datagramBox);
    leftLayout->addStretch();
    leftLayout->addWidget(cameraButtonBox);


    // Snapshot image ----------------------------------------------------------

    auto snapshot = QPixmap(512, 512);
    auto snapshotLbl = new QLabel();
    snapshotLbl->setPixmap(snapshot);

    auto snapshotLayout = new QVBoxLayout;
    snapshotLayout->addStretch();
    snapshotLayout->addWidget(snapshotLbl);
    snapshotLayout->addStretch();

    snapshotBox->setLayout(snapshotLayout);


    // Right layout ------------------------------------------------------------

    auto rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(snapshotBox);
    rightLayout->addStretch();

    // Main layout -------------------------------------------------------------

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

//------------------------------------------------------------------------------

}
