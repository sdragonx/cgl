/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 android.hpp

 2020-08-21 02:56:30

*/
#ifndef ANDROID_HPP_20200821025630
#define ANDROID_HPP_20200821025630

#include <cgl/public.h>
#include <android/native_activity.h>

namespace cgl{
namespace frame{

class native_app
{
};

class native_window
{
public:
    ANativeWindow* m_handle;

    native_window() : m_handle(0)
    {

    }

    native_window(native_app& app) : m_handle(0)
    {
    }

    ANativeWindow* handle()
    {
        return m_handle;
    }

    int set_window(ANativeWindow* window)
    {
        m_handle = window;
        return 0;
    }

    int width()
    {
        return ANativeWindow_getWidth(m_handle);
    }

    int height()
    {
        return ANativeWindow_getHeight(m_handle);
    }


};

}//end namespace frame
}//end namespace cgl

#endif //ANDROID_HPP_20200821025630
