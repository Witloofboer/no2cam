#include "AotfCrystal.h"

#include <cmath>

#include "global.h"

inline double pow2(double x) { return x*x; }

static const double pi = 2*asin(1.0);

AotfCrystal::AotfCrystal()
{
    loadSetting();
}

void AotfCrystal::parameters(double alpha_deg, double theta_deg,
                             double trans_height, double trans_length)
{
    this->alpha_deg = alpha_deg;
    this->theta_deg = theta_deg;
    this->transH = trans_height;
    this->transL = trans_length;

    alpha    = alpha_deg * pi / 180.0;
    theta        = theta_deg * pi / 180.0;
    sin_a    = sin(alpha);
    cos_a    = cos(alpha);
    sin_t    = sin(theta);
    cos_t    = cos(theta);
    sin_at   = sin(alpha+theta);
    cos_at   = cos(alpha+theta);
    sin_2a   = sin(2*alpha);
    cos2_t   = pow2(cos_t);
    sin2_a   = pow2(sin_a);
    cos2_a   = pow2(cos_a);
    sin2_at  = pow2(sin_at);
    cos2_at  = pow2(cos_at);
}



const double rho = 5990.0;  // TeO2 mass density [kg m-3]
const double T0  = 21.0;    // Ref. temperature [°]
const double p44 = -0.044;  // Photo-elastic coefficient []

double
AotfCrystal::acousticParam(double lambda, double T, bool isFrequency) const
{
    const double c11 = (5.620-0.00148*T)*1e10;
    const double c12 = (5.120-0.00178*T)*1e10;
    const double c44 = (2.675-0.00020*T)*1e10;
    // Temperature-dependent crystal stiffness coefficients [N m-2]

    const double V2 = (c11-c12)/2/rho*cos2_a + c44/rho*sin2_a;
    // Square of the temperature-dependent acoustic wave phase velocity [m2 s-2]

    const double l  = lambda * 1e-3;
    const double l2 = pow2(l);
    const double n0T0 = sqrt(1 + 3.71789*l2/(l2-pow2(0.19619))
                               + 0.07544*l2/(l2-pow2(4.61196)));
    const double neT0 = sqrt(1 + 4.33449*l2/(l2-pow2(0.20242))
                               + 0.14739*l2/(l2-pow2(4.93667)));
    const double n0T  = n0T0*(1 + 20e-6*(T-T0));
    const double neT  = neT0*(1 + 18e-6*(T-T0));
    // Temperature-dependent ordinary and extraordinary refractive indices

    const double ni = n0T*neT/sqrt(pow2(neT)*cos2_at + pow2(n0T)*sin2_at);
    // Temperature-dependent incident light refractive index

    if (isFrequency)
        return sqrt(V2) * (ni*sin_t - sqrt(pow2(n0T) - pow2(ni)*cos2_t)) / l;
        // Temperature-dependent tuning curve [MHz]

    const double psi = atan((c44-(c11-c12)/2)/(2*rho*V2)*sin_2a);
    // Temperature-dependent walk-off angle [rad]

    const double p = 0.113*cos_a*cos_at + p44*sin_a*sin_at;
    // Effective photoelastic coefficient

    const double M_2 = pow((ni*n0T/sqrt(V2)),3) * pow2(p) / rho;
    // Acousti-optic figure of merit

    return 5e-10*transH*ni/(transL*n0T*M_2)*pow2(l*cos(psi-theta)/cos(psi));
    // Acoustic power [mW]
}

void AotfCrystal::loadSetting()
{
    gSettings.beginGroup("Crystal parameters");
    parameters(gSettings.value("cut angle [deg]", 7.65).toDouble(),
               gSettings.value("incident angle [deg]", 10.1).toDouble(),
               gSettings.value("transducer height [mm]", 10.0).toDouble(),
               gSettings.value("transducer length [mm]", 15.0).toDouble());
    gSettings.endGroup();
}
