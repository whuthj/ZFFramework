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
echo set ZF_EXCLUDE to exclude dirs, separated by space, match exact, e.g.
echo   set ZF_EXCLUDE=f0 f1
exit /b 1
:run

rem ============================================================
rem default exclude dirs
set ZF_EXCLUDE_TMP=%ZF_EXCLUDE%
set ZF_EXCLUDE_TMP=%ZF_EXCLUDE_TMP% private

rem ============================================================
for /f %%f in ('dir /a-d/s/b %SRC_DIR%\%FILE_NAME%') do (
    call :run_with_filter %%f
)

goto :EOF

:run_with_filter
set _excluded=0
for %%f in (%ZF_EXCLUDE_TMP%) do (
    call :check_filter %1 %%f _excluded
)
if "%_excluded%" == "0" (
    echo running %1
    call %1 "%WORK_DIR%\..\.."
)
goto :EOF

:check_filter
echo %1 | findstr "\%2\\" >nul 2>nul && set %3=1
goto :EOF

