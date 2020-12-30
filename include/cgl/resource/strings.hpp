/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 strings.hpp

 2016-06-28 10:51:25

 这个应该在ui目录下

 cgl::resource::strings<T>::common_text

*/
#ifndef STRINGS_HPP_20160628105125
#define STRINGS_HPP_20160628105125

#include <cgl/public.h>

namespace cgl{
namespace resource{

template<typename T>
struct constant_text
{
    const static T* common_text;
    const static T* common_name;
    const static T* common_image;
    const static T* common_font;

    const static T* null;

    const static T* root;
    const static T* main;
    const static T* window;
    const static T* state;
    const static T* defname;
    const static T* classname;
    const static T* name;
    const static T* title;
    const static T* command;
    const static T* image;
    const static T* hint;
    const static T* frame;
    const static T* cursor;
    const static T* font;
    const static T* pen;
    const static T* brush;
    const static T* rect;
    const static T* scale_mode;
    const static T* constant_mode;
    const static T* scale;
    const static T* constant;
    const static T* client;
    const static T* show_title;
    const static T* enable;
    const static T* visible;
    const static T* select;
    const static T* checked;
    const static T* transparent;
    const static T* clipclient;
    const static T* style;
    const static T* x;
    const static T* y;
    const static T* width;
    const static T* height;
    const static T* left;
    const static T* top;
    const static T* right;
    const static T* bottom;
    const static T* mode;
};

template<typename T>const T* constant_text<T>::common_text  = T_TEXT(T, "@text");
template<typename T>const T* constant_text<T>::common_name  = T_TEXT(T, "@name");
template<typename T>const T* constant_text<T>::common_image = T_TEXT(T, "@image");
template<typename T>const T* constant_text<T>::common_font  = T_TEXT(T, "@font");

template<typename T>const T* constant_text<T>::null = T_TEXT(T, "null");

template<typename T>const T* constant_text<T>::root = T_TEXT(T, "root");
template<typename T>const T* constant_text<T>::main = T_TEXT(T, "main");
template<typename T>const T* constant_text<T>::window = T_TEXT(T, "window");
template<typename T>const T* constant_text<T>::state = T_TEXT(T, "state");
template<typename T>const T* constant_text<T>::classname = T_TEXT(T, "class");
template<typename T>const T* constant_text<T>::name = T_TEXT(T, "name");
template<typename T>const T* constant_text<T>::title = T_TEXT(T, "title");
template<typename T>const T* constant_text<T>::command = T_TEXT(T, "command");
template<typename T>const T* constant_text<T>::image = T_TEXT(T, "image");
template<typename T>const T* constant_text<T>::hint = T_TEXT(T, "hint");
template<typename T>const T* constant_text<T>::frame = T_TEXT(T, "frame");
template<typename T>const T* constant_text<T>::cursor = T_TEXT(T, "cursor");
template<typename T>const T* constant_text<T>::font = T_TEXT(T, "font");
template<typename T>const T* constant_text<T>::pen = T_TEXT(T, "pen");
template<typename T>const T* constant_text<T>::brush = T_TEXT(T, "brush");
template<typename T>const T* constant_text<T>::rect = T_TEXT(T, "rect");
template<typename T>const T* constant_text<T>::scale_mode = T_TEXT(T, "scale_mode");
template<typename T>const T* constant_text<T>::constant_mode = T_TEXT(T, "constant_mode");
template<typename T>const T* constant_text<T>::scale = T_TEXT(T, "scale");
template<typename T>const T* constant_text<T>::constant = T_TEXT(T, "constant");
template<typename T>const T* constant_text<T>::client = T_TEXT(T, "client");
template<typename T>const T* constant_text<T>::show_title = T_TEXT(T, "showtitle");
template<typename T>const T* constant_text<T>::enable = T_TEXT(T, "enable");
template<typename T>const T* constant_text<T>::visible = T_TEXT(T, "visible");
template<typename T>const T* constant_text<T>::select = T_TEXT(T, "select");
template<typename T>const T* constant_text<T>::checked = T_TEXT(T, "checked");
template<typename T>const T* constant_text<T>::transparent = T_TEXT(T, "transparent");
template<typename T>const T* constant_text<T>::clipclient = T_TEXT(T, "clipclient");
template<typename T>const T* constant_text<T>::style = T_TEXT(T, "style");
template<typename T>const T* constant_text<T>::x = T_TEXT(T, "x");
template<typename T>const T* constant_text<T>::y = T_TEXT(T, "y");
template<typename T>const T* constant_text<T>::width = T_TEXT(T, "width");
template<typename T>const T* constant_text<T>::height = T_TEXT(T, "height");
template<typename T>const T* constant_text<T>::left = T_TEXT(T, "left");
template<typename T>const T* constant_text<T>::top = T_TEXT(T, "top");
template<typename T>const T* constant_text<T>::right = T_TEXT(T, "right");
template<typename T>const T* constant_text<T>::bottom = T_TEXT(T, "bottom");
template<typename T>const T* constant_text<T>::mode = T_TEXT(T, "mode");

}//end namespace resource
}//end namespace cgl

#endif //STRINGS_HPP_20160628105125
