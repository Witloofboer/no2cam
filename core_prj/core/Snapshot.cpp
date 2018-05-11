#include "Snapshot.h"

namespace core
{
void clear(Snapshot& snapshot)
{
    for(int i=0; i<snapshotSize; ++i)
        for(int j=0; j<snapshotSize; ++j)
            snapshot[i][j] = 0;
}

}
