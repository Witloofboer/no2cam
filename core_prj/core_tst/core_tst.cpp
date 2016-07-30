#include <QTest>

#include <QSettings>

#include "CrystalTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2_Cam_test");

    int result = 0;

    result |= QTest::qExec(new AotfCrystalTest, argc, argv);

    QSettings settings;
    settings.remove("");

    return result;
}
