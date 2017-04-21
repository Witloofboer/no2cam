#ifndef TOOLS_H
#define TOOLS_H

//------------------------------------------------------------------------------
#include "core_global.h"

#include<QPair>
#include<QVector>

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

class CORESHARED_EXPORT BiInterpolator
{
public:
    BiInterpolator(const QVector<double> &xs,
                   const QVector<double> &ys,
                   const QVector<QVector<double>> &values);

    double operator()(double x, double y) const;

private:
    const QVector<double> _xs;
    const QVector<double> _ys;
    const QVector<QVector<double>> _vs;
};

//------------------------------------------------------------------------------

}

#endif // TOOLS_H
