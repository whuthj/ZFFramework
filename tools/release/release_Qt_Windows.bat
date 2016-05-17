@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set QT_TYPE=Qt_Windows
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\%QT_TYPE%

rmdir /s/q "%RELEASE_PATH%" >nul 2>nul
del /s/q "%ZF_ROOT_PATH%\_tmp\%QT_TYPE%\*.dll" >nul 2>nul
del /s/q "%ZF_ROOT_PATH%\_tmp\%QT_TYPE%\*.a" >nul 2>nul

call "%ZF_ROOT_PATH%\thirdparty_setup.bat"
call "%ZF_TOOLS_PATH%\common\run_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_release_Qt_Windows*.bat"

