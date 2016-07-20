#ifndef TOOLING_H
#define TOOLING_H

#include <QLineEdit>

class QGridLayout;
class QString;
class QWidget;

class IntLineEdit : public QLineEdit
{
public:
    IntLineEdit(const QString &inputMask);
    void setValue(int setValue);
    int value();
};

class DoubleLineEdit : public QLineEdit
{
public:
    DoubleLineEdit(const QString &inputMask);
    void setValue(double setValue);
    double value();
};


DoubleLineEdit *new_WavelengthEdit();
DoubleLineEdit *new_EetEdit();
DoubleLineEdit *new_CooldownEdit();

void putInGrid(QWidget* widget,
               QGridLayout* grid,
               int row,
               const QString &label,
               const QString &unit);


#endif // TOOLING_H
