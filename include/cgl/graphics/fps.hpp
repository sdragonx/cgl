/*

 fps.hpp

 sdragonx 2018-01-08 06:31:24

 //计算fps的小工具

*/
#ifndef FPS_HPP_20180108063124
#define FPS_HPP_20180108063124

#include <time.h>

namespace cgl{

float fps_stats()
{
	static int fps_total = 0;
	static int fps = 0;
	static int t = std::clock();
	++fps;
	if(std::clock() - t > 1000)
	{
		fps_total = fps;
		fps = 0;
		t = std::clock();
	}
	return fps_total;
}

}// end namespace cgl

#endif //FPS_HPP_20180108063124
