@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set OUTPUT_PATH=%WORK_DIR%\Doxygen\..\..\..\_tmp\DoxygenDoc

set PASSWORD=%~1%
if not defined PASSWORD (
    set /p PASSWORD="enter password: "
    if not defined PASSWORD (
        echo "canceled"
        exit /b 1
    )
)


if not exist "%OUTPUT_PATH%\ZFFramework_doc.chm" (
    echo "doc not exist"
    exit /b 1
)

call "%ZF_TOOLS_PATH%\common\git_check.bat" "https://github.com/ZFFramework/zfframework.github.com" "%OUTPUT_PATH%\repo"
call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%WORK_DIR%\doc_repo" "%OUTPUT_PATH%\repo"
rmdir /s/q "%OUTPUT_PATH%\repo\doc" >nul 2>nul
call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%OUTPUT_PATH%\html" "%OUTPUT_PATH%\repo\doc"

set _OLD_DIR=%cd%
cd "%OUTPUT_PATH%\repo"
git config user.email "z@zsaber.com"
git config user.name "ZSaberLv0"
git config push.default "simple"
git add -A
git commit -a -m "update doc"
git push "https://ZSaberLv0:%PASSWORD%@github.com/ZFFramework/zfframework.github.com"
cd "%_OLD_DIR%"

