#include "Modes.h"

#include "Crystal.h"

namespace core {

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

BaseMode::BaseMode(IModeToManager &manager,
                   const Crystal &crystal)
    : _manager(manager)
    , _crystal(crystal)
{

}

//------------------------------------------------------------------------------

BaseMode::~BaseMode() {}

//------------------------------------------------------------------------------

bool BaseMode::mustContinueAquisition() const
{
    return false;
}

//------------------------------------------------------------------------------

bool BaseMode::canCooldown() const
{
    return true;
}

//------------------------------------------------------------------------------

void BaseMode::start()
{
    setAcousticBeam();
}

//------------------------------------------------------------------------------
// OpticalSnapMode
//------------------------------------------------------------------------------

OpticalSnapMode::OpticalSnapMode(IModeToManager &manager,
                                 const Crystal &crystal,
                                 double wavelength)
    : BaseMode(manager, crystal)
    , _wavelength(wavelength)
{

}

//------------------------------------------------------------------------------

void OpticalSnapMode::setAcousticBeam()
{
    _refTemperature = _manager.temperature();
    _crystal.computeFreqPow(_wavelength,
                            _refTemperature,
                            _frequency,
                            _power);
    _manager.setAcousticBeam(_frequency, _power);
}

//------------------------------------------------------------------------------

void OpticalSnapMode::acousticBeamReady()
{
    _snapTime = QDateTime::currentDateTime();
    _manager.takeSnapshot(_wavelength);
}

//------------------------------------------------------------------------------

void OpticalSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);
    _manager.saveSnapshot(_snapTime,
                          'S',
                          _wavelength,
                          _frequency,
                          _power,
                          1,
                          _refTemperature,
                          snapshotBuffer);
}

//------------------------------------------------------------------------------
// AcousticSnapMode
//------------------------------------------------------------------------------

AcousticSnapMode::AcousticSnapMode(IModeToManager &manager,
                                   const Crystal &crystal,
                                   double frequency,
                                   double power)
    : BaseMode(manager, crystal)
    , _frequency(frequency)
    , _power(power)
{

}

//------------------------------------------------------------------------------

void AcousticSnapMode::setAcousticBeam()
{
    _refTemperature = _manager.temperature();
    _wavelength = _crystal.wavelength(_frequency, _refTemperature);
    _manager.setAcousticBeam(_frequency, _power);
}

//------------------------------------------------------------------------------

void AcousticSnapMode::acousticBeamReady()
{
    _snapTime = QDateTime::currentDateTime();
    _manager.takeSnapshot(0.0);
}

//------------------------------------------------------------------------------

void AcousticSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);
    _manager.saveSnapshot(_snapTime,
                          'A',
                          _wavelength,
                          _frequency,
                          _power,
                          1,
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
                                 int snapshotPerObs)
    : BaseMode(manager, crystal)
    , _wavelengths{0, wavelength1, wavelength2}
    , _snapshotPerObs(snapshotPerObs)
    , _idx(0)
    , _snapshotCount(0)
{
}

//------------------------------------------------------------------------------

