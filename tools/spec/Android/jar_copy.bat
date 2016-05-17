@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_DIR_NAME=%~2%
set PROJ_PATH=%~3%
set RELEASE_PATH=%~4%

if not defined PROJ_NAME goto :usage
if not defined PROJ_DIR_NAME goto :usage
if not defined PROJ_PATH goto :usage
if not defined RELEASE_PATH goto :usage
goto :run
:usage
echo usage:
echo   jar_copy.bat PROJ_NAME PROJ_DIR_NAME PROJ_PATH RELEASE_PATH
exit /b 1
:run

mkdir "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs" >nul 2>nul
del "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs\%PROJ_NAME%.jar" >nul 2>nul
copy "%PROJ_PATH%\bin\classes.jar" "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs\%PROJ_NAME%.jar"

