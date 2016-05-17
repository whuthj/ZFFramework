@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%~1%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\Android

call "%ZF_TOOLS_PATH%\spec\Android\release.bat" ZFFramework_impl ZF_impl ZFFramework_impl ZFFramework_impl_jni

