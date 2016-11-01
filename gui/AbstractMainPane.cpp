#include <iterator>

#include <QLabel>
#include <QPainter>
#include <QSettings>
#include <QVBoxLayout>

#include "AbstractMainPane.h"
#include "CameraBtnBox.h"
#include "DatagramBox.h"
#include "tooling.h"

#include "../core/ImageBuffer.h"

namespace gui {

//------------------------------------------------------------------------------

AbstractMainPane::AbstractMainPane(MainWindow *mainWindow)
    : QWidget()
    , _paramBoxLayout(new QGridLayout)
    , _snapshotBox(new QGroupBox)
    , _cameraBtnBox(new CameraBtnBox(mainWindow))
    , _snapshotLbl(new QLabel)
    , _exposureEdit(new IntLineEdit)
    , _cooldownTimeEdit(new IntLineEdit)
    , _cooldownPwrEdit(new IntLineEdit)
    , _sessionEdit(new LineEdit)
    , _snapshotImg(core::ImageBuffer::size,
                   core::ImageBuffer::size,
                   QImage::Format_Indexed8)
    , _datagramBox(new DatagramBox)
{
    // Parameter box -----------------------------------------------------------

    auto parameterBox = new QGroupBox(tr("Parameters"));
    parameterBox->setLayout(_paramBoxLayout);
    _paramBoxLayout->setColumnMinimumWidth(0, 75);


    // Left Layout -------------------------------------------------------------

    auto leftLayout = new QVBoxLayout;
    leftLayout->addWidget(parameterBox);
    leftLayout->addStretch();
    leftLayout->addWidget(_cameraBtnBox);


    // Snapshot image ----------------------------------------------------------

    for(int i=0; i<256; ++i)
    {
        _snapshotImg.setColor(i, qRgb(i, i, i));
    }

    _snapshotImg.fill(0);
    _snapshotLbl->setPixmap(QPixmap::fromImage(_snapshotImg));

    auto snapshotLayout = new QVBoxLayout;
    snapshotLayout->addStretch();
    snapshotLayout->addWidget(_snapshotLbl);
    snapshotLayout->addStretch();

    _snapshotBox->setLayout(snapshotLayout);


    // Middle layout -----------------------------------------------------------

    auto middleLayout = new QVBoxLayout;
    middleLayout->addWidget(_snapshotBox);
    middleLayout->addStretch();


    // Right layout ------------------------------------------------------------

    auto rightLayout = new QVBoxLayout;
    rightLayout->addWidget(_datagramBox);
    rightLayout->addStretch();


    // Main layout -------------------------------------------------------------

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);


    // Connections
    connect(_sessionEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cameraBtnBox, CameraBtnBox::start, this, start);

    connect(_exposureEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cooldownTimeEdit, LineEdit::textChanged, this, refreshBtns);
    connect(_cooldownPwrEdit, LineEdit::textChanged, this, refreshBtns);
}

//------------------------------------------------------------------------------

void AbstractMainPane::putInGrid(int &row)
{
    gui::putInGrid(_exposureEdit, _paramBoxLayout, row, tr("Exposure"), "[ms]");
    gui::putInGrid(_cooldownTimeEdit, _paramBoxLayout, row, tr("Cooldown time"), "[ms]");
    gui::putInGrid(_cooldownPwrEdit, _paramBoxLayout, row, tr("Cooldown power"), "[mW]");
}

//------------------------------------------------------------------------------

void AbstractMainPane::updateState(bool isAppReady)
{
    _cameraBtnBox->updateState(isAppReady);
}

//------------------------------------------------------------------------------

void AbstractMainPane::updateSnapshot()
{
    const int size = core::ImageBuffer::size;

    uchar *pixels = _snapshotImg.bits();
    core::gImageBuffer.fill(pixels);

    Datagram datagram;
    for(int i=0; i<256; ++i)
        datagram[i] = 0;

    for(int i=0; i<size*size; ++i)
    {
        ++datagram[pixels[i]];
    }

    _snapshotLbl->setPixmap(QPixmap::fromImage(_snapshotImg));
    _datagramBox->update(datagram);
}

//------------------------------------------------------------------------------

void AbstractMainPane::refreshBtns()
{
    _cameraBtnBox->enableBtns(areParametersValid(),
                              !_sessionEdit->text().isEmpty());
}

//------------------------------------------------------------------------------

bool AbstractMainPane::areParametersValid() const
{
    return _exposureEdit->isValid() &&
           _cooldownTimeEdit->isValid() &&
           _cooldownPwrEdit->isValid();
}

//------------------------------------------------------------------------------

static const char *exposureLbl = "exposure [ms]";
static const char *cooldownTimeLbl = "cooldown time[ms]";
static const char *cooldownPwrLbl = "cooldown power[mW]";

void AbstractMainPane::persiste(QSettings &settings) const
{
    if (_exposureEdit->isValid())
        settings.setValue(exposureLbl, _exposureEdit->text());
    if (_cooldownTimeEdit->isValid())
        settings.setValue(cooldownTimeLbl, _cooldownTimeEdit->text());
    if (_cooldownPwrEdit->isValid())
        settings.setValue(cooldownPwrLbl, _cooldownPwrEdit->text());
}

//------------------------------------------------------------------------------

void AbstractMainPane::restore(QSettings &settings)
{
    _exposureEdit->setText(settings.value(exposureLbl).toString());
    _cooldownTimeEdit->setText(settings.value(cooldownTimeLbl).toString());
    _cooldownPwrEdit->setText(settings.value(cooldownPwrLbl).toString());
}

//------------------------------------------------------------------------------

}
