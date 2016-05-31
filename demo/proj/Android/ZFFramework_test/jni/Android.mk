# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

LOCAL_PATH := $(call my-dir)

ifeq ($(OS),Windows_NT)
    $(info $(shell call ..\..\..\ZFSetup.bat))
    $(info $(shell call ..\..\..\..\..\ZFFramework\tools\release\release_Android_check.bat))

    $(info $(shell call ..\..\..\..\..\ZFFramework\tools\spec\Android\res_copy.bat ..\..\..\res assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\ZFFramework\tools\spec\Android\res_fix.bat assets\res >nul 2>nul))
    $(info $(shell xcopy /s/e/y/r/h ..\..\..\..\..\ZFFramework\_release\Android\ZF . >nul 2>nul))
    $(info $(shell xcopy /s/e/y/r/h ..\..\..\..\..\ZFFramework\_release\Android\ZF_impl . >nul 2>nul))
    $(info $(shell xcopy /s/e/y/r/h ..\..\..\..\..\ZFFramework\_release\Android\ZF_impl_ZFUIWebKit . >nul 2>nul))
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
else
    $(info $(shell sh ../../../ZFSetup.sh))
    $(info $(shell sh ../../../../../ZFFramework/tools/release/release_Android_check.sh))

    $(info $(shell sh ../../../../../ZFFramework/tools/spec/Android/res_copy.sh ../../../res assets/res))
    $(info $(shell sh ../../../../../ZFFramework/tools/spec/Android/res_fix.sh assets/res))
    $(info $(shell cp -rf ../../../../../ZFFramework/_release/Android/ZF/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf ../../../../../ZFFramework/_release/Android/ZF_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf ../../../../../ZFFramework/_release/Android/ZF_impl_ZFUIWebKit/. ./ >/dev/null 2>&1))
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
endif

include jni/jni_ZFFramework_test/Android.mk

