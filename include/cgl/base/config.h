/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 config.h

 2016-07-03 11:56:40

*/
#ifndef CONFIG_H_20160703115640
#define CONFIG_H_20160703115640

//CGL_MULTI_THREAD

//default cstring<T> cache size
//#define CGL_STRING_CACHE_SIZE 16

//using ilImage
//#define CGL_USING_ILIMAGE        //using devil

//using freetype2
//#define CGL_USING_FREETYPE    //using freetype2

//#ifndef CGL_DEFAULT_FONT_SIZE
//#define CGL_DEFAULT_FONT_SIZE 16
//#endif

//default char type is utf8
#ifdef CGL_PLATFORM_ANDROID
    #ifndef CGL_USING_UTF8_CHAR
        #define CGL_USING_UTF8_CHAR
    #endif
#endif


#endif //CONFIG_H_20160703115640
