#ifndef SNAPHOT_H
#define SNAPHOT_H

#include <QtGlobal>

//------------------------------------------------------------------------------

namespace core {

//------------------------------------------------------------------------------

const qint64 snapshotSize = 512;

typedef quint16 Pixel;
typedef Pixel Snapshot[snapshotSize][snapshotSize];

extern void clear(Snapshot& snapshot);

//------------------------------------------------------------------------------

}

#endif // SNAPHOT_H
