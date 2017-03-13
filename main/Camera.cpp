#include "Camera.h"
#include <QMessageBox>
#include <QString>
#include "dcamprop.h"
#include <stdlib.h>

//------------------------------------------------------------------------------

HamamatsuCamera::HamamatsuCamera()
    : core::CameraDriver()
    , _timer(new QTimer(this))
{
    _timer->setSingleShot(true);
    connect(_timer, QTimer::timeout, this, checkFrameReady);
}

//------------------------------------------------------------------------------

HamamatsuCamera::~HamamatsuCamera()
{
    dcambuf_release( _hdcam );
    dcamdev_close( _hdcam );
    dcamapi_uninit();
}

bool HamamatsuCamera::init()
{
    // first initialize the DCAM API software
    DCAMAPI_INIT paraminit;
    memset( &paraminit, 0, sizeof(paraminit) );
    paraminit.size	= sizeof(paraminit);

    _dcamErr = dcamapi_init( &paraminit );

    if( failed( _dcamErr ) )
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera init has failed.</p>"
               "<p>dcamapi_init error code: %1").arg(_dcamErr));
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
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera open has failed.</p>"
               "<p>dcamdev_open error code: %1").arg(_dcamErr));
        dcamapi_uninit();
        return false;
    }

    // once the camera could be opened succesfully, couple the device ID to the device handler
    _hdcam = paramopen.hdcam;

    // set the frame variable and allocate 1 frame to the camera
    memset( &frame, 0, sizeof(frame) );
    frame.size	= sizeof(frame);
    frame.iFrame= -1;		// latest frame

    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_BINNING, DCAMPROP_BINNING__4);
    if( failed( _dcamErr ) )
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera binning setting has failed.</p>"
               "<p>dcamprop_setvalue error code: %1").arg(_dcamErr));
        dcamdev_close( _hdcam );
        dcamapi_uninit();
        return false;
    }

    _dcamErr = dcambuf_alloc(_hdcam,1);
    if( failed( _dcamErr ) )
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera buffer allocation has failed.</p>"
               "<p>dcambuf_alloc error code: %1").arg(_dcamErr));
        dcamdev_close( _hdcam );
        dcamapi_uninit();
        return false;
    }

    // allow the software to send the trigger pulse to the camera
    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE );
    if( failed( _dcamErr ) )
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera trigger setting has failed.</p>"
               "<p>dcamprop_setvalue error code: %1").arg(_dcamErr));
        dcambuf_release( _hdcam );
        dcamdev_close( _hdcam );
        dcamapi_uninit();
        return false;
    }

    //start the capture mode of the camera (exposure will begin once camera receives trigger from software)
    _dcamErr = dcamcap_start( _hdcam, DCAMCAP_START_SEQUENCE );
    if( failed( _dcamErr ) )
    {
        QMessageBox::critical(
            0, "Aborting",
            tr("<p>Camera start has failed.</p>"
               "<p>dcamcap_start error code: %1").arg(_dcamErr));
        dcambuf_release( _hdcam );
        dcamdev_close( _hdcam );
        dcamapi_uninit();
        return false;
    }
}

//------------------------------------------------------------------------------

void HamamatsuCamera::setExposure(int exposure)
{
    qDebug("<exposure time: %d ms>", exposure);
    _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_EXPOSURETIME, ((double)exposure)/1000 );
    _timer->setInterval(exposure);
}

//------------------------------------------------------------------------------

void HamamatsuCamera::takeSnapshot()
{
    qDebug("<snapshotting>");
    state = dcam_firetrigger(_hdcam);
    _timer->start();
}

//------------------------------------------------------------------------------

void HamamatsuCamera::checkFrameReady()
{
    state = dcam_wait(_hdcam, &dw, 100, NULL);
    _dcamErr = dcambuf_lockframe(_hdcam, &frame);

    core::Pixel *from = reinterpret_cast<core::Pixel *>(frame.buf);
    for(int i=0; i<512; ++i)
        for(int j=0; j<512; ++j)
            _snap[i][j] = *from++;

    emit snapshotAvailable(_snap);
}

//------------------------------------------------------------------------------

void HamamatsuCamera::stop()
{
    qDebug("<camera stop>");
    _timer->stop();

    state = dcam_idle(_hdcam); // back to idle mode
    qDebug("Camera back to IDLE, error code = %d", state);

    _dcamErr = dcamcap_start( _hdcam, DCAMCAP_START_SEQUENCE );
    qDebug("Camera back to CAPTURE, error code = %x", _dcamErr);
}

//------------------------------------------------------------------------------
