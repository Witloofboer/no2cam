#include "CameraButtonBox.h"

#include <QHBoxLayout>
#include <QPushButton>

CameraButtonBox::CameraButtonBox(QWidget *parent)
    : QGroupBox(parent)
    , takeOneBtn(new QPushButton(QIcon(":/icons/media-one-32.png"), ""))
    , burstBtn(new QPushButton(QIcon(":/icons/media-burst-32.png"), ""))
    , recordBtn(new QPushButton(QIcon(":/icons/media-record-32.png"), ""))
    , stopBtn(new QPushButton(QIcon(":/icons/media-stop-32.png"), ""))
{
    takeOneBtn->setCheckable(true);
    takeOneBtn->setFixedWidth(40);
    connect(takeOneBtn, QPushButton::clicked, this, CameraButtonBox::takeOneClicked);

    burstBtn->setCheckable(true);
    burstBtn->setFixedWidth(40);
    burstBtn->setCheckable(true);
    connect(burstBtn, QPushButton::clicked, this, CameraButtonBox::burstClicked);

    recordBtn->setFixedWidth(40);
    recordBtn->setCheckable(true);
    connect(recordBtn, QPushButton::clicked, this, CameraButtonBox::recordClicked);

    stopBtn->setFixedWidth(40);

    connect(stopBtn, QPushButton::clicked, this, CameraButtonBox::stopClicked);

    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(takeOneBtn);
    btnLayout->addWidget(burstBtn);
    btnLayout->addWidget(recordBtn);
    btnLayout->addWidget(stopBtn);
    btnLayout->addStretch();

    setLayout(btnLayout);
}

void CameraButtonBox::takeOneClicked()
{
    takeOneBtn->setEnabled(false);
    burstBtn->setEnabled(false);
    emit clicked(kTakeOne);
}

void CameraButtonBox::burstClicked()
{
    takeOneBtn->setEnabled(false);
    burstBtn->setEnabled(false);
    emit clicked(kBurst);
}

void CameraButtonBox::recordClicked()
{
    emit clicked(recordBtn->isChecked() ? kRecord : kEndRecord);
}

void CameraButtonBox::stopClicked()
{
    takeOneBtn->setChecked(false);
    takeOneBtn->setEnabled(true);
    burstBtn->setChecked(false);
    burstBtn->setEnabled(true);
    emit clicked(kStop);
}

