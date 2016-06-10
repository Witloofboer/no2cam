#include "CameraButtonBox.h"

#include <QHBoxLayout>
#include <QPushButton>

CameraButtonBox::CameraButtonBox(QWidget *parent)
    : QGroupBox(parent)
    , mTakeOneBtn(new QPushButton(QIcon(":/icons/media-one-32.png"), ""))
    , mBurstBtn(new QPushButton(QIcon(":/icons/media-burst-32.png"), ""))
    , mRecordBtn(new QPushButton(QIcon(":/icons/media-record-32.png"), ""))
    , mStopBtn(new QPushButton(QIcon(":/icons/media-stop-32.png"), ""))
{
    mTakeOneBtn->setCheckable(true);
    mTakeOneBtn->setFixedWidth(40);
    connect(mTakeOneBtn, QPushButton::clicked, this, CameraButtonBox::takeOneClicked);

    mBurstBtn->setCheckable(true);
    mBurstBtn->setFixedWidth(40);
    mBurstBtn->setCheckable(true);
    connect(mBurstBtn, QPushButton::clicked, this, CameraButtonBox::burstClicked);

    mRecordBtn->setFixedWidth(40);
    mRecordBtn->setCheckable(true);
    connect(mRecordBtn, QPushButton::clicked, this, CameraButtonBox::recordClicked);

    mStopBtn->setFixedWidth(40);

    connect(mStopBtn, QPushButton::clicked, this, CameraButtonBox::stopClicked);

    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(mTakeOneBtn);
    btnLayout->addWidget(mBurstBtn);
    btnLayout->addWidget(mRecordBtn);
    btnLayout->addWidget(mStopBtn);
    btnLayout->addStretch();

    setLayout(btnLayout);
}

void CameraButtonBox::takeOneClicked()
{
    mTakeOneBtn->setEnabled(false);
    mBurstBtn->setEnabled(false);
    emit clicked(kTakeOne);
}

void CameraButtonBox::burstClicked()
{
    mTakeOneBtn->setEnabled(false);
    mBurstBtn->setEnabled(false);
    emit clicked(kBurst);
}

void CameraButtonBox::recordClicked()
{
    emit clicked(mRecordBtn->isChecked() ? kRecord : kEndRecord);
}

void CameraButtonBox::stopClicked()
{
    mTakeOneBtn->setChecked(false);
    mTakeOneBtn->setEnabled(true);
    mBurstBtn->setChecked(false);
    mBurstBtn->setEnabled(true);
    emit clicked(kStop);
}

