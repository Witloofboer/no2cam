#include "Camera.h"
#include <QMessageBox>
#include <QString>

#include "dcamprop.h"

//------------------------------------------------------------------------------

HamamatsuCamera::HamamatsuCamera()
    : core::CameraDriver()
    , _timer(new QTimer(this))
{
    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, checkFrameReady);
}

//------------------------------------------------------------------------------

HamamatsuCamera::~HamamatsuCamera() {}

//------------------------------------------------------------------------------

bool HamamatsuCamera::init()
{
    // first initialize the DCAM API software
    DCAMAPI_INIT paraminit;
    memset( &paraminit, 0, sizeof(paraminit) );
    paraminit.size	= sizeof(paraminit);

    _dcamErr = dcamapi_init( &paraminit );

    if( failed( _dcamErr ) )
    {
        qInfo("CAM ERROR ON INIT: %d", _dcamErr); // TODO Error
        return false;
    }


    // once the software is initilized try to open the connected Camera
    qDebug("NR of Counted devices: %d", static_cast<int>(paraminit.iDeviceCount));

    DCAMDEV_OPEN	paramopen;
    memset( &paramopen, 0, sizeof(paramopen) );
    paramopen.size	= sizeof(paramopen);
    paramopen.index	= 0;
    _dcamErr = dcamdev_open( &paramopen );

    if( failed( _dcamErr ) )
    {
        qInfo("CAM ERROR ON DEV OPEN: %d", _dcamErr);
        dcamapi_uninit();
        return false;
    }

    // once the camera could be opened succesfully, couple the device ID to the device handler
    qInfo("INIT DCAM SUCCESFULLY!:");
    _hdcam = paramopen.hdcam;

    memset( &frame, 0, sizeof(frame) );
    frame.size	= sizeof(frame);
    frame.iFrame= -1;		// latest frame

    double value;
    // set binning to 4x4
    //_dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_BINNING, DCAMPROP_BINNING__4);
    _dcamErr = dcamprop_getvalue(_hdcam, DCAM_IDPROP_HIGHDYNAMICRANGE_MODE,&value);
    qDebug("temp = %f",value);
    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE );

    bool state;


    state = dcam_precapture(_hdcam, DCAM_CAPTUREMODE_SNAP);
    state = dcam_allocframe(_hdcam, 1);




    return true;
}

void HamamatsuCamera::checkFrameReady()
{
    int32 Status;
   _dcamErr = dcamcap_status(_hdcam,&Status);

    if(Status == DCAMCAP_STATUS_READY)
    {
        _timer->stop();
        _dcamErr = dcambuf_lockframe(_hdcam, &frame );
        emit snapshotAvailable();
    }
    else
    {
        _timer->setInterval(1);
        _timer->start();
    }
}
//------------------------------------------------------------------------------

void HamamatsuCamera::uninit()
{
    dcambuf_release( _hdcam );
    dcamdev_close( _hdcam );
    dcamapi_uninit();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::setExposure(int exposure)
{
    qInfo("<exposure time: %d ms>", exposure);
    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_EXPOSURETIME, ((double)exposure)/1000 );
    _exposureTime = exposure;
}

//------------------------------------------------------------------------------

void HamamatsuCamera::takeSnapshot()
{
    qInfo("<snapshotting>");
    state = dcam_capture(_hdcam);
    state = dcam_firetrigger(_hdcam);

    _timer->setInterval(_exposureTime+35);
    _timer->start();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::stop()
{
    _timer->stop();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::getSnapshot(core::Snapshot &buffer)
{
   memcpy(buffer, frame.buf, sizeof buffer);
}

//------------------------------------------------------------------------------
