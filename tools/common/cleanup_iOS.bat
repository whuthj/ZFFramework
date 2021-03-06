@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_ROOT=%~1%
set PROJ_NAME=%~2%

if not defined PROJ_ROOT goto :usage
if not defined PROJ_NAME goto :usage
goto :run
:usage
echo usage:
echo   cleanup_iOS.bat PROJ_ROOT PROJ_NAME
exit /b 1
:run

rem ============================================================
rem iOS
>nul 2>nul (
    for /f "tokens=*" %%i in ('dir /a/d /b /s "%PROJ_ROOT%\xcuserdata*"') do (
        rmdir /s/q %%i >nul 2>nul
    )
)

>nul 2>nul (
    for /f "tokens=*" %%i in ('dir /a/d /b /s "%PROJ_ROOT%\xcshareddata*"') do (
        rmdir /s/q %%i >nul 2>nul
    )
)

del /s/q "%PROJ_ROOT%\*.DS_Store" >nul 2>nul

