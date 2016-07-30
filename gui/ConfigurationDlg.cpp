#include "ConfigurationDlg.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
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
    , buttonBox_(new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel))
    , crystal()
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

    connect(buttonBox_, QDialogButtonBox::accepted, this, accept);
    connect(buttonBox_, QDialogButtonBox::rejected, this, reject);
    connect(cutAngle_, LineEdit::focusLost, this, updateDlgBtns);
    connect(incidentAngle_, LineEdit::focusLost, this, updateDlgBtns);
    connect(transHeight_, LineEdit::focusLost, this, updateDlgBtns);
    connect(transLength_, LineEdit::focusLost, this, updateDlgBtns);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox_);
    setLayout(layout);

    // Initial population
    crystal.restore();
    pushParamsToGui();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::display()
{
    auto code = QDialog::exec();

    if (code == DialogCode::Accepted)
    {
        if (crystal.cutAngle() != cutAngle_->value() ||
            crystal.lightAngle() != incidentAngle_->value() ||
            crystal.transHeight() != transHeight_->value() ||
            crystal.transLength() != transLength_->value())
        {
            crystal.set(cutAngle_->value(),
                        incidentAngle_->value(),
                        transHeight_->value(),
                        transLength_->value());
            crystal.persiste();
        }
    } else {
        pushParamsToGui();
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

void ConfigurationDlg::pushParamsToGui()
{
    cutAngle_->setValue(crystal.cutAngle());
    incidentAngle_->setValue(crystal.lightAngle());
    transHeight_->setValue(crystal.transHeight());
    transLength_->setValue(crystal.transLength());
}

//------------------------------------------------------------------------------

}
