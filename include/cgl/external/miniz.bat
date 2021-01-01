@echo off
echo amalgamate miniz
pause

set file="..\miniz.h"
cd miniz
type miniz_common.h>%file%
type miniz.h>>%file%
type miniz_tdef.h>>%file%
type miniz_tinfl.h>>%file%
type miniz_zip.h>>%file%

type miniz.c>>%file%
type miniz_tdef.c>>%file%
type miniz_tinfl.c>>%file%
type miniz_zip.c>>%file%

pause
