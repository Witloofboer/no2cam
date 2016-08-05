#ifndef GUI_GLOBAL_H
#define GUI_GLOBAL_H

#include <QtCore/qglobal.h>

//------------------------------------------------------------------------------

#if defined(GUI_LIBRARY)
#  define GUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define GUISHARED_EXPORT Q_DECL_IMPORT
#endif

//------------------------------------------------------------------------------

GUISHARED_EXPORT void init(const char *version);
GUISHARED_EXPORT void start();
GUISHARED_EXPORT void shutdown();
GUISHARED_EXPORT void finalise();

//------------------------------------------------------------------------------

#endif // GUI_GLOBAL_H
