//---------------------------------------------------------------------------
#ifdef __GNUC__

#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wsequence-point"    //may be undefined
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wunused-variable"

//#pragma GCC diagnostic ignored "-Wformat-security"




/*
#pragma GCC system_header直到文件结束之间的代码会被编译器视为系统头文件之中的代码。
系统头文件中的代码往往不能完全遵循C标准, 所以头文件之中的警告信息往往不显示。
(除非用 #warning显式指明)。
*/

//#pragma GCC system_header

/*
对部分代码可以去除优化：

#pragma GCC push_options
#pragma GCC optimize ("O0")

code

#pragma GCC pop_options
*/

//---------------------------------------------------------------------------
#elif defined(CGL_PLATFORM_MSVC)

#pragma warning(disable: 4201) // nameless unions are part of C++
#pragma warning(disable: 4127) // constant expression
#pragma warning(disable: 4505) // unreferenced local function has been removed
#pragma warning(disable: 4512) // can't generate assignment operator (so what?)
#pragma warning(disable: 4514) // unreferenced inlines are common
#pragma warning(disable: 4103) // pragma pack
#pragma warning(disable: 4702) // unreachable code
#pragma warning(disable: 4237) // bool
#pragma warning(disable: 4710) // function couldn't be inlined
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
#pragma warning(disable: 4097) // typedef name used as synonym for class-name
#pragma warning(disable: 4786) // identifier was truncated in the debug information
#pragma warning(disable: 4268) // const static/global data initialized to zeros
#pragma warning(disable: 4291) // allow placement new

//---------------------------------------------------------------------------
#elif defined(__BORLANDC__)

#pragma option push
#pragma warn -rch           // unreachable code (break after return in default case)
#pragma warn -ccc           // condition is always true: [enumerated value] & [defined value]
#pragma option -w-8004      //is assigned a value that is never used
#pragma option -w-8008      //Condition is always true
#pragma option -w-8012      //Comparing signed and unsigned values
#pragma option -w-8017      //Redefinition of 'xxx' is not identical
//#pragma option -w-8022    //hides virtual function
#pragma option -w-8057      //Parameter 'xxx' is never used
#pragma option -w-8062      //Previous options and warnings not restored
#pragma option -w-8066      //Unreachable code
#pragma option -w-8070      //Function should return a value


#endif
