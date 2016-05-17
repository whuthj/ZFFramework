@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%~1%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

call "%ZF_TOOLS_PATH%\common\git_check.bat" "https://github.com/zeux/pugixml" "%WORK_DIR%\_repo\pugixml"

call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\_repo\pugixml\src" "%WORK_DIR%\_tmp\pugixml"
call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\zfautoscript_thirdparty_pugixml_template.hpp" "%WORK_DIR%\..\src\_repo\pugixml\pugixml.hpp"

