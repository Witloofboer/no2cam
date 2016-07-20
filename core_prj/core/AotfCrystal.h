#ifndef AOTFCRYSTAL_H
#define AOTFCRYSTAL_H

#include <QMutex>
#include <QObject>
#include "core_global.h"

/**
 * The AotfCrystal class accounsts for the AOTF crystal setup behaviour.
 *
 * \note In the current version, only a TeO<sub>2</sub> cristal is considered.
 */

class CORESHARED_EXPORT AotfCrystal: public QObject
{
public:
    Q_OBJECT

public:
    struct Parameters;

    /**
     * Constructs a new AOTF crystal instance.
     *
     * The values of the physical parameters are retreived from the previous
     * class usage through the platform persistence mechanism.
     */
    explicit AotfCrystal(QObject *parent = 0);

    /**
     * Sets the crystal parameters.
     */
    void setParameters(Parameters params);

    /**
     * Return the cystal parameters.
     */
    Parameters getParameters() const;

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

    /**
     * This structure is a container for the physical parameters of the
     * crystal.
     */
    struct Parameters
    {
        double alpha_deg;   /**< crystal cut-angle [rad] */
        double theta_deg;   /**< light incident angle  [rad] */
        double transHeight; /**< transducer height [mm] */
        double transLength; /**< transducer length [mm] */

        friend bool operator==(const AotfCrystal::Parameters& lhs,
                               const AotfCrystal::Parameters& rhs);

        friend bool operator!=(const AotfCrystal::Parameters& lhs,
                               const AotfCrystal::Parameters& rhs);
    };


private:
    void persisteSettings() const;
    void restoreSettings();

    double acousticParam(double lambda, double T, bool isFrequency) const;

    Parameters params_;
    mutable QMutex parametersLock_;
};

/**
 * Memberwise equality operator for AotfCrystal::Parameters instances.
 */
CORESHARED_EXPORT
bool operator==(const AotfCrystal::Parameters& lhs,
                const AotfCrystal::Parameters& rhs);

/**
 * Memberwise inequality operator for AotfCrystal::Parameters instances.
 */
inline
bool operator!=(const AotfCrystal::Parameters& lhs,
                const AotfCrystal::Parameters& rhs)
{
    return !(lhs == rhs);
}


inline double AotfCrystal::frequency(double lambda, double T) const
{
    return acousticParam(lambda, T, true);
}

inline double AotfCrystal::power(double lambda, double T) const
{
    return acousticParam(lambda, T, false);
}

#endif // AOTFCRYSTAL_H
