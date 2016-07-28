#include "tooling.h"

#include <QApplication>
#include <QColor>
#include <QFocusEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QMessageBox>
#include <QPalette>
#include <QRegExpValidator>
#include <QTimer>


#include "MainWindow.h"


LineEdit::LineEdit(int length, const QString &regexp)
    : QLineEdit()
{
    static const QSizePolicy fixedSizePolicy_(QSizePolicy::Fixed,
                                              QSizePolicy::Fixed);

    setAlignment(Qt::AlignRight);
    setFixedWidth(length * fontMetrics().width('0'));
    setSizePolicy(fixedSizePolicy_);

    if (!regexp.isEmpty())
    {
        setValidator(new QRegExpValidator(QRegExp(regexp)));
    }
}


IntLineEdit::IntLineEdit(int length, int nDgts)
    : LineEdit(length, QString("[0-9]{0,%1}").arg(nDgts))
{}

void IntLineEdit::setValue(int value)
{
     setText(QString::number(value));
}

int IntLineEdit::value()
{
     return text().toInt();
}

DoubleLineEdit::DoubleLineEdit(int length, int nIntDgts, int nFracDgts)
    : LineEdit(length, QString("[0-9]{0,%1}[.][0-9]{0,%2}").arg(nIntDgts).arg(nFracDgts))
    , nFracDgts_(nFracDgts)
{}

bool DoubleLineEdit::isValid()
{
    bool ok;
    locale().toDouble(text(), &ok);
    return ok;
}

double DoubleLineEdit::value()
{
    bool ok;
    double d = locale().toDouble(text(), &ok);

    Q_ASSERT(ok);
    return d;
}

void DoubleLineEdit::setValue(double value)
{
     setText(QString::number(value, 'f', nFracDgts_));
}

void putInGrid(QWidget* widget,
               QGridLayout *grid,
               int row,
               const QString &label,
               const QString &unit)
{
    grid->addWidget(new QLabel(label+":"), row, 0);

    grid->addWidget(widget, row, 1);

    if (!unit.isEmpty())
        grid->addWidget(new QLabel(unit), row, 2);
}

