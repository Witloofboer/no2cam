#include <QLabel>
#include <QVBoxLayout>

#include "AbstractMainPane.h"
#include "CameraButtonBox.h"
#include "DatagramBox.h"

AbstractMainPane::AbstractMainPane(QWidget *parent)
    : QWidget(parent)
    , mLeftLayout(new QVBoxLayout)
    , mParameters(new QGridLayout)
    , mSnapshotBox(new QGroupBox)
    , mSnapshotWidget(new QLabel)
{
    // Parameter box -----------------------------------------------------------

    auto parameterBox = new QGroupBox(tr("Parameters"));
    parameterBox->setLayout(mParameters);

    // Intensity datagram ------------------------------------------------------

    auto datagramBox = new DatagramBox;

    // Button box --------------------------------------------------------------

    auto cameraButtonBox = new CameraButtonBox;


    // Left Layout -------------------------------------------------------------

    mLeftLayout->addWidget(parameterBox);
    mLeftLayout->addWidget(datagramBox);
    mLeftLayout->addStretch();
    mLeftLayout->addWidget(cameraButtonBox);


    // Snapshot image ----------------------------------------------------------

    auto snapshot = QPixmap(512, 512);
    auto snapshotLbl = new QLabel();
    snapshotLbl->setPixmap(snapshot);

    auto snapshotLayout = new QVBoxLayout;
    snapshotLayout->addStretch();
    snapshotLayout->addWidget(snapshotLbl);
    snapshotLayout->addStretch();

    mSnapshotBox->setLayout(snapshotLayout);


    // Right layout ------------------------------------------------------------

    auto rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(mSnapshotBox);
    rightLayout->addStretch();

    // Main layout -------------------------------------------------------------

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(mLeftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}
