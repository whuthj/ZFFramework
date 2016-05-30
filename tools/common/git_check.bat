@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_GIT=%~1%
set DST_PATH=%~2%

if not defined PROJ_GIT goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   git_check.bat PROJ_GIT DST_PATH
exit /b 1
:run

set _OLD_DIR=%cd%
if exist "%DST_PATH%\.git" (
    call "%WORK_DIR%\timestamp_check.bat" "%DST_PATH%\.git" 3600

    if errorlevel 1 (
        cd "%DST_PATH%"
        git checkout .
        git reset --hard
        git clean -xdf
        git pull
        cd "%_OLD_DIR%"

        call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git" 3600
    )
) else (
    rmdir /s/q "%DST_PATH%" >nul 2>nul
    git clone "%PROJ_GIT%" "%DST_PATH%"

    call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git" 3600
)

