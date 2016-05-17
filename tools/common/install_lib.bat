@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_DIR_NAME=%~2%
set SRC_PATH=%~3%
set DST_PATH=%~4%

if not defined PROJ_NAME goto :usage
if not defined PROJ_DIR_NAME goto :usage
if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   install_lib.bat PROJ_NAME PROJ_DIR_NAME SRC_PATH DST_PATH
exit /b 1
:run

mkdir "%DST_PATH%" >nul 2>nul
copy "%SRC_PATH%\\*.dll" "%DST_PATH%"
copy "%SRC_PATH%\\*.a" "%DST_PATH%"

