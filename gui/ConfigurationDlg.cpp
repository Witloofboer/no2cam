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
    , cutAngle(new DoubleLineEdit(7, 2, 1))
    , incidentAngle(new DoubleLineEdit(7, 2, 1))
    , transHeight(new DoubleLineEdit(7, 2, 1))
    , transLength(new DoubleLineEdit(7, 2, 1))
    , buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel))
    , myCrystal()
{
    setWindowTitle(tr("Configuration"));

    auto crystalGrid = new QGridLayout();

    int row = 0;
    putInGrid(cutAngle, crystalGrid, row++, "Cut angle", "[deg]");
    putInGrid(incidentAngle, crystalGrid, row++, "Incident angle", "[deg]");
    putInGrid(transHeight, crystalGrid, row++, "Transducer height", "[mm]");
    putInGrid(transLength, crystalGrid, row++, "Transducer length", "[mm]");

    auto crystalBox = new QGroupBox(tr("Crystal parameters"));
    crystalBox->setLayout(crystalGrid);

    connect(buttonBox, QDialogButtonBox::accepted, this, accept);
    connect(buttonBox, QDialogButtonBox::rejected, this, reject);
    connect(cutAngle, LineEdit::focusLost, this, updateDlgBtns);
    connect(incidentAngle, LineEdit::focusLost, this, updateDlgBtns);
    connect(transHeight, LineEdit::focusLost, this, updateDlgBtns);
    connect(transLength, LineEdit::focusLost, this, updateDlgBtns);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox);
    setLayout(layout);

    // Initial population
    myCrystal.restore();
    pushParamsToGui();
}

//------------------------------------------------------------------------------

void ConfigurationDlg::display()
{
    auto code = exec();

    if (code == DialogCode::Accepted)
    {
        if (myCrystal.cutAngle() != cutAngle->value() ||
            myCrystal.lightAngle() != incidentAngle->value() ||
            myCrystal.transHeight() != transHeight->value() ||
            myCrystal.transLength() != transLength->value())
        {
            myCrystal.set(cutAngle->value(),
                        incidentAngle->value(),
                        transHeight->value(),
                        transLength->value());
            myCrystal.persiste();
            emit parametersUpdated();
        }
    } else {
        pushParamsToGui();
    }
}

//------------------------------------------------------------------------------

void ConfigurationDlg::updateDlgBtns()
{
    const bool enabled =
            cutAngle->isValid() &&
            incidentAngle->isValid() &&
            transHeight->isValid() &&
            transLength->isValid();

    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

//------------------------------------------------------------------------------

void ConfigurationDlg::pushParamsToGui()
{
    cutAngle->setValue(myCrystal.cutAngle());
    incidentAngle->setValue(myCrystal.lightAngle());
    transHeight->setValue(myCrystal.transHeight());
    transLength->setValue(myCrystal.transLength());
}

//------------------------------------------------------------------------------

}
