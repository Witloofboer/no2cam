#ifndef GUI_GLOBAL_H
#define GUI_GLOBAL_H

#include <QtCore/qglobal.h>

namespace core {
class AbstractCrysTempProbe;
class BaseCamera;
class AbstractGenerator;
class AbstractDriver;
}

//------------------------------------------------------------------------------

#if defined(GUI_LIBRARY)
#  define GUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define GUISHARED_EXPORT Q_DECL_IMPORT
#endif

//------------------------------------------------------------------------------

/**
 * Initialise the Core layer.
 *
 * @param version
 *
 *@param camera the camera to use. The core layer takes ownership of the camera
 *    as the Core singleton becomes the camera's parent.
 */
GUISHARED_EXPORT void init(const QString& version,
                           const QString& releaseNotes,
                           core::AbstractCrysTempProbe *crysTempProb,
                           core::BaseCamera *camera,
                           core::AbstractGenerator *generator,
                           core::AbstractDriver *driver);
GUISHARED_EXPORT void start();
GUISHARED_EXPORT void shutdown();
GUISHARED_EXPORT void finalise();

//------------------------------------------------------------------------------

#endif // GUI_GLOBAL_H
