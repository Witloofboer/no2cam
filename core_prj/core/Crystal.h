#ifndef CRYSTAL_H
#define CRYSTAL_H

#include <QObject>
#include "core_global.h"

/**
 * This structure is a container for the physical parameters of the
 * crystal.
 */
struct CORESHARED_EXPORT CrystalParameters
{
    double alpha_deg;   /**< crystal cut-angle [rad] */
    double theta_deg;   /**< light incident angle  [rad] */
    double transHeight; /**< transducer height [mm] */
    double transLength; /**< transducer length [mm] */

    void persiste() const;
    void restore();

    friend bool operator==(const CrystalParameters& lhs,
                           const CrystalParameters& rhs);

    friend bool operator!=(const CrystalParameters& lhs,
                           const CrystalParameters& rhs);
};

Q_DECLARE_METATYPE(CrystalParameters)

/**
 * The AotfCrystal class accounsts for the AOTF crystal setup behaviour.
 *
 * \note In the current version, only a TeO<sub>2</sub> cristal is considered.
 */

class CORESHARED_EXPORT Crystal: public QObject
{
public:
    Q_OBJECT

public:
    /**
     * Constructs a new AOTF crystal instance.
     *
     * The values of the physical parameters are retreived from the previous
     * class usage through the platform persistence mechanism.
     */
    explicit Crystal(QObject *parent = 0);

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
     * Sets the crystal parameters.
     */
    void setParameters(const CrystalParameters& params);

private:
    double acousticParam(double lambda, double T, bool isFrequency) const;

    CrystalParameters crystalParams;
};

inline double Crystal::frequency(double lambda, double T) const
{
    return acousticParam(lambda, T, true);
}

inline double Crystal::power(double lambda, double T) const
{
    return acousticParam(lambda, T, false);
}



/**
 * Memberwise equality operator for AotfCrystalParameters instances.
 */
CORESHARED_EXPORT
bool operator==(const CrystalParameters& lhs,
                const CrystalParameters& rhs);

/**
 * Memberwise inequality operator for AotfCrystalParameters instances.
 */
inline
bool operator!=(const CrystalParameters& lhs,
                const CrystalParameters& rhs)
{
    return !(lhs == rhs);
}

#endif // CRYSTAL_H
