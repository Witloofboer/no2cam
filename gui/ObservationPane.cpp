#include "ObservationPane.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>

#include "tooling.h"
#include "ConfigurationDlg.h"

namespace gui {

//------------------------------------------------------------------------------

ObservationPane::ObservationPane(MainWindow* mainWindow,
                                 const double &stabilisationTime)
    : AbstractMainPane(mainWindow)
    , _wavelength1Edit(new DoubleLineEdit)
    , _wavelength2Edit(new DoubleLineEdit)
    , _snapPerObsEdit(new IntLineEdit)
    , _stabilisationTime(stabilisationTime)
{
    // Parameter box -----------------------------------------------------------

    int row=0;

    gui::putInGrid(_wavelength1Edit, _paramBoxLayout, row, tr("Wavelength 1"), "[nm]");
    gui::putInGrid(_wavelength2Edit, _paramBoxLayout, row, tr("Wavelength 2"), "[nm]");
    gui::putInGrid(_snapPerObsEdit, _paramBoxLayout, row, tr("Snapshots/obs"), "");

    AbstractMainPane::putInGrid(row);

    _paramBoxLayout->addWidget(new QLabel("Session:"), row, 0);
    _paramBoxLayout->addWidget(_sessionEdit, row, 1, 1, 2);
    ++row;

    // Adapt the AbstractMainPane base
    _snapshotBox->setTitle(tr("Observation"));

    // Connectors
    connect(_wavelength1Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_wavelength2Edit, LineEdit::textChanged, this, refreshBtns);
    connect(_snapPerObsEdit, LineEdit::textChanged, this, refreshBtns);

    restore();
}

//------------------------------------------------------------------------------

void ObservationPane::start(bool burst, bool record)
{
    emit observationRequested(_wavelength1Edit->value(),
                              _wavelength2Edit->value(),
                              _exposureEdit->value(),
                              _snapPerObsEdit->value(),
                              _cooldownTimeEdit->value(),
                              _cooldownPwrEdit->value(),
                              _stabilisationTime,
                              burst,
                              record ? _sessionEdit->text() : "");
}

//------------------------------------------------------------------------------

bool ObservationPane::areParametersValid() const
{
    return _wavelength1Edit->isValid() &&
           _wavelength2Edit->isValid() &&
           _snapPerObsEdit->isValid() &&
           AbstractMainPane::areParametersValid();
}

//------------------------------------------------------------------------------

static const char *wavelength1Lbl = "Wavelength 1 [nm]";
static const char *wavelength2Lbl = "Wavelength 2 [nm]";
static const char *snapPerObsLbl = "Snapshops per observation [nm]";

void ObservationPane::persiste() const
{
    qInfo("Persisting observation parameters");

    QSettings settings;

    settings.beginGroup("Observations");

    AbstractMainPane::persiste(settings);

    if (_wavelength1Edit->isValid())
        settings.setValue(wavelength1Lbl, _wavelength1Edit->text());
    if (_wavelength2Edit->isValid())
        settings.setValue(wavelength2Lbl, _wavelength2Edit->text());
    if (_snapPerObsEdit->isValid())
        settings.setValue(snapPerObsLbl, _snapPerObsEdit->text());

    settings.endGroup();
}

//------------------------------------------------------------------------------

void ObservationPane::restore()
{
    qInfo("Restoring observation parameters");

    QSettings settings;

    settings.beginGroup("Observations");

    AbstractMainPane::restore(settings);

    _wavelength1Edit->setText(settings.value(wavelength1Lbl).toString());
    _wavelength2Edit->setText(settings.value(wavelength2Lbl).toString());
    _snapPerObsEdit->setText(settings.value(snapPerObsLbl).toString());
    settings.endGroup();

    refreshBtns();
}

//------------------------------------------------------------------------------

}
