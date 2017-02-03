#include "CameraBtnBox.h"

#include <QHBoxLayout>
#include <QPushButton>

namespace gui {

//------------------------------------------------------------------------------

CameraBtnBox::CameraBtnBox()
    : QGroupBox()
    , _singleBtn(new QPushButton(QIcon(":/icons/media-one-32.png"), ""))
    , _burstBtn(new QPushButton(QIcon(":/icons/media-burst-32.png"), ""))
    , _recordBtn(new QPushButton(QIcon(":/icons/media-record-32.png"), ""))
    , _stopBtn(new QPushButton(QIcon(":/icons/media-stop-32.png"), ""))
    , _isAppReady(true)
    , _playEnabled(false)
    , _recordEnabled(false)
{
    _singleBtn->setCheckable(true);
    _singleBtn->setFixedWidth(40);
    _singleBtn->setEnabled(false);

    _burstBtn->setCheckable(true);
    _burstBtn->setFixedWidth(40);
    _burstBtn->setEnabled(false);

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

    connect(_singleBtn, QPushButton::clicked, this, start);
    connect(_burstBtn, QPushButton::clicked, this, start);
    connect(_stopBtn, QPushButton::clicked, this, stopped);
}

//------------------------------------------------------------------------------

void CameraBtnBox::updateState(bool isAppReady)
{
    _isAppReady = isAppReady;
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
    _singleBtn->setEnabled(_isAppReady & _playEnabled);
    _burstBtn->setEnabled(_singleBtn->isEnabled());
    _recordBtn->setEnabled(_isAppReady & _recordEnabled);
    if (_isAppReady)
    {
        _singleBtn->setChecked(false);
        _burstBtn->setChecked(false);
        if (!_recordEnabled) _recordBtn->setChecked(false);
    }

}

//------------------------------------------------------------------------------

void CameraBtnBox::start()
{
    emit started(_burstBtn->isChecked(), _recordBtn->isChecked());
}

//------------------------------------------------------------------------------

}
