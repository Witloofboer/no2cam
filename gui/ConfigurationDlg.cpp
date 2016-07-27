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
    , params_(new CrystalParameters)
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
    connect(this, ConfigurationDlg::parametersUpdated,
            Core::singleton(), Core::setParameters);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox);
    setLayout(layout);

    // Initial population
    params_->restore();
    cutAngle_->setValue(params_->alpha_deg);
    incidentAngle_->setValue(params_->theta_deg);
    transHeight_->setValue(params_->transHeight);
    transLength_->setValue(params_->transLength);
    emit parametersUpdated(*params_);
}

ConfigurationDlg::~ConfigurationDlg()
{
    delete params_;
}

void ConfigurationDlg::display()
{
    auto code = QDialog::exec();

    if (code == DialogCode::Accepted)
    {
        if (params_->alpha_deg != cutAngle_->value() ||
            params_->theta_deg != incidentAngle_->value() ||
            params_->transHeight != transHeight_->value() ||
            params_->transLength != transLength_->value())
        {
            params_->alpha_deg = cutAngle_->value();
            params_->theta_deg = incidentAngle_->value();
            params_->transHeight = transHeight_->value();
            params_->transLength = transLength_->value();
            params_->persiste();
            emit parametersUpdated(*params_);
        }
    }
}
