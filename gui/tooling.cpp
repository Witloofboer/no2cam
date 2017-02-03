#include "tooling.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QRegExpValidator>

namespace gui {

//==============================================================================
// LineEdit
//==============================================================================

LineEdit::LineEdit(int length, const QString &regexp)
    : QLineEdit()
{
    static const QSizePolicy fixedSizePolicy_(QSizePolicy::Fixed,
            QSizePolicy::Fixed);

    setAlignment(Qt::AlignLeft);
    setFixedWidth(length * fontMetrics().width('0'));
    setSizePolicy(fixedSizePolicy_);

    setValidator(new QRegExpValidator(QRegExp(regexp)));
}

//------------------------------------------------------------------------------

bool LineEdit::isValid()
{
    int pos = 0;
    QString value = text();
    return QValidator::Acceptable == validator()->validate(value, pos);
}

//------------------------------------------------------------------------------

void LineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit focusLost();
}


//==============================================================================
// IntLineEdit
//==============================================================================

IntLineEdit::IntLineEdit(int length, int nDgts)
    : LineEdit(length, QString("^\\d{1,%1}$").arg(nDgts))
{}

//------------------------------------------------------------------------------

bool IntLineEdit::isValid()
{
    bool ok;
    text().toInt(&ok);
    return ok;
}

//------------------------------------------------------------------------------

int IntLineEdit::value()
{
    bool ok;
    int d = text().toInt(&ok);

    Q_ASSERT(ok);
    return d;
}

//------------------------------------------------------------------------------

void IntLineEdit::setValue(int value)
{
    setText(QString("%1").arg(value));
}


//==============================================================================
// PosIntLineEdit
//==============================================================================

PosIntLineEdit::PosIntLineEdit(int length, int nDgts)
    : IntLineEdit(length, nDgts)
{}

//------------------------------------------------------------------------------

bool PosIntLineEdit::isValid()
{
    return IntLineEdit::isValid() && value()>0;
}

//==============================================================================
// DoubleLineEdit
//==============================================================================

DoubleLineEdit::DoubleLineEdit(int length, int nIntDgts, int nFracDgts)
    : LineEdit(length,
               QString("^\\d{1,%1}[.]\\d{0,%2}$").arg(nIntDgts).arg(nFracDgts))
    , _nFracDgts(nFracDgts)
{}

//------------------------------------------------------------------------------

bool DoubleLineEdit::isValid()
{
    bool ok;
    text().toDouble(&ok);
    return ok;
}

//------------------------------------------------------------------------------

double DoubleLineEdit::value()
{
    bool ok;
    double d = text().toDouble(&ok);

    Q_ASSERT(ok);
    return d;
}

//------------------------------------------------------------------------------

void DoubleLineEdit::setValue(double value)
{
    setText(QString("%1").arg(value, 1, 'f', _nFracDgts));
}

//------------------------------------------------------------------------------

void putInGrid(QWidget* widget,
               QGridLayout *grid,
               int &row,
               const QString &label,
               const QString &unit)
{
    grid->addWidget(new QLabel(label+":"), row, 0);
    grid->addWidget(widget, row, 1);

    if (!unit.isEmpty())
        grid->addWidget(new QLabel(unit), row, 2);

    ++row;
}

//------------------------------------------------------------------------------

}
