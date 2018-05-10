#include "ConfigurationDlg.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include "core/Crystal.h"
#include "MainWindow.h"
#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ConfigurationDlg::ConfigurationDlg(MainWindow* mainWindow,
                                   core::Crystal *crystal)
    : QDialog(mainWindow)
    , _mainWindow(mainWindow)
    , _cutAngleEdit(new DoubleLineEdit(7, 2, 2))
    , _incidentEdit(new DoubleLineEdit(7, 2, 2))
    , _heightEdit(new DoubleLineEdit(7, 2, 2))
    , _lengthEdit(new DoubleLineEdit(7, 2, 2))
    , _doasRefWlEdit(new DoubleLineEdit(7, 3, 1))
    , _doasExposureFactorEdit(new DoubleLineEdit(7, 2, 2))
    , _stabilisationTimeEdit(new IntLineEdit(7))
    , _temperaturePeriodEdit(new PosIntLineEdit(7))
    , _buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel |
                                      QDialogButtonBox::Abort))
    , _crystal(crystal)
{
    setWindowTitle(tr("Configuration"));

    //----------------------------------------

    auto crystalGrid = new QGridLayout();

    int row = 0;
    putInGrid(_cutAngleEdit, crystalGrid, row, "Cut angle", "[deg]");
    putInGrid(_incidentEdit, crystalGrid, row, "Incident angle", "[deg]");
    putInGrid(_heightEdit, crystalGrid, row, "Transducer height", "[mm]");
    putInGrid(_lengthEdit, crystalGrid, row, "Transducer length", "[mm]");

    auto crystalBox = new QGroupBox(tr("Crystal parameters"));
    crystalBox->setLayout(crystalGrid);

    //----------------------------------------

    auto doasGrid = new QGridLayout();

    row = 0;
    putInGrid(_doasRefWlEdit, doasGrid, row, "Ref. wavelength", "[nm]");
    putInGrid(_doasExposureFactorEdit, doasGrid, row, "Exposure factor", "[% nm-1]");

    auto doasBox = new QGroupBox(tr("DOAS parameters"));
    doasBox->setLayout(doasGrid);

    //----------------------------------------

    auto paramGrid = new QGridLayout();
    row = 0;
    putInGrid(_stabilisationTimeEdit, paramGrid, row, "Stabilisation", "[ms]");
    putInGrid(_temperaturePeriodEdit, paramGrid, row, "Thermometer period", "[ms]");

    paramGrid->setColumnMinimumWidth(
        0,
        crystalGrid->itemAtPosition(2,0)->minimumSize().width());

    auto boardBox = new QGroupBox(tr("Functional parameters"));
    boardBox->setLayout(paramGrid);

    //----------------------------------------

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(doasBox);
    layout->addWidget(boardBox);
    layout->addWidget(_buttonBox);
    setLayout(layout);

    //----------------------------------------

    connect(_buttonBox, QDialogButtonBox::accepted, this, accept);
    connect(_buttonBox, QDialogButtonBox::rejected, this, reject);

    connect(_cutAngleEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_incidentEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_heightEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_lengthEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_doasRefWlEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_doasExposureFactorEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_stabilisationTimeEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_temperaturePeriodEdit, LineEdit::textChanged, this, updateOkBtn);

    //----------------------------------------

    restore();
}

//------------------------------------------------------------------------------

static const char *cutAngleLbl = "cut angle [deg]";
static const char *incidentLbl = "incident angle [deg]";
static const char *transHeightLbl = "transducer height [mm]";
static const char *transLengthLbl = "transducer length [mm]";
static const char *doasRefWlLbl = "DOAS reference wavelength [nm]";
static const char *doasExposureFactorLbl = "DOAS exposure factor [% nm-1]";
static const char *stabilisingTimeLbl = "board stabilisation [ms]";
static const char *temperaturePeriodLbl = "thermometer period [ms]";

//------------------------------------------------------------------------------

void ConfigurationDlg::persiste() const
{
    if (isValid())
    {
        qDebug("Persisting parameters");

        QSettings settings;
        settings.beginGroup("Crystal");
        settings.setValue(cutAngleLbl, _cutAngleEdit->text());
        settings.setValue(incidentLbl, _incidentEdit->text());
        settings.setValue(transHeightLbl, _heightEdit->text());
        settings.setValue(transLengthLbl, _lengthEdit->text());
        settings.endGroup();

        settings.beginGroup("DOAS");
        settings.setValue(doasRefWlLbl, _doasRefWlEdit->text());
        settings.setValue(doasExposureFactorLbl, _doasExposureFactorEdit->text());
        settings.endGroup();

        settings.beginGroup("Parameters");
        settings.setValue(stabilisingTimeLbl, _stabilisationTimeEdit->text());
        settings.setValue(temperaturePeriodLbl, _temperaturePeriodEdit->text());
        settings.endGroup();

    } else {
        qWarning("Not persisting parameters (invalid configuration)");
    }
}

