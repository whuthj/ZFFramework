@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%~1%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

rmdir /s/q "%WORK_DIR%\..\src\_repo" >nul 2>nul
rmdir /s/q "%WORK_DIR%\_repo" >nul 2>nul
rmdir /s/q "%WORK_DIR%\_tmp" >nul 2>nul

