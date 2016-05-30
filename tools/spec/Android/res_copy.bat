@echo off
setlocal

set WORK_DIR=%~dp0
set RES_PATH=%~1%
set RELEASE_PATH=%~2%

if not defined RES_PATH goto :usage
if not defined RELEASE_PATH goto :usage
goto :run
:usage
echo usage:
echo   res_copy.bat RES_PATH RELEASE_PATH
exit /b 1
:run

mkdir "%RELEASE_PATH%" >nul 2>nul
xcopy /s/e/y/r/h "%RES_PATH%" "%RELEASE_PATH%"