//------------------------------------------------------------------------------

void ConfigurationDlg::restore()
{
    qDebug("Restoring parameters");

    QSettings settings;
    settings.beginGroup("Crystal");
    _cutAngleEdit->setText(settings.value(cutAngleLbl, "").toString());
    _incidentEdit->setText(settings.value(incidentLbl, "").toString());
    _heightEdit->setText(settings.value(transHeightLbl, "").toString());
    _lengthEdit->setText(settings.value(transLengthLbl, "").toString());
    settings.endGroup();

    settings.beginGroup("DOAS");
    _doasRefWlEdit->setText(settings.value(doasRefWlLbl,  "500").toString());
    _doasExposureFactorEdit->setText(settings.value(doasExposureFactorLbl, "0").toString());
    _doasRefWl = _doasRefWlEdit->value();
    _doasExposureFactor = _doasExposureFactorEdit->value();
    settings.endGroup();


    settings.beginGroup("Parameters");
    _stabilisationTimeEdit->setText(settings.value(stabilisingTimeLbl,  "5").toString());
    _temperaturePeriodEdit->setText(settings.value(temperaturePeriodLbl, "1000").toString());
    _stabilisingTime = _stabilisationTimeEdit->value();
    _temperaturePeriod = _temperaturePeriodEdit->value();
    settings.endGroup();

    if (isValid())
        ackGuiValues();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::onDisplay(bool abortEnabled)
{
    _buttonBox->button(QDialogButtonBox::Cancel)->setVisible(!abortEnabled);
    _buttonBox->button(QDialogButtonBox::Abort)->setVisible(abortEnabled);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (abortEnabled)
        setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    else
        setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);


    updateOkBtn();
    int result = exec();

    if (result==Accepted)
    {
        ackGuiValues();
        restoreGuiValues();
    }
    else if (abortEnabled)
    {
        _mainWindow->close();    // Aborted
    }
    else
    {
        restoreGuiValues();      // Cancelled
    }
}

//------------------------------------------------------------------------------

double ConfigurationDlg::doasRefWavelength() const
{
    return _doasRefWl;
}

//------------------------------------------------------------------------------

double ConfigurationDlg::doasExposureFactor() const
{
    return _doasExposureFactor;
}

//------------------------------------------------------------------------------

int ConfigurationDlg::stabilisingTime() const
{
    return _stabilisingTime;
}

//------------------------------------------------------------------------------

int ConfigurationDlg::temperaturePeriod() const
{
    return _temperaturePeriod;
}

//------------------------------------------------------------------------------

void ConfigurationDlg::updateOkBtn()
{
    _buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid());
}

//------------------------------------------------------------------------------

bool ConfigurationDlg::isValid() const
{
    return _cutAngleEdit->isValid() &&
           _incidentEdit->isValid() &&
           _heightEdit->isValid() &&
           _lengthEdit->isValid() &&
           _doasRefWlEdit->isValid() &&
           _doasExposureFactorEdit->isValid() &&
           _stabilisationTimeEdit->isValid() &&
           _temperaturePeriodEdit->isValid();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::restoreGuiValues()
{
    _cutAngleEdit->setValue(_crystal->cutAngle());
    _incidentEdit->setValue(_crystal->incidentAngle());
    _heightEdit->setValue(_crystal->transHeight());
    _lengthEdit->setValue(_crystal->transLength());

    _doasRefWlEdit->setValue(_doasRefWl);
    _doasExposureFactorEdit->setValue(_doasExposureFactor);

    _stabilisationTimeEdit->setValue(_stabilisingTime);
    _temperaturePeriodEdit->setValue(_temperaturePeriod);

}

//------------------------------------------------------------------------------

void ConfigurationDlg::ackGuiValues()
{
    Q_ASSERT(isValid());

    _crystal->set(_cutAngleEdit->value(),
                  _incidentEdit->value(),
                  _heightEdit->value(),
                  _lengthEdit->value());

    _doasRefWl = _doasRefWlEdit->value();
    _doasExposureFactor = _doasExposureFactorEdit->value();
    _stabilisingTime = _stabilisationTimeEdit->value();
    _temperaturePeriod = _temperaturePeriodEdit->value();

    emit parametersUpdated();
}

//------------------------------------------------------------------------------

}
