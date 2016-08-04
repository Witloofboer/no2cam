#include "CameraBtnBox.h"

#include <QHBoxLayout>
#include <QPushButton>

#include "../core/core_lib.h"
#include "../core/Core.h"

namespace gui {

//------------------------------------------------------------------------------

CameraBtnBox::CameraBtnBox()
    : QGroupBox()
    , _singleBtn(new QPushButton(QIcon(":/icons/media-one-32.png"), ""))
    , _burstBtn(new QPushButton(QIcon(":/icons/media-burst-32.png"), ""))
    , _recordBtn(new QPushButton(QIcon(":/icons/media-record-32.png"), ""))
    , _stopBtn(new QPushButton(QIcon(":/icons/media-stop-32.png"), ""))
    , _coreReady(true)
    , _playEnabled(false)
    , _recordEnabled(false)
{
    _singleBtn->setCheckable(true);
    _singleBtn->setFixedWidth(40);
    _singleBtn->setEnabled(false);
    connect(_singleBtn, QPushButton::clicked, this, CameraBtnBox::singleClicked);

    _burstBtn->setCheckable(true);
    _burstBtn->setFixedWidth(40);
    _burstBtn->setEnabled(false);
    connect(_burstBtn, QPushButton::clicked, this, CameraBtnBox::burstClicked);

    _recordBtn->setFixedWidth(40);
    _recordBtn->setCheckable(true);
    _recordBtn->setEnabled(false);
    _stopBtn->setFixedWidth(40);


    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(_singleBtn);
    btnLayout->addWidget(_burstBtn);
    btnLayout->addWidget(_recordBtn);
    btnLayout->addWidget(_stopBtn);
    btnLayout->addStretch();

    setLayout(btnLayout);

    connect(_stopBtn, QPushButton::clicked, core::singleton(), core::Core::stopRequested);
    connect(core::singleton(), core::Core::ready, this, coreReady);
}

//------------------------------------------------------------------------------

void CameraBtnBox::coreReady()
{
    _coreReady = true;
    _singleBtn->setChecked(false);
    _burstBtn->setChecked(false);
    refreshBtns();
}

//------------------------------------------------------------------------------

void CameraBtnBox::enableBtns(bool playEnabled, bool recordEnabled)
{
    _playEnabled = playEnabled;
    _recordEnabled = recordEnabled;
    refreshBtns();
}


//------------------------------------------------------------------------------

void CameraBtnBox::refreshBtns()
{
    _singleBtn->setEnabled(_coreReady & _playEnabled);
    _burstBtn->setEnabled(_singleBtn->isEnabled());
    _recordBtn->setEnabled(_coreReady & _recordEnabled);
}

//------------------------------------------------------------------------------

void CameraBtnBox::singleClicked()
{
    _coreReady = false;
    refreshBtns();
    emit start(false, _recordBtn->isChecked());
}

//------------------------------------------------------------------------------

void CameraBtnBox::burstClicked()
{
    _coreReady = false;
    refreshBtns();
    emit start(true, _recordBtn->isChecked());
}

//------------------------------------------------------------------------------

}
