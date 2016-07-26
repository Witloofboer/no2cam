#ifndef CORETHREAD_H
#define CORETHREAD_H

#include <QThread>

class CoreThread: public QThread
{
    Q_OBJECT

protected:
    void run() override;

};

#endif // CORETHREAD_H
