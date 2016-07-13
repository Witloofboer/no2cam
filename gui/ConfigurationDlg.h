#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>

class QLineEdit;

class ConfigurationDlg : public QDialog
{
public:
    ConfigurationDlg(QWidget *parent=0);

private:
    QLineEdit* cutAngle;   // Cut angle
    QLineEdit* IncidentAngle;   // Incident angle
    QLineEdit* transHeight;       // Transducer height
    QLineEdit* transLength;       // Transducer length
    QLineEdit* elasticCoeff;     // Photo-elastic coeff.
};

#endif // CONFIGURATIONDLG_H
