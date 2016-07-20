#include <cmath>

#include <QMutex>
#include <QMutexLocker>
#include <QSettings>

#include "AotfCrystal.h"

inline double pow2(double x) { return x*x; }

static const double pi = 2*asin(1.0);

AotfCrystal::AotfCrystal(QObject *parent)
    : QObject(parent)
    , parametersLock_()
{
    restoreSettings();
}

void AotfCrystal::setParameters(Parameters params)
{
    QMutexLocker lock(&parametersLock_);
    Q_UNUSED(lock);

    if (params_ != params)
    {
        params_ = params;
        persisteSettings();
    }
}

AotfCrystal::Parameters AotfCrystal::getParameters() const
{
    QMutexLocker lock(&parametersLock_);
    Q_UNUSED(lock);

    return params_;
}

double AotfCrystal::wavelength(double freq, double T) const
{
    // Solve freq = frequency(lambda, T) for lambda using a Newton's method.

    const double tol = 1e-7; // Tol is 0.1 hz

    double l = 460; // Initial guess is 460 nm
    double nu = frequency(l, T);
    while (fabs(freq-nu) > tol)
    {
        l += (freq-nu)/(frequency(l+0.5, T)-frequency(l-0.5, T));
        nu = frequency(l, T);
    }

    return l;
}


const double rho = 5990.0;  // TeO2 mass density [kg m-3]
const double T0  = 21.0;    // Ref. temperature [°]
const double p44 = -0.044;  // Photo-elastic coefficient []

double
AotfCrystal::acousticParam(double lambda, double T, bool isFrequency) const
{
    const double alpha  = params_.alpha_deg * pi / 180.0; // alpha (rad)
    const double theta  = params_.theta_deg * pi / 180.0; // theta (rad)
    const double sina   = sin(alpha);                     // sin(alpha)
    const double cosa   = cos(alpha);                     // cos(alpha)
    const double sint   = sin(theta);                     // sin(thetha)
    const double cost   = cos(theta);                     // cos(thetha)
    const double sinat  = sin(alpha+theta);               // sin(alpha+thetha)
    const double cosat  = cos(alpha+theta);               // cos(alpha+thetha)
    const double sinaa  = sin(2*alpha);                   // sin(2*alpha)
    const double sin2a  = pow2(sina);                     // sin²(alpha)
    const double cos2a  = pow2(cosa);                     // cos²(alpha)
    const double cos2t  = pow2(cost);;                    // cos²(theta)
    const double sin2at = pow2(sinat);                    // sin²(alpha+theta)
    const double cos2at = pow2(cosat);                    // cos²(alpha+theta)

    const double &length = params_.transLength;
    const double &height = params_.transHeight;

    const double c11 = (5.620-0.00148*T)*1e10;
    const double c12 = (5.120-0.00178*T)*1e10;
    const double c44 = (2.675-0.00020*T)*1e10;
    // Temperature-dependent crystal stiffness coefficients [N m-2]

    const double V2 = (c11-c12)/2/rho*cos2a + c44/rho*sin2a;
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

    const double ni = n0T*neT/sqrt(pow2(neT)*cos2at + pow2(n0T)*sin2at);
    // Temperature-dependent incident light refractive index

    if (isFrequency)
        return sqrt(V2) * (ni*sint - sqrt(pow2(n0T) - pow2(ni)*cos2t)) / l;
        // Temperature-dependent tuning curve [MHz]

    const double psi = atan((c44-(c11-c12)/2)/(2*rho*V2)*sinaa);
    // Temperature-dependent walk-off angle [rad]

    const double p = 0.113*cosa*cosat + p44*sina*sinat;
    // Effective photoelastic coefficient

    const double M_2 = pow((ni*n0T/sqrt(V2)),3) * pow2(p) / rho;
    // Acousti-optic figure of merit

    return 5e-10*height*ni/(length*n0T*M_2)*pow2(l*cos(psi-theta)/cos(psi));
    // Acoustic power [mW]
}

void AotfCrystal::persisteSettings() const
{
    QSettings settings;
    settings.beginGroup("Crystal");
    settings.setValue("cut angle [deg]", params_.alpha_deg);
    settings.setValue("incident angle [deg]", params_.theta_deg);
    settings.setValue("transducer height [mm]", params_.transHeight);
    settings.setValue("transducer length [mm]", params_.transLength);
    settings.endGroup();

}

void AotfCrystal::restoreSettings()
{
    QSettings settings;

    Parameters params;

    settings.beginGroup("Crystal");
    params.alpha_deg = settings.value("cut angle [deg]", 7.65).toDouble();
    params.theta_deg = settings.value("incident angle [deg]", 10.1).toDouble();
    params.transHeight = settings.value("transducer height [mm]", 10.0).toDouble();
    params.transLength = settings.value("transducer length [mm]", 15.0).toDouble();
    settings.endGroup();

    setParameters(params);
}

bool operator==(const AotfCrystal::Parameters& lhs,
                const AotfCrystal::Parameters& rhs)
{
    return  lhs.alpha_deg == rhs.alpha_deg &&
            lhs.theta_deg == rhs.theta_deg &&
            lhs.transHeight == rhs.transHeight &&
            lhs.transLength == rhs.transLength;
}
