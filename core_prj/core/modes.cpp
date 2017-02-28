#include "modes.h"

#include "Crystal.h"

namespace core {

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

BaseMode::BaseMode(IModeToManager &manager,
                   const Crystal &crystal,
                   int exposure,
                   int cooldownTime,
                   int cooldownPwr,
                   int stabilisationTime,
                   bool burst,
                   bool record,
                   const QString &dataFolder,
                   const QString &session)
    : _manager(manager)
    , _crystal(crystal)
    , _exposure(exposure)
    , _cooldownTime(cooldownTime)
    , _cooldownPwr(cooldownPwr)
    , _stabilisationTime(stabilisationTime)
    , _burst(burst)
    , _record(record)
    , _dataFolder(dataFolder)
    , _session(session)
{

}

//------------------------------------------------------------------------------

BaseMode::~BaseMode() {}

//------------------------------------------------------------------------------

void BaseMode::acousticBeamReady()
{
    _snapTime = QDateTime::currentDateTime();
    _manager->takeSnapshot();
}

//------------------------------------------------------------------------------

void BaseMode::proceed()
{
    if (mustContinueAquisition())
    {
        if (mustCooldown())
        {
            _manager.cooldown();
        } else {
            setAcousticWave();
        }
    } else {
        _manager.stop();
    }
}

//------------------------------------------------------------------------------

bool BaseMode::mustContinueAquisition() const
{
    return _bursting;
}


//------------------------------------------------------------------------------

bool BaseMode::mustCooldown() const
{
    return _cooldownT->interval() != 0;
}

//------------------------------------------------------------------------------
// OpticalSnapMode
//------------------------------------------------------------------------------

OpticalSnapMode::OpticalSnapMode(IModeToManager &manager,
                                 const Crystal &crystal,
                                 double wavelength,
                                 int exposure,
                                 int cooldownTime,
                                 int cooldownPwr,
                                 int stabilisationTime,
                                 bool burst,
                                 bool record,
                                 const QString &dataFolder,
                                 const QString &session)
    : BaseMode(manager, crystal, exposure, cooldownTime, cooldownPwr,
               stabilisationTime, burst, record, dataFolder, session)
    , _wavelength(wavelength)
{

}

//------------------------------------------------------------------------------

void OpticalSnapMode::setAcousticWave()
{
    _temperature = _manager.temperature();
    _crystal.computeFreqPow(_wavelength,
                            _temperature,
                            _frequency,
                            _power);
    _manager.setAcousticBeam(_frequency, _power);
}

//------------------------------------------------------------------------------

void OpticalSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    gImageBuffer.set(snapshotBuffer);
    _manager.snapshotReadyforGui();
    _manager.saveSnapshot(_snapTime,
                          'S',
                          _wavelength,
                          _frequency,
                          _power,
                          1,
                          _exposure,
                          _refTemperature,
                          snapshotBuffer);

}

//------------------------------------------------------------------------------
// AcousticSnapMode
//------------------------------------------------------------------------------

AcousticSnapMode::AcousticSnapMode(IModeToManager &manager,
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
                                   const QString &session)
    : BaseMode(manager, crystal, exposure, cooldownTime, cooldownPwr,
               stabilisationTime, burst, record, dataFolder, session)
    , _frequency(frequency)
    , _power(power)
{

}

//------------------------------------------------------------------------------

void AcousticSnapMode::setAcousticWave()
{
    _temperature = _manager.temperature();
    _wavelength = _crystal.wavelength(_frequency, _temperature);
    _manager.setAcousticBeam(_frequency, _power);
}

//------------------------------------------------------------------------------

void AcousticSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    gImageBuffer.set(snapshotBuffer);
    _manager.snapshotReadyforGui();
    _manager.saveSnapshot(_snapTime,
                          'A',
                          _wavelength,
                          _frequency,
                          _power,
                          1,
                          _exposure,
                          _refTemperature,
                          snapshotBuffer);
}

//------------------------------------------------------------------------------
// ObservationMode
//------------------------------------------------------------------------------

ObservationMode::ObservationMode(IModeToManager &manager,
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
                                 const QString &session)
    : BaseMode(manager, crystal, exposure, cooldownTime, cooldownPwr,
               stabilisationTime, burst, record, dataFolder, session)
    , _wavelengths{0, wavelength1, wavelength2}
    , _snapshotPerObs(snapshotPerObs)
    , _idx(0)
    , _snapshotCount(0)
{
    _frequency[0]=0.0;
    _power[0]=0.0;
}

//------------------------------------------------------------------------------

