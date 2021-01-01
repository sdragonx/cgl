
#include <cgl/vec.hpp>

namespace cgl{
namespace platform{

#ifdef CGL_PLATFORM_WINDOWS

vec2i get_cursorpos()
{
    POINT pos;
    if (GetCursorPos(&pos))
    {
        ScreenToClient(window->win32.handle, &pos);

        if (xpos)
            *xpos = pos.x;
        if (ypos)
            *ypos = pos.y;
    }
    return vec2i(pos.x, pos.y);
}

int set_cursorpos(const vec2i& pos)
{
    POINT pt = {pos.x, pos.y};
    GetCursorPos(&pt);
    return 0;
}

    #endif

}
}
