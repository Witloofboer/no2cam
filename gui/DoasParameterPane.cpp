#include "DoasParameterPane.h"
#include "MainWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QVector>

#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

DoasParameterPane::DoasParameterPane(MainWindow *mainWindow)
    : BaseParameterPane(mainWindow)
    , _snapPerObsEdit(new IntLineEdit)
    , _wlFileEdit(new LineEdit(25, QString(".*")))
    , _wlFileBtn(new QPushButton)
{
    // Parameter box -----------------------------------------------------------

    _parameterBox->setTitle(tr("DOAS mode"));

    _wlFileBtn->setText(QString("Wavelengths"));

    int row=0;
    gui::putInGrid(_snapPerObsEdit, _paramBoxLayout, row, tr("Snapshots/obs"), "");

    _paramBoxLayout->addWidget(_wlFileBtn, row, 0);
    _paramBoxLayout->addWidget(_wlFileEdit, row, 1, 1, 2);
    ++row;

    BaseParameterPane::putInGrid(row);


    // Connectors
    connect(_snapPerObsEdit, LineEdit::textChanged, this, parametersChanged);
    connect(_wlFileBtn, QPushButton::clicked, this, selectWavelengthFile);

    restore();
}

//------------------------------------------------------------------------------

void DoasParameterPane::updateState(bool isAppReady)
{
    BaseParameterPane::updateState(isAppReady);

    _snapPerObsEdit->setEnabled(isAppReady);
    _wlFileEdit->setEnabled(isAppReady);
    _wlFileBtn->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

void DoasParameterPane::selectWavelengthFile()
{
    QString path(_wlFileEdit->text());

    if (path.isEmpty())
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    path = QFileDialog::getOpenFileName(this,
        tr("Select Wavelength File"), path, tr("All Files (*.*)"));

    if (!path.isNull())
        _wlFileEdit->setText(path);
}

//------------------------------------------------------------------------------

void DoasParameterPane::start(bool burst,
                              bool record,
                              double refWavelength,
                              double exposureFactor,
                              double stabilisationTime,
                              const QString &session,
                              const QString &dataFolder)
{      
    const QString filename = _wlFileEdit->text();
    const QFileInfo fileInfo(filename);

    if (! fileInfo.isFile())
    {
        qWarning() << "No file '" << _wlFileEdit->text() << "'" << endl;

        _mainWindow->onDisplayWarning(QString("<p><b>Error</b>: no wavelength file '%1'.</p>").arg(filename));
        _mainWindow->onUpdateApplicationReadiness(true);
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Error opening file '" << _wlFileEdit->text() << "'" << endl;
        _mainWindow->onDisplayWarning(QString("<p><b>Error</b>: error opening wavelength file '%1'.</p>").arg(filename));
        _mainWindow->onUpdateApplicationReadiness(true);
        return;
    }

    QVector<double> wavelengths;

    QTextStream in(&file);
    bool ok;
    int c=0;
    while (!in.atEnd()) {
        ++c;
        QString line = in.readLine();
        line = line.trimmed();
        int lastIndex = line.indexOf('#');
        if (lastIndex >= 0) line = line.left(lastIndex);
        if (line.isEmpty()) continue;
        double wl = line.toDouble(&ok);
        if (!ok)
        {
            qWarning() << "'" << line << "' is not valid as a double floating value" << endl;
            _mainWindow->onDisplayWarning(QString("<p><b>Error</b>: '%1' is not valid as double floating value (line %2 of %3).</p>").arg(line).arg(c));
            _mainWindow->onUpdateApplicationReadiness(true);
            return;
        }
        qDebug("Adding wavelength %f (nm)", wl);
        wavelengths.push_back(wl);
    }

    if (wavelengths.isEmpty())
    {
        qWarning() << "'" << filename << "' contains no wavelength" << endl;
        _mainWindow->onDisplayWarning(QString("<p><b>Error</b>: '%1' contains no wavelength.</p>").arg(filename));
        _mainWindow->onUpdateApplicationReadiness(true);
        return;
    }

    emit doasRequested(wavelengths,
                       _snapPerObsEdit->value(),
                       _exposureEdit->value(),
                       _cooldownTimeEdit->value(),
                       refWavelength,
                       exposureFactor,
                       stabilisationTime,
                       burst,
                       record,
                       dataFolder,
                       session);
}

//------------------------------------------------------------------------------

bool DoasParameterPane::areParametersValid() const
{
    return ~_wlFileEdit->text().isEmpty() &&
           _snapPerObsEdit->isValid() &&
           BaseParameterPane::areParametersValid();
}

//------------------------------------------------------------------------------

static const char *wlFileLbl = "Wavelength file";
static const char *snapPerObsLbl = "Snapshops per observation [nm]";

void DoasParameterPane::persiste() const
{
    qDebug("Persisting DOAS parameters");

    QSettings settings;

    settings.beginGroup("DOAS");

    BaseParameterPane::persiste(settings);

    settings.setValue(wlFileLbl, _wlFileEdit->text());
    if (_snapPerObsEdit->isValid())
        settings.setValue(snapPerObsLbl, _snapPerObsEdit->text());

    settings.endGroup();
}

//------------------------------------------------------------------------------

void DoasParameterPane::restore()
{
    qDebug("Restoring DOAS parameters");

    QSettings settings;

    settings.beginGroup("DOAS");

    BaseParameterPane::restore(settings);

    _wlFileEdit->setText(settings.value(wlFileLbl).toString());
    _snapPerObsEdit->setText(settings.value(snapPerObsLbl).toString());
    settings.endGroup();

    emit parametersChanged();
}

//------------------------------------------------------------------------------

}
