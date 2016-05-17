@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_DIR_NAME=%~2%
set RES_PATH=%~3%
set RELEASE_PATH=%~4%

if not defined PROJ_NAME goto :usage
if not defined PROJ_DIR_NAME goto :usage
if not defined RES_PATH goto :usage
if not defined RELEASE_PATH goto :usage
goto :run
:usage
echo usage:
echo   res_copy.bat PROJ_NAME PROJ_DIR_NAME RES_PATH RELEASE_PATH
exit /b 1
:run

mkdir "%RELEASE_PATH%" >nul 2>nul
xcopy /s/e/y/r/h "%RES_PATH%" "%RELEASE_PATH%"

