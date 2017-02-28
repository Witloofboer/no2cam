#include <QApplication>

#include "gui/gui_global.h"
#include <QMessageBox>
#include "Camera.h"
#include "Driver.h"
#include "core/mockups.h"
#include "Probe.h"



//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{

    qSetMessagePattern("%{time hh:mm:ss.zzz} (%{threadid}) %{type}: %{message}");

    QApplication application(argc, argv);

    auto probe = new Probe;                 // comment for fake Probe
    //auto probe = new core::MockProbe;     // uncomment for fake Probe
    auto driver = new Driver;
    auto camera = new HamamatsuCamera();    // comment for fake camera
    //auto camera = new core::MockCamera(); // uncomment for fake camera

    bool ok = camera->init();               // comment for fake camera
    if (!ok) return -1;                     // comment for fake camera

    bool probeOK = probe->init();           // comment for fake Probe
    if (!probeOK)                           // comment 'if structure' for fake Probe
    {
       QMessageBox::critical(0, "Aborting", "Failed to connect to Temperature Sensor.");
       return -1;
    }

    bool driverOK = driver->init();
    if (!driverOK)
    {
        QMessageBox::critical(0, "Aborting", "Failed to connect to RF GENERATOR module.");
        return -1;
    }

    init("",
         QObject::tr("<p>Actual devices are used.</p>"),
         probe, camera, driver);

    start();
    int result = application.exec();

    camera->uninit();                      // comment for fake camera
    probe-> uninit();                      // comment for fake Probe
    finalise();
    return result;
}

//------------------------------------------------------------------------------
