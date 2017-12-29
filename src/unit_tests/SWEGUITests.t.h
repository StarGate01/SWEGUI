#include <cxxtest/TestSuite.h>

namespace swegui_tests
{
    class SWEGUITests;
}

class swegui_tests::SWEGUITests : public CxxTest::TestSuite
{
   
    public:

        void testTrue()
        {
            TS_ASSERT(true);
        }

};