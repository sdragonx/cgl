/*

 display_mode.hpp

 sdragonx 2018-05-09 14:36:31

*/
#ifndef DISPLAY_MODE_HPP_20180509143631
#define DISPLAY_MODE_HPP_20180509143631

//#include "native_window.hpp"

namespace cgl{
namespace pf{

int set_display_mode(void* display, int width, int height, int bpp, int refresh_rate, bool fullscreen);


get_display_mode(void* display, int& width, int& height, int& bpp, int& refresh_rate);
enum_display_mode(void* display, ...);

}//end namespace platform
}//end namespace cgl

#endif //DISPLAY_MODE_HPP_20180509143631
