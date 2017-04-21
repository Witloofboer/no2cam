#include<Interpolation.h>
#include<qglobal.h>

#include<exception>

namespace core {

//------------------------------------------------------------------------------

BiInterpolation::BiInterpolation(const QVector<double> &xs,
                                 const QVector<double> &ys,
                                 const QVector<QVector<double>> &values)
    : _xs(xs)
    , _ys(ys)
    , _vs(values)
{

    if (xs.length() < 2)
        throw std::invalid_argument(
            QString("BiInterpolation::BiInterpolation: "
                    "xs invalid dimension "
                    "(dim(xs) = %1 < 2)")
            .arg(xs.length())
            .toStdString()
        );

    for(int i=0; i<xs.length()-1; ++i)
        if (xs[i]>=xs[i+1])
            throw std::invalid_argument(
                QString("BiInterpolation::BiInterpolation: "
                        "xs' values not strictly increasing "
                        "(xs[%1] = %2, xs[%3] = %4)")
                .arg(i)
                .arg(xs[i])
                .arg(i+1)
                .arg(xs[i+1])
                .toStdString()
            );

    if (ys.length() < 2)
        throw std::invalid_argument(
            QString("BiInterpolation::BiInterpolation: "
                    "ys invalid dimension "
                    "(dim(ys) = %1 < 2)")
            .arg(ys.length())
            .toStdString()
        );


    for(int i=0; i<ys.length()-1; ++i)
        if (ys[i]>=ys[i+1])
            throw std::invalid_argument(
                QString("BiInterpolation::BiInterpolation: "
                        "ys' values not strictly increasing "
                        "(ys[%1] = %2, ys[%3] = %4)")
                .arg(i)
                .arg(ys[i])
                .arg(i+1)
                .arg(ys[i+1])
                .toStdString()
            );

    if (xs.length() != values.length())
        throw std::invalid_argument(
            QString("BiInterpolation::BiInterpolation: "
                    "dim(xs) = %1, dim(values) = %2")
            .arg(xs.length())
            .arg(values.length())
            .toStdString()
        );

    for (int i=0; i<values.length(); ++i)
    {
        if (ys.length() != values[i].length())
            throw std::invalid_argument(
                QString("BiInterpolation::BiInterpolation: "
                        "dim(ys) = %1, dim(values[%2]) = %3")
                .arg(ys.length())
                .arg(i)
                .arg(values.length())
                .toStdString()
            );
    }
}

//------------------------------------------------------------------------------

double BiInterpolation::operator()(double x, double y)
{
    if (x < _xs.first() || _xs.last() < x)
        throw std::domain_error(
            QString("BiInterpolation::operator(): "
                    "parameter x (%1) out of range ([%2 %3])")
            .arg(x)
            .arg(_xs.first())
            .arg(_xs.last())
            .toStdString());

    if (y < _ys.first() || _ys.last() < y)
        throw std::domain_error(
            QString("BiInterpolation: parameter y (%1) out of range ([%2 %3])")
            .arg(y)
            .arg(_ys.first())
            .arg(_ys.last())
            .toStdString());

    int i=0;
    while (_xs[i+1] < x) ++i;

    int j=0;
    while (_ys[j+1] < y) ++j;

    const double x0 = (_xs[i+1]-x)/(_xs[i+1]-_xs[i]);
    const double x1 = 1-x0;
    const double y0 = (_ys[j+1]-y)/(_ys[j+1]-_ys[j]);
    const double y1 = 1-y0;

    return x0*(y0*_vs[i][j]+y1*_vs[i][j+1]) +
           x1*(y0*_vs[i+1][j]+y1*_vs[i+1][j+1]);
}

//------------------------------------------------------------------------------

}
