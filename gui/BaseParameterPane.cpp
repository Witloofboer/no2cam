#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "BaseParameterPane.h"

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

BaseParameterPane::BaseParameterPane()
    : QWidget()
    , _parameterBox(new QGroupBox())
    , _paramBoxLayout(new QGridLayout)
    , _exposureEdit(new IntLineEdit)
    , _cooldownTimeEdit(new IntLineEdit)
    , _cooldownPwrEdit(new IntLineEdit)
{
    // Layout
    _parameterBox->setLayout(_paramBoxLayout);
    _paramBoxLayout->setColumnMinimumWidth(0, 75);

    auto layout = new QVBoxLayout;
    layout->addWidget(_parameterBox);
    setLayout(layout);
    layout->setContentsMargins(0,0,0,0);

    // Connections
    connect(_exposureEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_cooldownTimeEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_cooldownPwrEdit, LineEdit::textChanged, this, parametersChanged);
}

//------------------------------------------------------------------------------

void BaseParameterPane::putInGrid(int &row)
{
    gui::putInGrid(_exposureEdit, _paramBoxLayout, row, tr("Exposure"), "[ms]");
    gui::putInGrid(_cooldownTimeEdit, _paramBoxLayout, row, tr("Cooldown time"), "[ms]");
    gui::putInGrid(_cooldownPwrEdit, _paramBoxLayout, row, tr("Cooldown power"), "[mW]");
}

//------------------------------------------------------------------------------

void BaseParameterPane::updateState(bool isAppReady)
{
    _exposureEdit->setEnabled(isAppReady);
    _cooldownTimeEdit->setEnabled(isAppReady);
    _cooldownPwrEdit->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

bool BaseParameterPane::areParametersValid() const
{
    return _exposureEdit->isValid() &&
           _cooldownTimeEdit->isValid() &&
           _cooldownPwrEdit->isValid();
}

//------------------------------------------------------------------------------

static const char *exposureLbl = "exposure [ms]";
static const char *cooldownTimeLbl = "cooldown time[ms]";
static const char *cooldownPwrLbl = "cooldown power[mW]";

void BaseParameterPane::persiste(QSettings &settings) const
{
    if (_exposureEdit->isValid())
        settings.setValue(exposureLbl, _exposureEdit->text());
    if (_cooldownTimeEdit->isValid())
        settings.setValue(cooldownTimeLbl, _cooldownTimeEdit->text());
    if (_cooldownPwrEdit->isValid())
        settings.setValue(cooldownPwrLbl, _cooldownPwrEdit->text());
}

//------------------------------------------------------------------------------

void BaseParameterPane::restore(QSettings &settings)
{
    _exposureEdit->setText(settings.value(exposureLbl).toString());
    _cooldownTimeEdit->setText(settings.value(cooldownTimeLbl).toString());
    _cooldownPwrEdit->setText(settings.value(cooldownPwrLbl).toString());
}

//------------------------------------------------------------------------------

}
