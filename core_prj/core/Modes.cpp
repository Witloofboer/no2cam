#include "Modes.h"

#include "Crystal.h"
#include "Manager.h"

namespace core {

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

BaseMode::BaseMode(Manager &manager,
                   const Crystal &crystal,
                   int baseExposure,
                   double refWavelength,
                   double exposureFactor)
    : _manager(manager)
    , _crystal(crystal)
    , _baseExposure(baseExposure)
    , _refWavelength(refWavelength)
    , _exposureFactor(exposureFactor)
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

OpticalSnapMode::OpticalSnapMode(Manager &manager,
                                 const Crystal &crystal,
                                 double wavelength,
                                 int exposure,
                                 double refWavelength,
                                 double exposureFactor)
    : BaseMode(manager, crystal, exposure, refWavelength, exposureFactor)
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
    _manager.takeSnapshot(_baseExposure);
}

//------------------------------------------------------------------------------

void OpticalSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);
    _manager.saveSnapshot(_snapTime,
                          0,
                          "snap",
                          _baseExposure,
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

AcousticSnapMode::AcousticSnapMode(Manager &manager,
                                   const Crystal &crystal,
                                   double frequency,
                                   double power,
                                   int exposure)
    : BaseMode(manager, crystal, exposure, 0, 0)
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
    _manager.takeSnapshot(_baseExposure);
}

//------------------------------------------------------------------------------

void AcousticSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);
    _manager.saveSnapshot(_snapTime,
                          0,
                          "acou",
                          _baseExposure,
                          _wavelength,
                          _frequency,
                          _power,
                          1,
                          _refTemperature,
                          snapshotBuffer);
}


//------------------------------------------------------------------------------
// Generic mode
//------------------------------------------------------------------------------

GenericMode::GenericMode(Manager &manager,
                         const Crystal &crystal,
                         const QVector<double> &wavelengths,
                         const QString &mode,
                         int nbrSeqPerObs,
                         int exposure,
                         double refWavelength,
                         double exposureFactor)
    : BaseMode(manager, crystal, exposure, refWavelength, exposureFactor)
    , _wavelengths(wavelengths)
    , _frequencies(wavelengths.size())
    , _powers(wavelengths.size())
    , _exposures(wavelengths.size())
    , _snapshotBuffers(new Snapshot[wavelengths.size()])
    , _mode(mode)
    , _nbrSeqPerObs(nbrSeqPerObs)
    , _refTemperature(0) // will be set at sequence beginning
    , _wlIx(0)
    , _seqIx(0)
{
}

//------------------------------------------------------------------------------

GenericMode::~GenericMode()
{
    delete [] _snapshotBuffers;
}

//------------------------------------------------------------------------------

void GenericMode::setAcousticBeam()
{
    if (_wlIx == 0 && _seqIx == 0)
    {
        _snapTime = QDateTime::currentDateTime();
        for (int i=0; i<_wavelengths.size(); ++i) clear(_snapshotBuffers[i]);

        _refTemperature = _manager.temperature();

        _frequencies[0] = 0;
        _powers[0] = 0;
        _exposures[0] = _baseExposure;
        for (int i=1; i<_wavelengths.size(); ++i)
        {
            if (_wavelengths[i] == 0)
            {
                _frequencies[i] = 0;
                _powers[i] = 0;
                _exposures[i] = _baseExposure;
            }
            else
            {
                _crystal.computeFreqPow(_wavelengths[i],
                                        _refTemperature,
                                        _frequencies[i],
                                        _powers[i]);

                _exposures[i] = 0.5
                        + _baseExposure
                        * (1.0-(_wavelengths[i]-_refWavelength)*_exposureFactor*0.01);
            }
        }
    }

    qDebug("Initiating snapshot at %.1f nm", _wavelengths[_wlIx]);
    _manager.setAcousticBeam(_frequencies[_wlIx], _powers[_wlIx]);
}

//------------------------------------------------------------------------------

void GenericMode::acousticBeamReady()
{
    _manager.takeSnapshot(_exposures[_wlIx]);
}

//------------------------------------------------------------------------------

void GenericMode::processSnapshot(const Snapshot &snapshotBuffer)
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
                                  i,
                                  _mode,
                                  _exposures[i],
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

bool GenericMode::mustContinueAquisition() const
{
    return  _wlIx != 0 || _seqIx != 0;
    // An observation continues until is is done.
}

//------------------------------------------------------------------------------

bool GenericMode::canCooldown() const
{
    return !mustContinueAquisition();
    // No cooldown within an observation
}

//------------------------------------------------------------------------------

}
