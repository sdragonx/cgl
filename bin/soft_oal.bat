implib -a soft_oal.lib soft_oal.dll

rem libvorbis.dll libvorbisfile.dll
rem dlltool -d soft_oal.def -l libsoft_oal.a

rem E:\MyStudio\mingw32\bin\dlltool -D soft_oal.dll -d soft_oal.def -l libsoft_oal.a
rem E:\MyStudio\mingw32\bin\dlltool -z soft_oal.def --export-all-symbol soft_oal.dll

pause
