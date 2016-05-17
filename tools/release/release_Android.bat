@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\Android

rmdir /s/q "%RELEASE_PATH%" >nul 2>nul

call "%ZF_ROOT_PATH%\thirdparty_setup.bat"
call "%ZF_TOOLS_PATH%\common\run_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_release_Android*.bat"

