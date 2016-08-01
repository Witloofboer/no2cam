#include "gui_lib.h"

#include <QCoreApplication>

#include "MainWindow.h"

//------------------------------------------------------------------------------

static void gui_init_resource() {
    Q_INIT_RESOURCE(resources); // must be used outside namespace.
}

//------------------------------------------------------------------------------

namespace gui {

MainWindow *singleton_ = nullptr;

//------------------------------------------------------------------------------

void init(const QString& version)
{
    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2 Camera Command Interface");
    gui_init_resource();
    singleton_ = new gui::MainWindow(version);
    singleton_->show();
}

//------------------------------------------------------------------------------

void finalise()
{
    delete singleton_;
    singleton_ = nullptr;
}

//------------------------------------------------------------------------------

}
