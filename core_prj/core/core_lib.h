#ifndef CORE_LIB_H
#define CORE_LIB_H

#include "core_global.h"

//------------------------------------------------------------------------------

namespace core {

class Core;

//------------------------------------------------------------------------------

/**
 * Initialises the core layer.
 */
CORESHARED_EXPORT void init();


/**
 * Starts the core layer thread.
 *
 * \note: the core singleton is moved to that thread.
 */
CORESHARED_EXPORT void start();


/**
 * Shutdowns the core layer thread.
 *
 * \note:
 *     1. The core singleton is moved back to the main thread.
 *     2. The call waits for the end of the thread.
*/
CORESHARED_EXPORT void shutdown();


/**
 * Initialises the core layer.
 */
CORESHARED_EXPORT void finalise();


/**
 * Return the core singleton.
 */
CORESHARED_EXPORT Core *singleton();
}

//------------------------------------------------------------------------------

#endif // CORE_LIB_H
