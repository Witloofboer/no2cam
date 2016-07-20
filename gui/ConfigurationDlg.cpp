#include "ConfigurationDlg.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

#include "core/Crystal.h"
#include "core/Core.h"
#include "tooling.h"

ConfigurationDlg::ConfigurationDlg(QWidget *parent)
    : QDialog(parent)
    , cutAngle_(new DoubleLineEdit(" 09.9"))
    , incidentAngle_(new DoubleLineEdit(" 09.9"))
    , transHeight_(new DoubleLineEdit(" 09.9"))
    , transLength_(new DoubleLineEdit(" 09.9"))
{
    setWindowTitle(tr("Configuration"));

    auto crystalGrid = new QGridLayout();

    int row = 0;
    putInGrid(cutAngle_, crystalGrid, row++, "Cut angle", "[deg]");
    putInGrid(incidentAngle_, crystalGrid, row++, "Incident angle", "[deg]");
    putInGrid(transHeight_, crystalGrid, row++, "Transducer height", "[mm]");
    putInGrid(transLength_, crystalGrid, row++, "Transducer length", "[mm]");

    auto crystalBox = new QGroupBox(tr("Crystal parameters"));
    crystalBox->setLayout(crystalGrid);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Cancel);

    connect(buttonBox, QDialogButtonBox::accepted, this, QDialog::accept);
    connect(buttonBox, QDialogButtonBox::rejected, this, QDialog::reject);

    connect(this,   ConfigurationDlg::parametersUpdated,
            &gCore, Core::updateParameters);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void ConfigurationDlg::display()
{
    CrystalParameters inParams;
    inParams.restore();

    cutAngle_->setValue(inParams.alpha_deg);
    incidentAngle_->setValue(inParams.theta_deg);
    transHeight_->setValue(inParams.transHeight);
    transLength_->setValue(inParams.transLength);

    auto code = QDialog::exec();

    if (code == DialogCode::Accepted)
    {
        CrystalParameters outParams;
        outParams.alpha_deg = cutAngle_->value();
        outParams.theta_deg = incidentAngle_->value();
        outParams.transHeight = transHeight_->value();
        outParams.transLength = transLength_->value();

        if (inParams != outParams)
        {
            outParams.persiste();
            emit parametersUpdated(outParams);
        }
    }
}
