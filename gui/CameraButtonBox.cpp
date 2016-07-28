#include "CameraButtonBox.h"

#include <QHBoxLayout>
#include <QPushButton>

namespace gui {

//------------------------------------------------------------------------------

CameraButtonBox::CameraButtonBox(QWidget *parent)
    : QGroupBox(parent)
    , takeOneBtn_(new QPushButton(QIcon(":/icons/media-one-32.png"), ""))
    , burstBtn_(new QPushButton(QIcon(":/icons/media-burst-32.png"), ""))
    , recordBtn_(new QPushButton(QIcon(":/icons/media-record-32.png"), ""))
    , stopBtn_(new QPushButton(QIcon(":/icons/media-stop-32.png"), ""))
{
    takeOneBtn_->setCheckable(true);
    takeOneBtn_->setFixedWidth(40);
    connect(takeOneBtn_, QPushButton::clicked, this, CameraButtonBox::takeOneClicked);

    burstBtn_->setCheckable(true);
    burstBtn_->setFixedWidth(40);
    burstBtn_->setCheckable(true);
    connect(burstBtn_, QPushButton::clicked, this, CameraButtonBox::burstClicked);

    recordBtn_->setFixedWidth(40);
    recordBtn_->setCheckable(true);
    connect(recordBtn_, QPushButton::clicked, this, CameraButtonBox::recordClicked);

    stopBtn_->setFixedWidth(40);

    connect(stopBtn_, QPushButton::clicked, this, CameraButtonBox::stopClicked);

    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(takeOneBtn_);
    btnLayout->addWidget(burstBtn_);
    btnLayout->addWidget(recordBtn_);
    btnLayout->addWidget(stopBtn_);
    btnLayout->addStretch();

    setLayout(btnLayout);
}

//------------------------------------------------------------------------------

void CameraButtonBox::takeOneClicked()
{
    takeOneBtn_->setEnabled(false);
    burstBtn_->setEnabled(false);
    emit clicked(kTakeOne);
}

//------------------------------------------------------------------------------

void CameraButtonBox::burstClicked()
{
    takeOneBtn_->setEnabled(false);
    burstBtn_->setEnabled(false);
    emit clicked(kBurst);
}

void CameraButtonBox::recordClicked()
{
    emit clicked(recordBtn_->isChecked() ? kRecord : kEndRecord);
}

//------------------------------------------------------------------------------

void CameraButtonBox::stopClicked()
{
    takeOneBtn_->setChecked(false);
    takeOneBtn_->setEnabled(true);
    burstBtn_->setChecked(false);
    burstBtn_->setEnabled(true);
    emit clicked(kStop);
}

//------------------------------------------------------------------------------

}
