#include "TestCase.h"
#include "TestHarness.h"

TestCase::TestCase() : QObject()
{
    TestHarness::append(this);
}
