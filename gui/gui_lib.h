#ifndef GUI_LIB_H
#define GUI_LIB_H

#include "gui_global.h"

//------------------------------------------------------------------------------

class QString;

namespace gui {

class MainWindow;

//------------------------------------------------------------------------------

GUISHARED_EXPORT void init(const QString& version);
GUISHARED_EXPORT void finalise();

//------------------------------------------------------------------------------

}

#endif // GUI_LIB_H
