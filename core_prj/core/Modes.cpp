#include "Modes.h"

#include <cstring>
#include <limits>

#include "Crystal.h"
#include "Manager.h"


namespace core {

//------------------------------------------------------------------------------
// BaseMode
//------------------------------------------------------------------------------

BaseMode::BaseMode(Manager &manager,
                   const Crystal &crystal,
                   int exposure)
    : _manager(manager)
    , _crystal(crystal)
    , _exposure(exposure)
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
                                 int exposure)
    : BaseMode(manager, crystal, exposure)
    , _wavelength(wavelength)
{

}

//------------------------------------------------------------------------------

void OpticalSnapMode::setAcousticBeam()
{
    _refTemp = _manager.temperature();
    _crystal.computeFreqPow(_wavelength,
                            _refTemp,
                            _frequency,
                            _power);
    _manager.setAcousticBeam(_frequency, _power);
}

//------------------------------------------------------------------------------

void OpticalSnapMode::acousticBeamReady()
{
    _snapTime = QDateTime::currentDateTime();
    _manager.takeSnapshot(_exposure);
}

//------------------------------------------------------------------------------

void OpticalSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);
    _manager.saveSnapshot(_snapTime,
                          0,
                          "snap",
                          _exposure,
                          _wavelength,
                          _frequency,
                          _power,
                          1,
                          _refTemp,
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
    : BaseMode(manager, crystal, exposure)
    , _frequency(frequency)
    , _power(power)
{

}

//------------------------------------------------------------------------------

void AcousticSnapMode::setAcousticBeam()
{
    _refTemp = _manager.temperature();
    _wavelength = _crystal.wavelength(_frequency, _refTemp);
    _manager.setAcousticBeam(_frequency, _power);
}

//------------------------------------------------------------------------------

void AcousticSnapMode::acousticBeamReady()
{
    _snapTime = QDateTime::currentDateTime();
    _manager.takeSnapshot(_exposure);
}

//------------------------------------------------------------------------------

void AcousticSnapMode::processSnapshot(const Snapshot &snapshotBuffer)
{
    _manager.setSnapshotForGui(snapshotBuffer);
    _manager.saveSnapshot(_snapTime,
                          0,
                          "acou",
                          _exposure,
                          _wavelength,
                          _frequency,
                          _power,
                          1,
                          _refTemp,
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
                         int exposure)
    : BaseMode(manager, crystal, exposure)
    , _wavelengths(wavelengths)
    , _mode(mode)
    , _nbrSeqPerObs(nbrSeqPerObs)
    , _refTemp(0)
    , _wlIx(0)
    , _seqIx(0)
{    
    if (_wavelengths[0] != 0)
    {
        qDebug("Adding black snapshot");
        _wavelengths.push_front(0.0); // Insert a black snapshot as first snapshot.
    }

    _frequencies.resize(_wavelengths.size());
    _powers.resize(_wavelengths.size());
    _accumulators = new AccumulatingBuffer[_wavelengths.size()];

}

//------------------------------------------------------------------------------

GenericMode::~GenericMode()
{
    delete [] _accumulators;
}

//------------------------------------------------------------------------------

void GenericMode::setAcousticBeam()
{
    if (_wlIx == 0 && _seqIx == 0) // Start of the observation?
    {
        memset(_accumulators, 0,
               sizeof(AccumulatingBuffer)*_wavelengths.size());

        // Set the observation reference time and temperature
        _snapTime = QDateTime::currentDateTime();
        _refTemp = _manager.temperature();

        for (int wx=0; wx<_wavelengths.size(); ++wx)
        {
            // Compute the acoustic frequency and power for each wavelength
            if (_wavelengths[wx] == 0)
            {
                _frequencies[wx] = 0;
                _powers[wx] = 0;
            }
            else
            {
                _crystal.computeFreqPow(_wavelengths[wx],
                                        _refTemp,
                                        _frequencies[wx],
                                        _powers[wx]);
            }
        }
    }

    qDebug("Initiating snapshot at %.1f nm", _wavelengths[_wlIx]);
    _manager.setAcousticBeam(_frequencies[_wlIx], _powers[_wlIx]);
}

//------------------------------------------------------------------------------

void GenericMode::acousticBeamReady()
{
    _manager.takeSnapshot(_exposure);
}

//------------------------------------------------------------------------------

void GenericMode::processSnapshot(const Snapshot &snapshotBuffer)
{           
    {
        // Accumulate the current snapshot
        auto& accumSnap = _accumulators[_wlIx];

        for(int i=0; i<snapshotSize; ++i)
            for(int j=0; j<snapshotSize; ++j)
                accumSnap[i][j] += snapshotBuffer[i][j];
    }

    // Move to the next snapshot
    ++_wlIx;


    // If last snapshot from the current sequence then restart the sequence
    if (_wlIx == _wavelengths.size())
    {
        _wlIx = 0;
        ++_seqIx;
    }


    // If last snapshot of the last sequence, save data on disk and display them
    if (_seqIx == _nbrSeqPerObs)
    {
        _wlIx = 0;
        _seqIx = 0;

        Snapshot displaySnap = {0};
        // snapshot to display

        int blkIx;
        // black snapshot index

        const Pixel maxPixel = std::numeric_limits<Pixel>::max();

        // Iterate on all the accumulated buffers
        for(int wx=0; wx<_wavelengths.size(); ++wx)
        {
            Snapshot fileSnap; // Snapshot to save in file;

            const auto& accumSnap = _accumulators[wx];

            if (_wavelengths[wx] == 0)
            {
                blkIx = wx;

                for(int i=0; i<snapshotSize; ++i)
                    for(int j=0; j<snapshotSize; ++j)
                    {
                        const SgnPixel &sp = accumSnap[i][j];
                        Pixel p = static_cast<Pixel>(sp);

                        if (p != sp)
                            p = maxPixel;

                        fileSnap[i][j] = p;
                    }

            } else {

                const AccumulatingBuffer& blkSnap = _accumulators[blkIx];

                for(int i=0; i<snapshotSize; ++i)
                    for(int j=0; j<snapshotSize; ++j)
                    {
                        const SgnPixel sp = accumSnap[i][j] - blkSnap[i][j];
                        Pixel p = static_cast<Pixel>(sp);

                        if (p != sp)
                            p = (sp < 0) ? 0 : maxPixel;

                        fileSnap[i][j] = p;

                        if (displaySnap[i][j] < p)
                            displaySnap[i][j] = p;
                    }
            }

            _manager.saveSnapshot(_snapTime,
                                  wx,
                                  _mode,
                                  _exposure,
                                  _wavelengths[wx],
                                  _frequencies[wx],
                                  _powers[wx],
                                  _nbrSeqPerObs,
                                  _refTemp,
                                  fileSnap);
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
