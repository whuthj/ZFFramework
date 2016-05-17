@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%~1%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

call "%ZF_TOOLS_PATH%\common\git_check.bat" "https://github.com/sswater/deelx" "%WORK_DIR%\_repo\deelx"
call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\_repo\deelx\deelx.h" "%WORK_DIR%\..\src\_repo\deelx\deelx.h"

