@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

call "%ZF_TOOLS_PATH%\common\run_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_thirdparty*.bat"

