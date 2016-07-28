#include "ConfigurationDlg.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "core/Crystal.h"
#include "core/Core.h"
#include "core/core_lib.h"
#include "tooling.h"

namespace gui {

//------------------------------------------------------------------------------

ConfigurationDlg::ConfigurationDlg(QWidget *parent)
    : QDialog(parent)
    , cutAngle_(new DoubleLineEdit(7, 2, 1))
    , incidentAngle_(new DoubleLineEdit(7, 2, 1))
    , transHeight_(new DoubleLineEdit(7, 2, 1))
    , transLength_(new DoubleLineEdit(7, 2, 1))
    , params_(new core::CrystalParameters)
    , buttonBox_(new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel))
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

    connect(buttonBox_, QDialogButtonBox::accepted, this, QDialog::accept);
    connect(buttonBox_, QDialogButtonBox::rejected, this, QDialog::reject);
    connect(this, ConfigurationDlg::parametersUpdated,
            core::singleton(), core::Core::setParameters);

    connect(cutAngle_, DoubleLineEdit::textEdited,
            this, ConfigurationDlg::updateDlgBtns);
    connect(incidentAngle_, DoubleLineEdit::textEdited,
            this, ConfigurationDlg::updateDlgBtns);
    connect(transHeight_, DoubleLineEdit::textEdited,
            this, ConfigurationDlg::updateDlgBtns);
    connect(transLength_, DoubleLineEdit::textEdited,
            this, ConfigurationDlg::updateDlgBtns);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox_);
    setLayout(layout);

    // Initial population
    params_->restore();
    cutAngle_->setValue(params_->alpha_deg);
    incidentAngle_->setValue(params_->theta_deg);
    transHeight_->setValue(params_->transHeight);
    transLength_->setValue(params_->transLength);
    emit parametersUpdated(*params_);
}

//------------------------------------------------------------------------------

ConfigurationDlg::~ConfigurationDlg()
{
    delete params_;
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

void ConfigurationDlg::updateDlgBtns()
{
    const bool enabled =
            cutAngle_->isValid() &&
            incidentAngle_->isValid() &&
            transHeight_->isValid() &&
            transLength_->isValid();

    buttonBox_->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

//------------------------------------------------------------------------------

}
