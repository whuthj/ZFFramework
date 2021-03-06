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
echo   libs_copy.bat PROJ_NAME PROJ_DIR_NAME PROJ_PATH RELEASE_PATH
exit /b 1
:run

del /s/q "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs\%PROJ_DIR_NAME%\*%PROJ_NAME%.so" >nul 2>nul
mkdir "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs\%PROJ_DIR_NAME%" >nul 2>nul
xcopy /s/e/y/r/h "%PROJ_PATH%\libs\*%PROJ_NAME%.so" "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs\%PROJ_DIR_NAME%"

