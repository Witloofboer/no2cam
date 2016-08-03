#include "CameraButtonBox.h"

#include <QHBoxLayout>
#include <QPushButton>

#include "../core/core_lib.h"
#include "../core/Core.h"

namespace gui {

//------------------------------------------------------------------------------

CameraButtonBox::CameraButtonBox(QWidget *parent)
    : QGroupBox(parent)
    , singleBtn(new QPushButton(QIcon(":/icons/media-one-32.png"), ""))
    , burstBtn(new QPushButton(QIcon(":/icons/media-burst-32.png"), ""))
    , recordBtn(new QPushButton(QIcon(":/icons/media-record-32.png"), ""))
    , stopBtn(new QPushButton(QIcon(":/icons/media-stop-32.png"), ""))
    , applicationReady(true)
    , playEnabled(false)
    , recordEnabled(false)
{
    singleBtn->setCheckable(true);
    singleBtn->setFixedWidth(40);
    singleBtn->setEnabled(false);
    connect(singleBtn, QPushButton::clicked, this, CameraButtonBox::singleClicked);

    burstBtn->setCheckable(true);
    burstBtn->setFixedWidth(40);
    burstBtn->setEnabled(false);
    connect(burstBtn, QPushButton::clicked, this, CameraButtonBox::burstClicked);

    recordBtn->setFixedWidth(40);
    recordBtn->setCheckable(true);
    recordBtn->setEnabled(false);
    stopBtn->setFixedWidth(40);

    connect(stopBtn, QPushButton::clicked, this, CameraButtonBox::stop);

    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(singleBtn);
    btnLayout->addWidget(burstBtn);
    btnLayout->addWidget(recordBtn);
    btnLayout->addWidget(stopBtn);
    btnLayout->addStretch();

    setLayout(btnLayout);

    connect(core::singleton(), core::Core::done, this, done);
}

//------------------------------------------------------------------------------

void CameraButtonBox::done()
{
    applicationReady = true;
    singleBtn->setChecked(false);
    burstBtn->setChecked(false);
    refreshBtnStatus();
}

//------------------------------------------------------------------------------

void CameraButtonBox::setButtons(bool playEnabled, bool recordEnabled)
{
    this->playEnabled = playEnabled;
    this->recordEnabled = recordEnabled;
    refreshBtnStatus();
}


//------------------------------------------------------------------------------

void CameraButtonBox::refreshBtnStatus()
{
    singleBtn->setEnabled(applicationReady & playEnabled);
    burstBtn->setEnabled(applicationReady & playEnabled);
    recordBtn->setEnabled(applicationReady & recordEnabled);
}

//------------------------------------------------------------------------------

void CameraButtonBox::singleClicked()
{
    applicationReady=false;
    refreshBtnStatus();
    emit play(false, recordBtn->isChecked());
}

//------------------------------------------------------------------------------

void CameraButtonBox::burstClicked()
{
    applicationReady=false;
    refreshBtnStatus();
    emit play(true, recordBtn->isChecked());
}

//------------------------------------------------------------------------------

}
