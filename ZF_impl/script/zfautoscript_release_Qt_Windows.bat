@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%~1%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set QT_TYPE=Qt_Windows
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\%QT_TYPE%

call "%ZF_TOOLS_PATH%\spec\Qt\release_Windows.bat" ZFFramework_impl ZF_impl