void ObservationMode::setAcousticWave()
{
    if (_snapshotCount == 0)
    {
        // First snapshot... initialisation of the observation
        _temperature = _manager.temperature();

        for(int s=0; s<3; ++s)
            for(int i=0; i<snapshotSize; ++i)
                for(int j=0; j<snapshotSize; ++j)
                    _snapshotBuffers[s][i][j] = 0;

        _crystal.computeFreqPow(_wavelengths[1],
                                _temperature,
                                _frequency[1],
                                _power[1]);
        _crystal.computeFreqPow(_wavelengths[2],
                                _temperature,
                                _frequency[2],
                                _power[2]);
    }

    if (_snapshotCount < _snapshotPerObs)
    {
        // First '_snapshotPerObs' snapshot are black snapshots
        _manager.setAcousticBeam(0.0, 0.0);
    } else {
        _manager.setAcousticBeam(_frequency[_idx], _power[_idx]);
    }
}

//------------------------------------------------------------------------------

void ObservationMode::acousticBeamReady()
{
    if (_snapshotCount == 0)
        _snapTime = QDateTime::currentDateTime();
    _manager->takeSnapshot();
}

//------------------------------------------------------------------------------

void ObservationMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    // Add the current snapshot to the consolidated one.
    for(int i=0; i<snapshotSize; ++i)
        for(int j=0; j<snapshotSize; ++j)
            _snapshotBuffers[_idx][i][j] += snapshotBuffer[i][j];

    ++_snapshotCount;

    if (_snapshotCount < _snapshotPerObs)
    {
        _idx = 0;
    }
    else if (_snapshotCount == _snapshotPerObs)
    {
        _idx = 1;
    }
    else if  (_snapshotCount < 3*_snapshotPerObs)
    {
        _idx = 3 - _idx; // 1->2, 2->1
    }
    else
    {
        _snapshotCount = 0;
        _idx = 0;

        for(int i=0; i<3; ++i)
            saveSnapshot(_snapTime,
                         'O',
                         _wavelengths[i],
                         _frequency[i],
                         _power[i],
                         _snapshotPerObs,
                         _exposure,
                         _refTemperature,
                         _snapshotBuffers[i]);

        gImageBuffer.set(_snapshotBuffers[1]);
        _manager.snapshotReadyforGui();
    }
}

//------------------------------------------------------------------------------

bool ObservationMode::mustContinueAquisition() const
{
    return BaseMode::mustContinueAquisition() || _snapshotCount != 0;
    // An observation continues until is is done.
}

//------------------------------------------------------------------------------

bool ObservationMode::mustCooldown() const
{
    return BaseMode::mustCooldown() && _snapshotCount == 0;
    // No cooldown within an observation
}

//------------------------------------------------------------------------------
// SweepMode
//------------------------------------------------------------------------------

SweepMode::SweepMode(IModeToManager &manager,
                     const Crystal &crystal,
                     double minWavelength,
                     double maxWavelength,
                     double wavelengthStep,
                     int    blackSnapshotRate,
                     int exposure,
                     int cooldownTime,
                     int cooldownPwr,
                     int stabilisationTime,
                     bool burst,
                     bool record,
                     const QString &dataFolder,
                     const QString &session)
    : BaseMode(manager, crystal, exposure, cooldownTime, cooldownPwr,
               stabilisationTime, burst, record, dataFolder, session)
    , _minWavelength(minWavelength)
    , _maxWavelength(maxWavelength+1e-5) // 1e-5 to account for rounding error
    , _wavelengthStep(wavelengthStep)
    , _blackSnapshotRate(blackSnapshotRate)
    , _counter(0)
{
}

//------------------------------------------------------------------------------

void SweepMode::setAcousticWave()
{
    _temperature = _manager.temperature();

    if (0 == _counter)
    {
        _manager.setAcousticBeam(0.0, 0.0); // Black snapshot
    } else {
        _crystal.computeFreqPow(_wavelength,
                                _temperature,
                                _frequency,
                                _power);
        _manager.setAcousticBeam(_frequency, _power);
    }
}

//------------------------------------------------------------------------------

void SweepMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    if (0 == _counter)
    {
        saveSnapshot(_snapTime,
                     'W',
                     0.0,
                     0.0,
                     0.0,
                     1,
                     _exposure,
                     _refTemperature,
                     _snapshotBuffer);
    } else {
        saveSnapshot(_snapTime,
                     Sweep,
                     _wavelength,
                     _frequency,
                     _power,
                     1,
                     _exposure,
                     _refTemperature,
                     _snapshotBuffer);

        _wavelength += _wavelengthStep;
        if (_bursting && _maxWavelength < _wavelength)
        {
            _wavelength = _minWavelength;
            _counter = -1; // Will be set to 0 by the ++ operator below.
        }
    }

    gImageBuffer.set(_snapshotBuffer);
    _manager.snapshotReadyforGui();

    ++_counter;
    if (_counter == _blackSnapshotRate+1)
        _counter = 0;
}

//------------------------------------------------------------------------------

bool SweepMode::mustContinueAquisition() const
{
    return BaseMode::mustContinueAquisition()
           || _wavelength <= _maxWavelength;
}

//------------------------------------------------------------------------------

}
