#ifndef AOTFCRYSTAL_H
#define AOTFCRYSTAL_H

class AotfCrystal
{
public:
    explicit AotfCrystal();

    void setParameters(double alpha_deg, double theta_deg,
                       double trans_height, double trans_length);

    double frequency(double lambda, double T) const;
    double power(double lambda, double T) const;
    double wavelength(double freq, double T) const;

private:
    void loadSetting();
    double acousticParam(double lambda, double T, bool isFrequency) const;

    double alpha_deg; // alpha [deg]
    double theta_deg; // theta [deg]
    double transH;    // Transducer height [mm]
    double transL;    // Transducer length [mm]

    double alpha;     // alpha (rad)
    double theta;     // theta (rad)
    double sin_a;     // sin(alpha)
    double cos_a;     // cos(alpha)
    double sin_t;     // sin(thetha)
    double cos_t;     // cos(thetha)
    double sin_at;    // sin(alpha+thetha)
    double cos_at;    // cos(alpha+thetha)
    double sin_2a;    // sin(2*alpha)
    double sin2_a;    // sin²(alpha)
    double cos2_a;    // cos²(alpha)
    double cos2_t;    // cos²(theta)
    double sin2_at;   // sin²(alpha+theta)
    double cos2_at;   // cos²(alpha+theta)
};

inline double AotfCrystal::frequency(double lambda, double T) const
{
    return acousticParam(lambda, T, true);
}

inline double AotfCrystal::power(double lambda, double T) const
{
    return acousticParam(lambda, T, false);
}

#endif // AOTFCRYSTAL_H
