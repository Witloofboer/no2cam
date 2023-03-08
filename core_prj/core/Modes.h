#ifndef BASEMODE_H
#define BASEMODE_H

#include "Snapshot.h"

#include <QDateTime>
#include <QVector>

//------------------------------------------------------------------------------

class QString;

namespace core {

class Crystal;
class Manager;

typedef qint32 SgnPixel;
typedef SgnPixel AccumulatingBuffer[snapshotSize][snapshotSize];

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

class BaseMode
{
public:
    BaseMode(Manager &manager,
             const Crystal &crystal,
             int exposure);

    virtual ~BaseMode();

    virtual void setAcousticBeam()=0;
    virtual void acousticBeamReady()=0;
    virtual void processSnapshot(const Snapshot &snapshotBuffer)=0;
    virtual bool mustContinueAquisition() const;
    virtual bool canCooldown() const;

    void start();

    Manager &_manager;
    const Crystal &_crystal;
    int _exposure;
    double _refTemp;
    QDateTime _snapTime;
};

//------------------------------------------------------------------------------
// OpticalSnapMode
//------------------------------------------------------------------------------

class OpticalSnapMode: public BaseMode
{
public:
    OpticalSnapMode(Manager &manager,
                    const Crystal &crystal,
                    double wavelength,
                    int exposure);

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
    AcousticSnapMode(Manager &manager,
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
// Generic mode
//------------------------------------------------------------------------------

class GenericMode: public BaseMode
{
public:
    GenericMode(Manager &manager,
             const Crystal &crystal,
             const QVector<double> &wavelengths,
             const QString &mode,
             int nbrSeqPerObs,
             int exposure);
    virtual ~GenericMode();

    void setAcousticBeam() override;
    void acousticBeamReady() override;
    void processSnapshot(const Snapshot &snapshotBuffer) override;

protected:
    bool mustContinueAquisition() const override;
    bool canCooldown() const override;

private:
    QVector<double> _wavelengths, _frequencies, _powers;
    AccumulatingBuffer* _accumulators;
    const QString _mode;
    const int _nbrSeqPerObs;
    double _refTemp;
    int _wlIx;   // Wavelength index
    int _seqIx;  // Sequence index
};

//------------------------------------------------------------------------------

}

#endif // BASEMODE_H
