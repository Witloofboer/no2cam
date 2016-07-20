#include "tooling.h"

#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>

const QSizePolicy fixedSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

IntLineEdit::IntLineEdit(const QString &inputMask)
{
    setAlignment(Qt::AlignRight);
    setInputMask(inputMask);
    setText(inputMask);
    setFixedWidth((2+inputMask.length()) * fontMetrics().width('0'));
    setSizePolicy(fixedSizePolicy);
}

void IntLineEdit::setValue(int value)
{
     setText(QString::number(value));
}

int IntLineEdit::value()
{
     return text().toInt();
}

DoubleLineEdit::DoubleLineEdit(const QString &inputMask)
{
    setAlignment(Qt::AlignRight);
    setInputMask(inputMask);
    setText(inputMask);
    setFixedWidth((2+inputMask.length()) * fontMetrics().width('0'));
    setSizePolicy(fixedSizePolicy);
}

void DoubleLineEdit::setValue(double value)
{
     setText(QString::number(value));
}

double DoubleLineEdit::value()
{
     return text().toDouble();
}

DoubleLineEdit *new_WavelengthEdit() { return new DoubleLineEdit("999.9"); }
DoubleLineEdit *new_EetEdit()        { return new DoubleLineEdit("009.9"); }
DoubleLineEdit *new_CooldownEdit()   { return new DoubleLineEdit("009.9"); }

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
