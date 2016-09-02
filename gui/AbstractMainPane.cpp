#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

#include "AbstractMainPane.h"
#include "CameraBtnBox.h"
#include "DatagramBox.h"
#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

AbstractMainPane::AbstractMainPane(MainWindow *mainWindow)
    : QWidget()
    , _leftLayout(new QVBoxLayout)
    , _paramBoxLayout(new QGridLayout)
    , _snapshotBox(new QGroupBox)
    , _cameraBtnBox(new CameraBtnBox(mainWindow))
    , _exposureEdit(new IntLineEdit)
    , _cooldownTimeEdit(new IntLineEdit)
    , _cooldownPwrEdit(new IntLineEdit)
    , _sessionEdit(new LineEdit)
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
