#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

int main(int argc, char **argv)
{
    auto runTests = [&]() {
        InitGoogleTest(&argc, argv);
        RUN_ALL_TESTS();
    };

    runTests();

#ifdef _WIN32
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
}
