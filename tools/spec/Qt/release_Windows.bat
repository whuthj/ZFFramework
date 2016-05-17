@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_DIR_NAME=%~2%

if not defined PROJ_NAME goto :usage
if not defined PROJ_DIR_NAME goto :usage
goto :run
:usage
echo usage:
echo   release_Windows.bat PROJ_NAME PROJ_DIR_NAME
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set QT_TYPE=Qt_Windows
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\%QT_TYPE%

set _OLD_DIR=%cd%
mkdir "%ZF_ROOT_PATH%\_tmp\%QT_TYPE%\%PROJ_DIR_NAME%" >nul 2>nul
cd "%ZF_ROOT_PATH%\_tmp\%QT_TYPE%\%PROJ_DIR_NAME%"
qmake "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\proj\Qt\%PROJ_NAME%\%PROJ_NAME%.pro" -r -spec win32-g++
mingw32-make -j2
cd "%_OLD_DIR%"

