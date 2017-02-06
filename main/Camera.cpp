#include "Camera.h"
#include <QMessageBox>
#include <QString>
#include "dcamprop.h"

//------------------------------------------------------------------------------

HamamatsuCamera::HamamatsuCamera()
    : core::CameraDriver()
{
}

//------------------------------------------------------------------------------

HamamatsuCamera::~HamamatsuCamera() {}

//------------------------------------------------------------------------------

void HamamatsuCamera::init()
{
    // first initialize the DCAM API software
    DCAMAPI_INIT paraminit;
    memset( &paraminit, 0, sizeof(paraminit) );
    paraminit.size	= sizeof(paraminit);

    _dcamErr = dcamapi_init( &paraminit );

    if( failed( _dcamErr ) )
    {
        qInfo("CAM ERROR ON INIT:");
        qInfo(QString::number(_dcamErr).toStdString().c_str());
        HamamatsuCamera::uninit();
    }
    else
    {
        // once the software is initilized try to open the connected Camera
        qInfo("NR of Counted devices:");
        qInfo(QString::number(paraminit.iDeviceCount).toStdString().c_str());

        DCAMDEV_OPEN	paramopen;
        memset( &paramopen, 0, sizeof(paramopen) );
        paramopen.size	= sizeof(paramopen);
        paramopen.index	= 0;
        _dcamErr = dcamdev_open( &paramopen );

        if( failed( _dcamErr ) )
        {
            qInfo("CAM ERROR ON DEV OPEN:");
            qInfo(QString::number(_dcamErr).toStdString().c_str());
            HamamatsuCamera::uninit();
        }
        else
        {
            // once the camera could be opened succesfully, couple the device ID to the device handler
            qInfo("INIT DCAM SUCCESFULLY!:");
            _hdcam = paramopen.hdcam;

            // allocate 1 buffer inside the camera to store the image
            _dcamErr = dcambuf_alloc( _hdcam, 1 );

            if( failed( _dcamErr ) )
            {
                qInfo("CAM ERROR ON ATTACH:");
                qInfo(QString::number(_dcamErr).toStdString().c_str());
                dcamdev_close(_hdcam);
                HamamatsuCamera::uninit();
            }
            else
            {
                qInfo("BUFFER ALLOC SUCCESFULLY!:");
                // tell the camera it should wait for an event to take a new image (here this is 'frame_ready')
                memset( &waitopen, 0, sizeof(waitopen) );
                waitopen.size = sizeof(waitopen);
                waitopen.hdcam	= _hdcam;

                _dcamErr = dcamwait_open( &waitopen );

                memset( &paramwait, 0, sizeof(paramwait) );
                paramwait.size		= sizeof(paramwait);
                paramwait.eventmask	= DCAMCAP_EVENT_FRAMEREADY;
                paramwait.timeout	= 1000;

                memset( &frame, 0, sizeof(frame) );
                frame.size	= sizeof(frame);
                frame.iFrame= -1;		// latest frame

                // set binning to 4x4
                //_dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_BINNING, DCAMPROP_BINNING__4);

                _dcamErr = dcamprop_setvalue(_hdcam, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE );
                _dcamErr = dcamcap_start( _hdcam, DCAMCAP_START_SEQUENCE );



            }
        }
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
}

//------------------------------------------------------------------------------

void HamamatsuCamera::takeSnapshot()
{
    qInfo("<snapshotting>");

    HDCAMWAIT hwait = waitopen.hwait;

    //let the software trigger the camera to take a snapshot
    dcamcap_firetrigger( _hdcam );
    _dcamErr = dcamwait_start( hwait, &paramwait ); // this function waits until it receives 'frame_ready' event from the camera

    if( failed( _dcamErr ) )
    {
        qInfo("CAM ERROR ON CAPTURE:");
        qInfo(QString::number(_dcamErr).toStdString().c_str());
    }
    else
    {
        _dcamErr = dcambuf_lockframe(_hdcam, &frame );
        emit snapshotAvailable();
    }
}

//------------------------------------------------------------------------------

void HamamatsuCamera::stop()
{
//    if (_timer->isActive())
//    {
    qInfo("Forcing camera stop TO BE IMPLEMENTED");
//        _timer->stop();
//    }
}

//------------------------------------------------------------------------------

void HamamatsuCamera::getSnapshot(core::Snapshot &buffer)
{
    memcpy(buffer, frame.buf, sizeof buffer);
}

//------------------------------------------------------------------------------
