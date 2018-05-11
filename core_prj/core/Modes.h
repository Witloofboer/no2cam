#ifndef BASEMODE_H
#define BASEMODE_H

#include "Snapshot.h"

#include <QDateTime>
#include <QVector>

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
    virtual void takeSnapshot(int exposure)=0;
    virtual void setSnapshotForGui(const Snapshot &snapshotBuffer)=0;

    virtual void saveSnapshot(const QDateTime &dateTime,
                              char mode,
                              int exposure,
                              double wavelength,
                              double frequency,
                              double power,
                              int snapPerObs,
                              double temperature,
                              const Snapshot &snapshotBuffer)=0;

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
             int baseExposure,
             double refWavelength,
             double exposureFactor);

    virtual ~BaseMode();

    virtual void setAcousticBeam()=0;
    virtual void acousticBeamReady()=0;
    virtual void processSnapshot(const Snapshot &snapshotBuffer)=0;
    virtual bool mustContinueAquisition() const;
    virtual bool canCooldown() const;

    void start();

    IModeToManager &_manager;
    const Crystal &_crystal;
    int _baseExposure;
    double _refWavelength;
    double _exposureFactor;
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
                    double wavelength,
                    int exposure,
                    double refWavelength,
                    double exposureFactor);

    void setAcousticBeam() override;
    void acousticBeamReady() override;
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
                     int exposure);

    void setAcousticBeam() override;
    void acousticBeamReady() override;
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
                    double refWavelength,
                    double exposureFactor);

    void setAcousticBeam() override;
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
// DOAS Mode
//------------------------------------------------------------------------------

class DoasMode: public BaseMode
{
public:
    DoasMode(IModeToManager &manager,
             const Crystal &crystal,
             const QVector<double> &wavelengths,
             int nbrSeqPerObs,
             int exposure,
             double refWavelength,
             double exposureFactor);
    virtual ~DoasMode();

    void setAcousticBeam() override;
    void acousticBeamReady() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

protected:
    bool mustContinueAquisition() const override;
    bool canCooldown() const override;

private:
    QVector<double> _wavelengths, _frequencies, _powers, _exposures;
    Snapshot  *_snapshotBuffers;
    const int _nbrSeqPerObs;

    double _refTemperature;
    int _wlIx;   // Wavelength index
    int _seqIx;  // Sequence index
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
              double refWavelength,
              double exposureFactor);

    void setAcousticBeam() override;
    void acousticBeamReady() override;
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
