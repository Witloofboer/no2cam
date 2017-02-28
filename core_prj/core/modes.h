#ifndef BASEMODE_H
#define BASEMODE_H

#include "Snapshot.h"

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
    virtual double temperature() const =0;
    virtual void setAcousticBeam(double frequency, double power) =0;
    virtual void takeSnapshot() =0;
    virtual void snapshotReadyforGui() =0;
    virtual void saveSnapshot(const QDateTime &dateTime,
                              char mode,
                              double wavelength,
                              double frequency,
                              double power,
                              int snapPerObs,
                              int exposure,
                              double temperature,
                              const Snapshot &snapshotBuffer)=0;
    virtual void coolDown()=0;

    /**
      * Requests the stop of all the devices.
    */
    virtual void stop()=0;
};

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

class BaseMode
{
public:
    BaseMode(IModeToManager &manager,
             const Crystal &crystal,
             int exposure,
             int cooldownTime,
             int cooldownPwr,
             int stabilisationTime,
             bool burst,
             bool record,
             const QString &dataFolder,
             const QString &session);

    virtual ~BaseMode();

    virtual void setAcousticWave()=0;
    virtual void acousticBeamReady();
    virtual void processSnapshot(const Snapshot &snapshotBuffer)=0;
    void proceed();

protected:
    virtual bool mustContinueAquisition() const;
    virtual bool mustCooldown() const;


    IModeToManager &_manager;
    const Crystal &_crystal;
    int _exposure;
    int _cooldownTime;
    int _cooldownPwr;
    int _stabilisationTime;
    bool _burst;
    bool _record;
    const QString &_dataFolder;
    const QString &_session;

    double _temperature;
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
                    double wavelength,
                    int exposure,
                    int cooldownTime,
                    int cooldownPwr,
                    int stabilisationTime,
                    bool burst,
                    bool record,
                    const QString &dataFolder,
                    const QString &session);

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
                     double power,
                     int exposure,
                     int cooldownTime,
                     int cooldownPwr,
                     int stabilisationTime,
                     bool burst,
                     bool record,
                     const QString &dataFolder,
                     const QString &session);

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
                    int snapshotPerObs,
                    int exposure,
                    int cooldownTime,
                    int cooldownPwr,
                    int stabilisationTime,
                    bool burst,
                    bool record,
                    const QString &dataFolder,
                    const QString &session);

    void setAcousticWave() override;
    void acousticBeamReady() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

protected:
    bool mustContinueAquisition() const override;
    bool mustCooldown() const override;

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
              int blackSnapshotRate,
              int exposure,
              int cooldownTime,
              int cooldownPwr,
              int stabilisationTime,
              bool burst,
              bool record,
              const QString &dataFolder,
              const QString &session);

    void setAcousticWave() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

protected:
    bool mustContinueAquisition() const override;

private:
    const double _minWavelength;
    const double _maxWavelength;
    const double _wavelengthStep;
    const int _blackSnapshotRate;

    int _counter;
    double _wavelength;
    double _frequency;
    double _power;

};

//------------------------------------------------------------------------------

}

#endif // BASEMODE_H
