#include "ConfigurationDlg.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "tooling.h"

ConfigurationDlg::ConfigurationDlg(QWidget *parent)
    : QDialog(parent)
    , cutAngle_(new_NumberEdit(" 09.9"))
    , IncidentAngle_(new_NumberEdit(" 09.9"))
    , transHeight_(new_NumberEdit(" 09.9"))
    , transLength_(new_NumberEdit(" 09.9"))
    , elasticCoeff_(new_NumberEdit("-09.9"))
{
    setWindowTitle(tr("Configuration"));

    auto crystalGrid = new QGridLayout();

    int row = 0;
    putInGrid(cutAngle_, crystalGrid, row++, "Cut angle", "[deg]");
    putInGrid(IncidentAngle_, crystalGrid, row++, "Incident angle", "[deg]");
    putInGrid(transHeight_, crystalGrid, row++, "Transducer height", "[mm]");
    putInGrid(transLength_, crystalGrid, row++, "Transducer length", "[mm]");
    putInGrid(elasticCoeff_, crystalGrid, row++, "Photo-elastic coeff", "x 10<sup>-3</sup>");

    auto crystalBox = new QGroupBox(tr("Crystal parameters"));
    crystalBox->setLayout(crystalGrid);


    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Cancel);

    connect(buttonBox, QDialogButtonBox::accepted, this, QDialog::accept);
    connect(buttonBox, QDialogButtonBox::rejected, this, QDialog::reject);

    auto layout = new QVBoxLayout;
    layout->addWidget(crystalBox);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
