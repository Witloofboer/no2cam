#include "ConfigurationDlg.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>


ConfigurationDlg::ConfigurationDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Configuration"));

    auto buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

    connect(buttonBox, QDialogButtonBox::accepted, this, QDialog::accept);
    connect(buttonBox, QDialogButtonBox::rejected, this, QDialog::reject);

    auto layout = new QVBoxLayout;
    layout->addWidget(new QLabel("<b>Parameters will come here...</b>"));
    layout->addWidget(buttonBox);
    setLayout(layout);
}
