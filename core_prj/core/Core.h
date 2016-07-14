#ifndef CORE_H
#define CORE_H

#include <QSettings>
#include <QThread>

class Core : public QThread
{
    Q_OBJECT
public:
    Core();
};

extern QSettings gSettings;
extern Core gCore;

#endif // CORE_H
