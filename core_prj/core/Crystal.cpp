#include <cmath>

#include <QSettings>

#include "Crystal.h"

inline double pow2(double x) {
    return x*x;
}

static const double pi = 2*asin(1.0);
static int _1 = qRegisterMetaType<core::Crystal>("core::Crystal");

namespace core {

//------------------------------------------------------------------------------

void Crystal::set(double cutAngle,
                  double incidentAngle,
                  double transHeight,
                  double transLength)
{
    _alphaDeg = cutAngle;
    _thetaDeg = incidentAngle;
    _h = transHeight;
    _l = transLength;
    _alpha = cutAngle * pi / 180.0;
    _theta = _thetaDeg * pi / 180.0;
    _sina = sin(_alpha);
    _cosa = cos(_alpha);
    _sint = sin(_theta);
    _cost = cos(_theta);
    _sinat = sin(_alpha+_theta);
    _cosat = cos(_alpha+_theta);
    _sinaa = sin(2*_alpha);
    _sin2a = pow2(_sina);
    _cos2a = pow2(_cosa);
    _cos2t = pow2(_cost);
    _sin2at = pow2(_sinat);
    _cos2at = pow2(_cosat);
}

//------------------------------------------------------------------------------

const double rho = 5990.0;  // TeO2 mass density [kg m-3]
const double T0  = 21.0;    // Ref. temperature [°]
const double p44 = -0.044;  // Photo-elastic coefficient []

void Crystal::computeFreqPow(double wavelength, double T,
                             double &freq, double &power) const
{
    const double c11 = (5.620-0.00148*T)*1e10;
    const double c12 = (5.120-0.00178*T)*1e10;
    const double c44 = (2.675-0.00020*T)*1e10;
    const double ceff = (-3.41e3*T+1.0496e6)*T+2.2522e9;
    // Temperature-dependent crystal stiffness coefficients [N m-2]

    const double V2 = ceff/rho*_cos2a + c44/rho*_sin2a;
    // Square of the temperature-dependent acoustic wave phase velocity [m2 s-2]

    const double l  = wavelength * 1e-3;
    const double l2 = pow2(l);
    const double n0T0 = sqrt(1 + 3.71789*l2/(l2-pow2(0.19619))
                             + 0.07544*l2/(l2-pow2(4.61196)));
    const double neT0 = sqrt(1 + 4.33449*l2/(l2-pow2(0.20242))
                             + 0.14739*l2/(l2-pow2(4.93667)));
    const double n0T  = n0T0*(1 + 20e-6*(T-T0));
    const double neT  = neT0*(1 + 18e-6*(T-T0));
    // Temperature-dependent ordinary and extraordinary refractive indices

    const double ni = n0T*neT/sqrt(pow2(neT)*_cos2at + pow2(n0T)*_sin2at);
    // Temperature-dependent incident light refractive index

    freq = sqrt(V2) * (ni*_sint - sqrt(pow2(n0T) - pow2(ni)*_cos2t)) / l;
    // Temperature-dependent tuning curve [MHz]

    const double psi = atan((c44-(c11-c12)/2)/(2*rho*V2)*_sinaa);
    // Temperature-dependent walk-off angle [rad]

    const double p = 0.113*_cosa*_cosat + p44*_sina*_sinat;
    // Effective photoelastic coefficient

    const double M_2 = pow((ni*n0T/sqrt(V2)),3) * pow2(p) / rho;
    // Acousti-optic figure of merit

    power = 5e-10*_h*ni/(_l*n0T*M_2)*pow2(l*cos(psi-_theta)/cos(psi));
    // Acoustic power [mW]
}

//------------------------------------------------------------------------------

double Crystal::frequency(double wavelength, double T) const
{
    double frequency, power;

    computeFreqPow(wavelength,T, frequency, power);

    return frequency;
}

//------------------------------------------------------------------------------

double Crystal::wavelength(double freq, double T) const
{
    // Solve freq = frequency(lambda, T) for lambda using a Newton's method.

    const double tol = 1e-7; // Tol is 0.1 hz

    double wavelength = 460; // Initial guess is 460 nm
    double freq_, power;

    computeFreqPow(wavelength, T, freq_, power);
    while (fabs(freq-freq_) > tol)
    {

        wavelength += (freq-freq_)/(frequency(wavelength+0.5, T)-frequency(wavelength-0.5, T));
        computeFreqPow(wavelength, T, freq_, power);
    }

    return wavelength;
}

//------------------------------------------------------------------------------

}
