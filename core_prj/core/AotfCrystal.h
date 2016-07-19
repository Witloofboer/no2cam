#ifndef AOTFCRYSTAL_H
#define AOTFCRYSTAL_H

#include "core_global.h"

/**
 * The AotfCrystal class accounsts for the AOTF crystal setup behaviour.
 *
 * \note In the current version, only a TeO<sub>2</sub> cristal is considered.
 */

class CORESHARED_EXPORT AotfCrystal
{
public:
    /**
     * Constructs a new AOTF crystal instance.
     *
     * The values of the physical parameters are retreived from the previous
     * class usage through the execution platform persistence mechanism.
     */
    explicit AotfCrystal();

    /**
     * Sets the cystal parameters.
     *
     * @param alpha_deg    crystal cut-angle [rad]
     * @param theta_deg    light incident angle  [rad]
     * @param trans_height transducer height [mm]
     * @param trans_length transducer length [mm]
     */
    void setParameters(double alpha_deg, double theta_deg,
                       double trans_height, double trans_length);

    /**
     * Returns the acoustic frequency matching an optical wavelength at a given
     * crystal temperature.
     *
     * @param lambda optical wavelength [nm]
     * @param T      crystal temperature [°C]
     * @return       the matching acoustic frequency [MHz]
     */
    double frequency(double lambda, double T) const;

    /**
     * Returns the optimal acoustic power for a given optical wavelength at a
     * given crystal temperature
     *
     * @param lambda optical wavelength [nm]
     * @param T      crystal temperature [°C]
     * @return       the optimal acoustic power [MHz]
     */
    double power(double lambda, double T) const;

    /**
     * Returns the optical wavelength matching an acoustic frequency at a given
     * crystal temperature.
     *
     * @param freq acoustic frequency [MHz]
     * @param T    crystal temperature [°C]
     * @return     the matching optical wavelength [nm]
     */
    double wavelength(double freq, double T) const;

private:
    void loadSetting();
    double acousticParam(double lambda, double T, bool isFrequency) const;

    double alpha_deg_; // alpha [deg]
    double theta_deg_; // theta [deg]
    double transH_;    // Transducer height [mm]
    double transL_;    // Transducer length [mm]
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
