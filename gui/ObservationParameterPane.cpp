#include "ObservationParameterPane.h"

#include <QGroupBox>
#include <QSettings>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ObservationParameterPane::ObservationParameterPane(MainWindow *mainWindow)
    : BaseParameterPane(mainWindow)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _snapPerObsEdit(new IntLineEdit)
{
    // Parameter box -----------------------------------------------------------
    _parameterBox->setTitle(tr("Observation mode"));

    int row=0;

    gui::putInGrid(_wavelength1Edit, _paramBoxLayout, row, tr("Wavelength 1"), "[nm]");
    gui::putInGrid(_wavelength2Edit, _paramBoxLayout, row, tr("Wavelength 2"), "[nm]");
    gui::putInGrid(_snapPerObsEdit, _paramBoxLayout, row, tr("Snapshots/obs"), "");

    BaseParameterPane::putInGrid(row);

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, parametersChanged);
    connect(_wavelength2Edit, LineEdit::textChanged, this, parametersChanged);
    connect(_snapPerObsEdit, LineEdit::textChanged, this, parametersChanged);

    restore();
}

//------------------------------------------------------------------------------

void ObservationParameterPane::updateState(bool isAppReady)
{
    BaseParameterPane::updateState(isAppReady);
    _wavelength1Edit->setEnabled(isAppReady);
    _wavelength2Edit->setEnabled(isAppReady);
    _snapPerObsEdit->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

void ObservationParameterPane::start(bool burst,
                                     bool record,
                                     double stabilisationTime,
                                     const QString &session,
                                     const QString &dataFolder)
{
    emit observationRequested(_wavelength1Edit->value(),
                              _wavelength2Edit->value(),
                              _snapPerObsEdit->value(),
                              _exposureEdit->value(),
                              _cooldownTimeEdit->value(),
                              stabilisationTime,
                              burst,
                              record,
                              dataFolder,
                              session);
}

//------------------------------------------------------------------------------

bool ObservationParameterPane::areParametersValid() const
{
    return _wavelength1Edit->isValid() &&
           _wavelength2Edit->isValid() &&
           _snapPerObsEdit->isValid() &&
           BaseParameterPane::areParametersValid();
}

//------------------------------------------------------------------------------

static const char *wavelength1Lbl = "Wavelength 1 [nm]";
static const char *wavelength2Lbl = "Wavelength 2 [nm]";
static const char *snapPerObsLbl = "Snapshops per observation [nm]";

void ObservationParameterPane::persiste() const
{
    qDebug("Persisting observation parameters");

    QSettings settings;

    settings.beginGroup("Observations");

    BaseParameterPane::persiste(settings);

    if (_wavelength1Edit->isValid())
        settings.setValue(wavelength1Lbl, _wavelength1Edit->text());
    if (_wavelength2Edit->isValid())
        settings.setValue(wavelength2Lbl, _wavelength2Edit->text());
    if (_snapPerObsEdit->isValid())
        settings.setValue(snapPerObsLbl, _snapPerObsEdit->text());

    settings.endGroup();
}

//------------------------------------------------------------------------------

void ObservationParameterPane::restore()
{
    qDebug("Restoring observation parameters");

    QSettings settings;

    settings.beginGroup("Observations");

    BaseParameterPane::restore(settings);

    _wavelength1Edit->setText(settings.value(wavelength1Lbl).toString());
    _wavelength2Edit->setText(settings.value(wavelength2Lbl).toString());
    _snapPerObsEdit->setText(settings.value(snapPerObsLbl).toString());
    settings.endGroup();

    emit parametersChanged();
}

//------------------------------------------------------------------------------

}
