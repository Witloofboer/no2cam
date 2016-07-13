#include "tooling.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

const QSizePolicy fixedSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

QLineEdit *new_NumberEdit(const QString &inputMask)
{
    auto result = new QLineEdit();

    result->setAlignment(Qt::AlignRight);
    result->setInputMask(inputMask);
    result->setText(inputMask);
    result->setFixedWidth((2+inputMask.length()) * result->fontMetrics().width('0'));
    result->setSizePolicy(fixedSizePolicy);

    return result;
}

QLineEdit *new_WavelengthEdit() { return new_NumberEdit("999.9"); }
QLineEdit *new_EetEdit()        { return new_NumberEdit("009.9"); }
QLineEdit *new_CooldownEdit()   { return new_NumberEdit("009.9"); }

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
