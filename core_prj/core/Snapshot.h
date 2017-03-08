#ifndef SNAPHOT_H
#define SNAPHOT_H

#include <QtGlobal>

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

const qint64 snapshotSize = 2048;

typedef quint16 Pixel;
typedef Pixel Snapshot[snapshotSize][snapshotSize];



//------------------------------------------------------------------------------

}

#endif // SNAPHOT_H
