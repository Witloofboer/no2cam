#ifndef TESTHARNESS_H
#define TESTHARNESS_H

class QObject;

namespace TestHarness
{
    void append(QObject *test);
    int runTests(int argc, char *argv[]);
}

#define ADD_TESTCASE(TestCase)
#endif // TESTHARNESS_H
