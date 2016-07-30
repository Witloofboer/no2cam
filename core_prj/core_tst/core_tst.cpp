#include <QTest>

#include <QSettings>

#include "CrystalTest.h"
#include "CoreTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("BIRA-IASB");
    QCoreApplication::setApplicationName("NO2_Cam_test");

    int result = 0;

    result |= QTest::qExec(new AotfCrystalTest, argc, argv);
    result |= QTest::qExec(new CoreTest, argc, argv);

    QSettings settings;
    settings.remove("");

    return result;
}
