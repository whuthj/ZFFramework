@echo off
setlocal

set WORK_DIR=%~dp0
set SRC_DIR=%~1%
set FILE_NAME=%~2%

if not defined SRC_DIR goto :usage
if not defined FILE_NAME goto :usage
goto :run
:usage
echo usage:
echo   run_recursive.sh SRC_DIR FILE_NAME
exit /b 1
:run

for /f %%f in ('dir /a-d/s/b %SRC_DIR%\%FILE_NAME%') do (
    echo running %%f
    call %%f "%WORK_DIR%\..\.."
)

