#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "core_global.h"

namespace core {

//------------------------------------------------------------------------------

/**
 * The Crystal class accounts for the AOTF crystal setup behaviour.
 *
 * \note In the current version, only a TeO<sub>2</sub> cristal is considered.
 */

class CORESHARED_EXPORT Crystal
{
public:
    /**
     * Sets the parameters.
     *
     * The values of the physical parameters are retreived from the previous
     * class usage through the platform persistence mechanism.
     *
     * \param cutAngle crystal cut angle [deg]
     * \param incidentAngle light incident angle [deg]
     * \param transHeight transducer height [mm]
     * \param transLength transducer length [mm]
     */
     void set(double cutAngle,
              double lightAngle,
              double transHeight,
              double transLength);

     double cutAngle() const {return alpha_deg;}
     double lightAngle() const {return theta_deg;}
     double transHeight() const {return tH;}
     double transLength() const {return tL;}

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
    double acousticParam(double lambda, double T, bool isFrequency) const;

    double alpha_deg;   // crystal cut-angle [rad]
    double theta_deg;   // light incident angle [rad]
    double tH; // transducer height [mm]
    double tL; // transducer length [mm]
    double alpha; // alpha (rad)
    double theta; // theta (rad)
    double sina;  // sin(alpha)
    double cosa;  // cos(alpha)
    double sint;  // sin(thetha)
    double cost;  // cos(thetha)
    double sinat; // sin(alpha+thetha)
    double cosat; // cos(alpha+thetha)
    double sinaa; // sin(2*alpha)
    double sin2a; // sin²(alpha)
    double cos2a; // cos²(alpha)
    double cos2t; // cos²(theta)
    double sin2at; // sin²(alpha+theta)
    double cos2at; // cos²(alpha+theta)
};

//------------------------------------------------------------------------------

inline double Crystal::frequency(double lambda, double T) const
{
    return acousticParam(lambda, T, true);
}

//------------------------------------------------------------------------------

inline double Crystal::power(double lambda, double T) const
{
    return acousticParam(lambda, T, false);
}

//------------------------------------------------------------------------------

/**
 * Memberwise equality operator for AotfCrystalParameters instances.
 */
CORESHARED_EXPORT
bool operator==(const Crystal& lhs,
                const Crystal& rhs);

//------------------------------------------------------------------------------

/**
 * Memberwise inequality operator for AotfCrystalParameters instances.
 */
inline
bool operator!=(const Crystal& lhs,
                const Crystal& rhs)
{
    return !(lhs == rhs);
}

//------------------------------------------------------------------------------

}

#endif // CRYSTAL_H
