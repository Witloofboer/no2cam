#ifndef CRYSTAL_H
#define CRYSTAL_H

#include <QMetaType>

#include "core_global.h"

//------------------------------------------------------------------------------

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
     * \param transHeight transduGcer height [mm]
     * \param transLength transducer length [mm]
     */
    void set(double cutAngle,
             double incidentAngle,
             double transHeight,
             double transLength);

    double cutAngle() const {
        return _alphaDeg;
    }
    double incidentAngle() const {
        return _thetaDeg;
    }
    double transHeight() const {
        return _h;
    }
    double transLength() const {
        return _l;
    }


    /**
     * Returns the acoustic frequency and the power matching an optical
     * wavelength at a given crystal temperature.
     *
     * @param wavelength optical wavelength [nm]
     * @param T          crystal temperature [°C]
     * @param freq       corresponding acoustic frequency [MHz]
     * @param power      corresponding acoustic power [mW]
     */
    void computeFreqPow(double wavelength, double T,
                        double &freq, double &power) const;

    /**
     * Returns the acoustic frequency matching an optical wavelength at a given
     * crystal temperature.
     *
     * @param wavelength optical wavelength [nm]
     * @param T          crystal temperature [°C]
     * @return the acoustic frequency [MHz]
     */
    double frequency(double wavelength, double T) const;

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

    double _alphaDeg;   // crystal cut-angle [rad]
    double _thetaDeg;   // light incident angle [rad]
    double _h; // transducer height [mm]
    double _l; // transducer length [mm]
    double _alpha; // alpha (rad)
    double _theta; // theta (rad)
    double _sina;  // sin(alpha)
    double _cosa;  // cos(alpha)
    double _sint;  // sin(thetha)
    double _cost;  // cos(thetha)
    double _sinat; // sin(alpha+thetha)
    double _cosat; // cos(alpha+thetha)
    double _sinaa; // sin(2*alpha)
    double _sin2a; // sin²(alpha)
    double _cos2a; // cos²(alpha)
    double _cos2t; // cos²(theta)
    double _sin2at; // sin²(alpha+theta)
    double _cos2at; // cos²(alpha+theta)
};

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
