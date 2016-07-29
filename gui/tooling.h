#ifndef TOOLING_H
#define TOOLING_H

#include <QLineEdit>

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

signals:
    void focusLost();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
};


//==============================================================================
// IntLineEdit
//==============================================================================

class IntLineEdit: public LineEdit
{
    Q_OBJECT

public:
    IntLineEdit(int length=9, int nDgts=5);

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
    DoubleLineEdit(int length=9, int nIntDgts=3, int nFracDgts=1);

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
