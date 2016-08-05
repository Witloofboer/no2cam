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

ConfigurationDlg::ConfigurationDlg(MainWindow* mainWindow)
    : QDialog(mainWindow)
    , _mainWindow(mainWindow)
    , _cutAngleEdit(new DoubleLineEdit(7, 2, 1))
    , _incidentEdit(new DoubleLineEdit(7, 2, 1))
    , _heightEdit(new DoubleLineEdit(7, 2, 1))
    , _lengthEdit(new DoubleLineEdit(7, 2, 1))
    , _buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel |
                                      QDialogButtonBox::Abort))
    , _crystal()
{
    setWindowTitle(tr("Configuration"));

    auto crystalGrid = new QGridLayout();

    int row = 0;
    putInGrid(_cutAngleEdit, crystalGrid, row++, "Cut angle", "[deg]");
    putInGrid(_incidentEdit, crystalGrid, row++, "Incident angle", "[deg]");
    putInGrid(_heightEdit, crystalGrid, row++, "Transducer height", "[mm]");
    putInGrid(_lengthEdit, crystalGrid, row++, "Transducer length", "[mm]");

    auto crystalBox = new QGroupBox(tr("Crystal parameters"));
    crystalBox->setLayout(crystalGrid);

    connect(_buttonBox, QDialogButtonBox::accepted, this, accept);
    connect(_buttonBox, QDialogButtonBox::rejected, this, reject);

    connect(_cutAngleEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_incidentEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_heightEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(_lengthEdit, LineEdit::textChanged, this, updateOkBtn);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(_buttonBox);
    setLayout(layout);

    restore();
}

//------------------------------------------------------------------------------

static const char *cutAngleLbl = "cut angle [deg]";
static const char *incidentLbl = "incident angle [deg]";
static const char *transHeightLbl = "transducer height [mm]";
static const char *transLengthLbl = "transducer length [mm]";

void ConfigurationDlg::persiste() const
{
    if (isValid())
    {
        qInfo("Persisting crystal parameters");

        QSettings settings;
        settings.beginGroup("Crystal");
        settings.setValue(cutAngleLbl, _cutAngleEdit->text());
        settings.setValue(incidentLbl, _incidentEdit->text());
        settings.setValue(transHeightLbl, _heightEdit->text());
        settings.setValue(transLengthLbl, _lengthEdit->text());
        settings.endGroup();
    } else {
        qWarning("Not persisting crystal parameters (invalid configuration)");
    }
}

//------------------------------------------------------------------------------

void ConfigurationDlg::restore()
{
    qInfo("Restoring crystal parameters");

    QSettings settings;
    settings.beginGroup("Crystal");
    _cutAngleEdit->setText(settings.value(cutAngleLbl, "").toString());
    _incidentEdit->setText(settings.value(incidentLbl, "").toString());
    _heightEdit->setText(settings.value(transHeightLbl, "").toString());
    _lengthEdit->setText(settings.value(transLengthLbl, "").toString());
    settings.endGroup();

    if (isValid())
        pushToCrystal();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::display(bool abortEnabled)
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
        pushToCrystal();        // Accepted
        pushToGui();            // Normalisation
        emit crystalUpdated();
    }
    else if (abortEnabled)
    {
        _mainWindow->close();    // Aborted
    }
    else
    {
        pushToGui();            // Cancelled
    }
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
           _lengthEdit->isValid();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::pushToGui()
{
    _cutAngleEdit->setValue(_crystal.cutAngle());
    _incidentEdit->setValue(_crystal.lightAngle());
    _heightEdit->setValue(_crystal.transHeight());
    _lengthEdit->setValue(_crystal.transLength());
}

//------------------------------------------------------------------------------

void ConfigurationDlg::pushToCrystal()
{
    Q_ASSERT(isValid());

    _crystal.set(_cutAngleEdit->value(),
                _incidentEdit->value(),
                _heightEdit->value(),
                _lengthEdit->value());
}

//------------------------------------------------------------------------------

}
