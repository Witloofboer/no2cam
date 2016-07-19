#include <QLabel>
#include <QVBoxLayout>

#include "AbstractMainPane.h"
#include "CameraButtonBox.h"
#include "DatagramBox.h"

AbstractMainPane::AbstractMainPane(QWidget *parent)
    : QWidget(parent)
    , leftLayout_(new QVBoxLayout)
    , paramBox_(new QGridLayout)
    , snapshotBox_(new QGroupBox)
{
    // Parameter box -----------------------------------------------------------

    auto parameterBox = new QGroupBox(tr("Parameters"));
    parameterBox->setLayout(paramBox_);

    // Intensity datagram ------------------------------------------------------

    auto datagramBox = new DatagramBox;

    // Button box --------------------------------------------------------------

    auto cameraButtonBox = new CameraButtonBox;


    // Left Layout -------------------------------------------------------------

    leftLayout_->addWidget(parameterBox);
    leftLayout_->addWidget(datagramBox);
    leftLayout_->addStretch();
    leftLayout_->addWidget(cameraButtonBox);


    // Snapshot image ----------------------------------------------------------

    auto snapshot = QPixmap(512, 512);
    auto snapshotLbl = new QLabel();
    snapshotLbl->setPixmap(snapshot);

    auto snapshotLayout = new QVBoxLayout;
    snapshotLayout->addStretch();
    snapshotLayout->addWidget(snapshotLbl);
    snapshotLayout->addStretch();

    snapshotBox_->setLayout(snapshotLayout);


    // Right layout ------------------------------------------------------------

    auto rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(snapshotBox_);
    rightLayout->addStretch();

    // Main layout -------------------------------------------------------------

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout_);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}
