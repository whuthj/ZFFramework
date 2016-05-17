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
    cd "%DST_PATH%"
    git checkout .
    git reset --hard
    git clean -xdf
    git pull
) else (
    rmdir /s/q "%DST_PATH%" >nul 2>nul
    git clone "%PROJ_GIT%" "%DST_PATH%"
)
cd "%_OLD_DIR%"

