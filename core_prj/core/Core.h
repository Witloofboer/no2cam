#ifndef CORE_H
#define CORE_H

#include <QThread>

class Core : public QThread
{
    Q_OBJECT
public:
    Core();
};

#endif // CORE_H
