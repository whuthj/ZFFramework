@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_DIR_NAME=%~2%
set JAR_PROJ_DIR_NAME_OR_DUMMY=%~3%
set LIB_PROJ_DIR_NAME_OR_DUMMY=%~4%

if not defined PROJ_NAME goto :usage
if not defined PROJ_DIR_NAME goto :usage
if not defined JAR_PROJ_DIR_NAME_OR_DUMMY goto :usage
if not defined LIB_PROJ_DIR_NAME_OR_DUMMY goto :usage
goto :run
:usage
echo usage:
echo   release.bat PROJ_NAME PROJ_DIR_NAME JAR_PROJ_DIR_NAME_OR_DUMMY LIB_PROJ_DIR_NAME_OR_DUMMY
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\Android

if not "%JAR_PROJ_DIR_NAME_OR_DUMMY%" == "DUMMY" (
    call "%ZF_TOOLS_PATH%\spec\Android\jar_build.bat" %PROJ_NAME% %PROJ_DIR_NAME% "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\proj\Android\%JAR_PROJ_DIR_NAME_OR_DUMMY%"
    call "%ZF_TOOLS_PATH%\spec\Android\jar_copy.bat" %PROJ_NAME% %PROJ_DIR_NAME% "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\proj\Android\%JAR_PROJ_DIR_NAME_OR_DUMMY%" "%RELEASE_PATH%" >nul 2>nul
)

if not "%LIB_PROJ_DIR_NAME_OR_DUMMY%" == "DUMMY" (
    call "%ZF_TOOLS_PATH%\spec\Android\libs_build.bat" %PROJ_NAME% %PROJ_DIR_NAME% "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\proj\Android\%LIB_PROJ_DIR_NAME_OR_DUMMY%"
    call "%ZF_TOOLS_PATH%\spec\Android\libs_copy.bat" %PROJ_NAME% %PROJ_DIR_NAME% "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\proj\Android\%LIB_PROJ_DIR_NAME_OR_DUMMY%" "%RELEASE_PATH%" >nul 2>nul
)

call "%ZF_TOOLS_PATH%\spec\Android\res_copy.bat" "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\res" "%RELEASE_PATH%\%PROJ_DIR_NAME%\assets\res" >nul 2>nul
call "%ZF_TOOLS_PATH%\spec\Android\res_fix.bat" "%RELEASE_PATH%\%PROJ_DIR_NAME%\assets\res" >nul 2>nul
call "%ZF_TOOLS_PATH%\common\copy_header.bat" "%ZF_ROOT_PATH%\%PROJ_DIR_NAME%\src" "%RELEASE_PATH%\%PROJ_DIR_NAME%\libs\%PROJ_DIR_NAME%\include" >nul 2>nul

