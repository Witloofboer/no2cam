#include<Interpolate.h>

namespace core {

//------------------------------------------------------------------------------

BiInterpolation::BiInterpolation(const QVector<double> &xs,
                                 const QVector<double> &ys,
                                 const QVector<QVector<double>> &values)
    : _xs(xs)
    , _ys(ys)
    , _vs(values)
{
    Q_ASSERT(2 <= xs.length());
    Q_ASSERT(2 <= ys.length());
    Q_ASSERT(xs.length() == values.length());

    for (auto it=_vs.constBegin(); it != _vs.constEnd(); ++it)
    {
        Q_ASSERT(ys.length() == it->length());
    }
}

//------------------------------------------------------------------------------

double BiInterpolation::operator()(double x, double y)
{
    if (x < _xs.first() || _xs.last() < x ||
            y < _ys.first() || _ys.last() < y)
        throw std::domain_error(nullptr);


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
