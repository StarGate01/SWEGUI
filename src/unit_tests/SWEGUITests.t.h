#include <cxxtest/TestSuite.h>
#include <SFML/Graphics.hpp>
#include "../renderer/NetCdfImageStream.hpp"

namespace swegui_tests
{
    class SWEGUITests;
}

class swegui_tests::SWEGUITests : public CxxTest::TestSuite
{
   
    public:

        void testImageStream()
        {
            renderer::NetCdfImageStream netcdf_stream;
            bool ret = netcdf_stream.open("/mnt/c/Users/Christoph/Documents/Studium/5_WS1718/TSISIM/Tutorium/SWE/build/data/swe_prod_00.nc");
            TS_ASSERT(ret);
            ret = netcdf_stream.select(renderer::NetCdfImageStream::Variable::B, 0);
            TS_ASSERT(ret);

            sf::Image bath_img;
            ret = bath_img.loadFromStream(netcdf_stream);
            TS_ASSERT(ret);
            ret = bath_img.saveToFile("/mnt/c/Users/Christoph/Desktop/test.bmp");
            TS_ASSERT(ret);
            
            // int max_tex = sf::Texture::getMaximumSize();
            // cout << max_tex << endl;
            // sf::Rect<int> area(0, 0, 
            //     max(netcdf_stream.meta_info->width, max_tex),
            //     max(netcdf_stream.meta_info->height, max_tex));

            // sf::Texture bath_tex;
            // ret = bath_tex.loadFromImage(bath_img);
            // TS_ASSERT(ret);
        }

};