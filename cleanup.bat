@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

rmdir /s/q "%ZF_ROOT_PATH%\_release" >nul 2>nul
rmdir /s/q "%ZF_ROOT_PATH%\_tmp" >nul 2>nul

call "%ZF_TOOLS_PATH%\common\run_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_cleanup*.bat"

