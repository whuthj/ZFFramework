@echo off
setlocal

set WORK_DIR=%~dp0
set TARGET_FILE=%~1%

if not defined TARGET_FILE goto :usage
goto :run
:usage
echo usage:
echo   unity_build.bat TARGET_FILE [SRC_DIRS, ...]
exit /b 1
:run

set TARGET_FILE_TMP=%TARGET_FILE%.tmp

del /s/q %TARGET_FILE_TMP% >nul 2>nul
md %TARGET_FILE_TMP% >nul 2>nul
rmdir /s/q %TARGET_FILE_TMP% >nul 2>nul

shift /1
:SRC_DIR_LOOP
if "%1" == "" (goto :SRC_DIR_LOOP_END)
>nul 2>nul (
    if exist %1\ (
        dir /a-d/s/b %1\*.c >> %TARGET_FILE_TMP%
        dir /a-d/s/b %1\*.cxx >> %TARGET_FILE_TMP%
        dir /a-d/s/b %1\*.cpp >> %TARGET_FILE_TMP%
        dir /a-d/s/b %1\*.m >> %TARGET_FILE_TMP%
        dir /a-d/s/b %1\*.mm >> %TARGET_FILE_TMP%
    ) else (
        if exist %1 (
            dir /a-d/s/b %1 >> %TARGET_FILE_TMP%
        )
    )
)
shift /1
goto :SRC_DIR_LOOP
:SRC_DIR_LOOP_END

setlocal enabledelayedexpansion
set TARGET_FILE_TMP_2=%TARGET_FILE_TMP%2
del /s/q %TARGET_FILE_TMP_2% >nul 2>nul
for /f "delims=" %%a in (%TARGET_FILE_TMP%) do (
    set v=#include "%%a"
    echo !v!>>%TARGET_FILE_TMP_2%
)
del /s/q %TARGET_FILE_TMP% >nul 2>nul
move %TARGET_FILE_TMP_2% %TARGET_FILE_TMP% >nul 2>&1

echo n|comp %TARGET_FILE% %TARGET_FILE_TMP% >nul 2>&1
if errorlevel 1 (
    rem changed
    del /s/q %TARGET_FILE% >nul 2>nul
    move %TARGET_FILE_TMP% %TARGET_FILE% >nul 2>&1
) else (
    rem not changed
)
del /s/q %TARGET_FILE_TMP% >nul 2>nul
del /s/q %TARGET_FILE_TMP_2% >nul 2>nul

