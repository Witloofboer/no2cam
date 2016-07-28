#ifndef TOOLING_H
#define TOOLING_H

#include <QLineEdit>
#include <QDoubleValidator>

class QString;
class QGridLayout;

namespace gui {

//==============================================================================
// LineEdit
//==============================================================================

class LineEdit: public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(int length=19, const QString& regexp = ".*");

    bool isValid();
};


//==============================================================================
// IntLineEdit
//==============================================================================

class IntLineEdit: public LineEdit
{
    Q_OBJECT

public:
    IntLineEdit(int length=7, int nDgts=5);

    bool isValid();
    void setValue(int value);
    int value();
};


//==============================================================================
// DoubleLineEdit
//==============================================================================

class DoubleLineEdit: public LineEdit
{
    Q_OBJECT

public:
    DoubleLineEdit(int length=7, int nIntDgts=3, int nFracDgts=1);

    bool isValid();
    void setValue(double value);
    double value();

private:
    int nFracDgts_;
};


//==============================================================================

void putInGrid(QWidget* widget,
               QGridLayout* grid,
               int row,
               const QString &label,
               const QString &unit);

//==============================================================================

}

#endif // TOOLING_H
