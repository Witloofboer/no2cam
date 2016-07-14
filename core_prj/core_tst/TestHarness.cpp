#include "TestHarness.h"

#include <QTest>
#include <QList>

static QList<QObject *> tests;

void TestHarness::append(QObject *test)
{
    tests.append(test);
}

int TestHarness::runTests(int argc, char *argv[])
{
    int result = 0;

    for(auto it_test = tests.begin(); it_test != tests.end(); ++it_test)
    {
        result |= QTest::qExec(*it_test, argc, argv);
    }

    return result;
}
