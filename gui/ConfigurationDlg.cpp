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
#include "core/Core.h"
#include "core/core_lib.h"
#include "MainWindow.h"
#include "gui_lib.h"
#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ConfigurationDlg::ConfigurationDlg(MainWindow* mainWindow)
    : QDialog(mainWindow)
    , mainWindow(mainWindow)
    , cutAngleEdit(new DoubleLineEdit(7, 2, 1))
    , incidentEdit(new DoubleLineEdit(7, 2, 1))
    , transHeightEdit(new DoubleLineEdit(7, 2, 1))
    , transLengthEdit(new DoubleLineEdit(7, 2, 1))
    , buttonBox(new QDialogButtonBox( QDialogButtonBox::Ok
                                    | QDialogButtonBox::Cancel
                                    | QDialogButtonBox::Abort))
    , crystal()
{
    setWindowTitle(tr("Configuration"));

    auto crystalGrid = new QGridLayout();

    int row = 0;
    putInGrid(cutAngleEdit, crystalGrid, row++, "Cut angle", "[deg]");
    putInGrid(incidentEdit, crystalGrid, row++, "Incident angle", "[deg]");
    putInGrid(transHeightEdit, crystalGrid, row++, "Transducer height", "[mm]");
    putInGrid(transLengthEdit, crystalGrid, row++, "Transducer length", "[mm]");

    auto crystalBox = new QGroupBox(tr("Crystal parameters"));
    crystalBox->setLayout(crystalGrid);

    connect(buttonBox, QDialogButtonBox::accepted, this, accept);
    connect(buttonBox, QDialogButtonBox::rejected, this, reject);

    connect(cutAngleEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(incidentEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(transHeightEdit, LineEdit::textChanged, this, updateOkBtn);
    connect(transLengthEdit, LineEdit::textChanged, this, updateOkBtn);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox);
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
        settings.setValue(cutAngleLbl, cutAngleEdit->text());
        settings.setValue(incidentLbl, incidentEdit->text());
        settings.setValue(transHeightLbl, transHeightEdit->text());
        settings.setValue(transLengthLbl, transLengthEdit->text());
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
    cutAngleEdit->setText(settings.value(cutAngleLbl, "").toString());
    incidentEdit->setText(settings.value(incidentLbl, "").toString());
    transHeightEdit->setText(settings.value(transHeightLbl, "").toString());
    transLengthEdit->setText(settings.value(transLengthLbl, "").toString());
    settings.endGroup();

    if (isValid())
        pushToCrystal();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::display(bool abortEnabled)
{
    buttonBox->button(QDialogButtonBox::Cancel)->setVisible(!abortEnabled);
    buttonBox->button(QDialogButtonBox::Abort)->setVisible(abortEnabled);
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
        mainWindow->close();    // Aborted
    }
    else
    {
        pushToGui();            // Cancelled
    }
}

//------------------------------------------------------------------------------

void ConfigurationDlg::updateOkBtn()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid());
}

//------------------------------------------------------------------------------

bool ConfigurationDlg::isValid() const
{
    return cutAngleEdit->isValid() &&
           incidentEdit->isValid() &&
           transHeightEdit->isValid() &&
           transLengthEdit->isValid();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::pushToGui()
{
    cutAngleEdit->setValue(crystal.cutAngle());
    incidentEdit->setValue(crystal.lightAngle());
    transHeightEdit->setValue(crystal.transHeight());
    transLengthEdit->setValue(crystal.transLength());
}

//------------------------------------------------------------------------------

void ConfigurationDlg::pushToCrystal()
{
    Q_ASSERT(isValid());

    crystal.set(cutAngleEdit->value(),
                incidentEdit->value(),
                transHeightEdit->value(),
                transLengthEdit->value());
}

//------------------------------------------------------------------------------

}
