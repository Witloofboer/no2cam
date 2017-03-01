#ifndef BASEMODE_H
#define BASEMODE_H

#include "Snapshot.h"

#include <QDateTime>

//------------------------------------------------------------------------------

class QString;

namespace core {

class Crystal;

//------------------------------------------------------------------------------
// IModeToManager
//------------------------------------------------------------------------------

class IModeToManager
{
public:
    virtual double temperature() const=0;
    virtual void setAcousticBeam(double frequency, double power)=0;
    virtual void takeSnapshot()=0;
    virtual void setSnapshotForGui(const Snapshot &snapshotBuffer)=0;

    virtual void saveSnapshot(const QDateTime &dateTime,
                              char mode,
                              double wavelength,
                              double frequency,
                              double power,
                              int snapPerObs,
                              double temperature,
                              const Snapshot &snapshotBuffer)=0;

    /**
      * Requests the stop of all the devices.
    */
    virtual void onStop()=0;
};

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

class BaseMode
{
public:
    BaseMode(IModeToManager &manager,
             const Crystal &crystal);

    virtual ~BaseMode();

    virtual void setAcousticWave()=0;
    virtual void acousticBeamReady();
    virtual void processSnapshot(const Snapshot &snapshotBuffer)=0;
    virtual bool mustContinueAquisition() const;
    virtual bool canCooldown() const;

    void start();

    IModeToManager &_manager;
    const Crystal &_crystal;
    double _refTemperature;
    QDateTime _snapTime;
};

//------------------------------------------------------------------------------
// OpticalSnapMode
//------------------------------------------------------------------------------

class OpticalSnapMode: public BaseMode
{
public:
    OpticalSnapMode(IModeToManager &manager,
                    const Crystal &crystal,
                    double wavelength);

    void setAcousticWave() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

private:
    const double _wavelength;
    double _frequency;
    double _power;
};

//------------------------------------------------------------------------------
// AcousticSnapMode
//------------------------------------------------------------------------------

class AcousticSnapMode: public BaseMode
{
public:
    AcousticSnapMode(IModeToManager &manager,
                     const Crystal &crystal,
                     double frequency,
                     double power);

    void setAcousticWave() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

private:
    const double _frequency;
    const double _power;
    double _wavelength;
};

//------------------------------------------------------------------------------
// ObservationMode
//------------------------------------------------------------------------------

class ObservationMode: public BaseMode
{
public:
    ObservationMode(IModeToManager &manager,
                    const Crystal &crystal,
                    double wavelength1,
                    double wavelength2,
                    int snapshotPerObs);

    void setAcousticWave() override;
    void acousticBeamReady() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

protected:
    bool mustContinueAquisition() const override;
    bool canCooldown() const override;

private:
    const double _wavelengths[3];
    const int    _snapshotPerObs;

    int _idx;
    int _snapshotCount;

    double _frequency[3];
    double _power[3];
    Snapshot _snapshotBuffers[3];
};

//------------------------------------------------------------------------------
// SweepMode
//------------------------------------------------------------------------------

class SweepMode: public BaseMode
{
public:
    SweepMode(IModeToManager &manager,
              const Crystal &crystal,
              double minWavelength,
              double maxWavelength,
              double wavelengthStep,
              int blackSnapshotRate);

    void setAcousticWave() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

protected:
    bool mustContinueAquisition() const override;

private:
    const double _minWavelength;
    const double _maxWavelength;
    const double _wavelengthStep;
    const int _blackSnapshotRate;

    double _wavelength;
    double _frequency;
    double _power;
    int _counter;
};

//------------------------------------------------------------------------------

}

#endif // BASEMODE_H
