#ifndef NUMBEREDITWIDGET_H
#define NUMBEREDITWIDGET_H

class QGridLayout;
class QLineEdit;
class QString;
class QWidget;

QLineEdit *new_NumberEdit(const QString &inputMask);

QLineEdit *new_WavelengthEdit();
QLineEdit *new_EetEdit();
QLineEdit *new_CooldownEdit();

void putInGrid(QWidget* widget,
               QGridLayout* grid,
               int row,
               const QString &label,
               const QString &unit);


#endif // NUMBEREDITWIDGET_H
