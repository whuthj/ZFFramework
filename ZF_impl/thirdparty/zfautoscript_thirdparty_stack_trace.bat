@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%~1%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

call "%ZF_TOOLS_PATH%\common\git_check.bat" "https://github.com/ZSaberLv0/stack_trace" "%WORK_DIR%\_repo\stack_trace"

call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\_repo\stack_trace\include" "%WORK_DIR%\_tmp\stack_trace"
call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\_repo\stack_trace\src" "%WORK_DIR%\_tmp\stack_trace"

call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\zfautoscript_thirdparty_stack_trace_template.hpp" "%WORK_DIR%\..\src\_repo\stack_trace\stack.hpp"
call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\zfautoscript_thirdparty_stack_trace_template.cpp" "%WORK_DIR%\..\src\_repo\stack_trace\stack.cpp"

