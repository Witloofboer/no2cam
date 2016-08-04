#include <QLabel>
#include <QVBoxLayout>

#include "AbstractMainPane.h"
#include "CameraBtnBox.h"
#include "DatagramBox.h"

namespace gui {

//------------------------------------------------------------------------------

AbstractMainPane::AbstractMainPane(const core::Crystal& crystal)
    : QWidget()
    , _leftLayout(new QVBoxLayout)
    , _paramBoxLayout(new QGridLayout)
    , _snapshotBox(new QGroupBox)
    , _cameraBtnBox(new CameraBtnBox)
    , _crystal(crystal)
{
    // Parameter box -----------------------------------------------------------

    auto parameterBox = new QGroupBox(tr("Parameters"));
    parameterBox->setLayout(_paramBoxLayout);
    _paramBoxLayout->setColumnMinimumWidth(0, 75);


    // Intensity datagram ------------------------------------------------------

    auto datagramBox = new DatagramBox;


    // Left Layout -------------------------------------------------------------

    _leftLayout->addWidget(parameterBox);
    _leftLayout->addWidget(datagramBox);
    _leftLayout->addStretch();
    _leftLayout->addWidget(_cameraBtnBox);


    // Snapshot image ----------------------------------------------------------

    auto snapshot = QPixmap(512, 512);
    auto snapshotLbl = new QLabel();
    snapshotLbl->setPixmap(snapshot);

    auto snapshotLayout = new QVBoxLayout;
    snapshotLayout->addStretch();
    snapshotLayout->addWidget(snapshotLbl);
    snapshotLayout->addStretch();

    _snapshotBox->setLayout(snapshotLayout);


    // Right layout ------------------------------------------------------------

    auto rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(_snapshotBox);
    rightLayout->addStretch();

    // Main layout -------------------------------------------------------------

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(_leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

//------------------------------------------------------------------------------

}