void ObservationMode::setAcousticBeam()
{
    if (_snapshotCount == 0)
    {
        // First snapshot... initialisation of the observation
        _snapTime = QDateTime::currentDateTime();
        _refTemperature = _manager.temperature();

        for(int s=0; s<3; ++s)
            for(int i=0; i<snapshotSize; ++i)
                for(int j=0; j<snapshotSize; ++j)
                    _snapshotBuffers[s][i][j] = 0;

        _crystal.computeFreqPow(_wavelengths[1],
                                _refTemperature,
                                _frequency[1],
                                _power[1]);
        _crystal.computeFreqPow(_wavelengths[2],
                                _refTemperature,
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
    _manager.takeSnapshot(_wavelengths[_idx]);
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
            _manager.saveSnapshot(_snapTime,
                                  'O',
                                  _wavelengths[i],
                                  _frequency[i],
                                  _power[i],
                                  _snapshotPerObs,
                                  _refTemperature,
                                  _snapshotBuffers[i]);

        _manager.setSnapshotForGui(_snapshotBuffers[1]);
    }
}

//------------------------------------------------------------------------------

bool ObservationMode::mustContinueAquisition() const
{
    return _snapshotCount != 0;
    // An observation continues until is is done.
}

//------------------------------------------------------------------------------

bool ObservationMode::canCooldown() const
{
    return _snapshotCount == 0;
    // No cooldown within an observation
}

//------------------------------------------------------------------------------
// DoasMode
//------------------------------------------------------------------------------

DoasMode::DoasMode(IModeToManager &manager,
                   const Crystal &crystal,
                   const QVector<double> &wavelengths,
                   int nbrSeqPerObs)
    : BaseMode(manager, crystal)
    , _wavelengths()
    , _frequencies(1+wavelengths.size()) // +1 for black snapshot
    , _powers(1+wavelengths.size())
    , _snapshotBuffers(new Snapshot[1+wavelengths.size()])
    , _nbrSeqPerObs(nbrSeqPerObs)
    , _refTemperature(0) // will be set at sequence beginning
    , _wlIx(0)
    , _seqIx(0)
{
    _wavelengths.push_back(0); // First snapshot is the black one
    _wavelengths.append(wavelengths);

}

//------------------------------------------------------------------------------

DoasMode::~DoasMode()
{
    delete [] _snapshotBuffers;
}

//------------------------------------------------------------------------------

void DoasMode::setAcousticBeam()
{
    if (_wlIx == 0)
    {
        if (_seqIx == 0)
        {   // Start of a new observation
            _snapTime = QDateTime::currentDateTime();
            for (int i=0; i<_wavelengths.size(); ++i)
                clear(_snapshotBuffers[i]);
        }

        // Initialise the freqs and powers
        _refTemperature = _manager.temperature();

        _frequencies[0] = 0;
        _powers[0] = 0;
        for (int i=1; i<_wavelengths.size(); ++i)
            _crystal.computeFreqPow(_wavelengths[i],
                                    _refTemperature,
                                    _frequencies[i],
                                    _powers[i]);
    }

    _manager.setAcousticBeam(_frequencies[_wlIx], _powers[_wlIx]);
}

//------------------------------------------------------------------------------

void DoasMode::acousticBeamReady()
{
    _manager.takeSnapshot(_wavelengths[_wlIx]);
}

//------------------------------------------------------------------------------

void DoasMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    // Add the current snapshot to the consolidated one.
    for(int i=0; i<snapshotSize; ++i)
        for(int j=0; j<snapshotSize; ++j)
            _snapshotBuffers[_wlIx][i][j] += snapshotBuffer[i][j];

    ++_wlIx;

    if (_wlIx == _wavelengths.size())
    {
        _wlIx = 0;
        ++_seqIx;
    }

    if (_seqIx == _nbrSeqPerObs)
    {
        _wlIx = 0;
        _seqIx = 0;

        for(int i=0; i<_wavelengths.size(); ++i)
            _manager.saveSnapshot(_snapTime,
                                  'D',
                                  _wavelengths[i],
                                  _frequencies[i],
                                  _powers[i],
                                  _nbrSeqPerObs,
                                  _refTemperature,
                                  _snapshotBuffers[i]);

        Snapshot displaySnap;

        for(int i=0; i<snapshotSize; ++i)
            for(int j=0; j<snapshotSize; ++j)
                displaySnap[i][j]=0;

        for(int k=0; k<_wavelengths.size(); ++k)
        {
            const Snapshot &snap = _snapshotBuffers[k];

            for(int i=0; i<snapshotSize; ++i)
                for(int j=0; j<snapshotSize; ++j)
                    if (displaySnap[i][j] < snap[i][j]) displaySnap[i][j] = snap[i][j];
        }
        _manager.setSnapshotForGui(displaySnap);
    }
}

//------------------------------------------------------------------------------

bool DoasMode::mustContinueAquisition() const
{
    return  _wlIx != 0 || _seqIx != 0;
    // An observation continues until is is done.
}

//------------------------------------------------------------------------------

bool DoasMode::canCooldown() const
{
    return !mustContinueAquisition();
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
                     int    blackSnapshotRate)
    : BaseMode(manager, crystal)
    , _minWavelength(minWavelength)
    , _maxWavelength(maxWavelength+1e-5) // 1e-5 to account for rounding error
    , _wavelengthStep(wavelengthStep)
    , _blackSnapshotRate(blackSnapshotRate)
    , _wavelength(minWavelength)
    , _counter(0)
{
}

//------------------------------------------------------------------------------

void SweepMode::setAcousticBeam()
{
    _refTemperature = _manager.temperature();

    if (0 == _counter)
    {
        qDebug("Snapshot: black");
        _manager.setAcousticBeam(0.0, 0.0); // Black snapshot
    } else {

        if (_maxWavelength < _wavelength)
        {
            _wavelength = _minWavelength;
        }

        qDebug("Snapshot: %.3f nm", _wavelength);
        _crystal.computeFreqPow(_wavelength,
                                _refTemperature,
                                _frequency,
                                _power);
        _manager.setAcousticBeam(_frequency, _power);
    }
}

//------------------------------------------------------------------------------

void SweepMode::acousticBeamReady()
{
    _snapTime = QDateTime::currentDateTime();
    _manager.takeSnapshot(_wavelength);
}

//------------------------------------------------------------------------------

void SweepMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);

    if (0 == _counter)
    {
        _manager.saveSnapshot(_snapTime,
                              'W',
                              0.0,
                              0.0,
                              0.0,
                              1,
                              _refTemperature,
                              snapshotBuffer);
    } else {
        _manager.saveSnapshot(_snapTime,
                              'W',
                              _wavelength,
                              _frequency,
                              _power,
                              1,
                              _refTemperature,
                              snapshotBuffer);

        _wavelength += _wavelengthStep;
    }

    ++_counter;
    if (_counter == _blackSnapshotRate+1)
        _counter = 0;
}

//------------------------------------------------------------------------------

bool SweepMode::mustContinueAquisition() const
{
    return _wavelength <= _maxWavelength;
}

//------------------------------------------------------------------------------

}
