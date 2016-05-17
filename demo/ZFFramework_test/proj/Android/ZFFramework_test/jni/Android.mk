# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

LOCAL_PATH := $(call my-dir)

ZF_NEED_ZFFramework = 1
ZF_NEED_ZFFramework_impl = 1
ZF_NEED_ZFFramework_impl_ZFUIWebKit = 1

ifeq ($(OS),Windows_NT)
    $(info $(shell call ..\..\..\..\..\tools\release\release_Android_check.bat))

    $(info $(shell rmdir /s/q assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\tools\spec\Android\res_copy.bat ZFFramework_test ZFFramework_test ..\..\..\res assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\tools\spec\Android\res_fix.bat assets\res >nul 2>nul))
    ifeq ($(ZF_NEED_ZFFramework),1)
        $(info $(shell xcopy /s/e/y/r/h ..\..\..\..\..\_release\Android\ZF . >nul 2>nul))
    endif
    ifeq ($(ZF_NEED_ZFFramework_impl),1)
        $(info $(shell xcopy /s/e/y/r/h ..\..\..\..\..\_release\Android\ZF_impl . >nul 2>nul))
    endif
    ifeq ($(ZF_NEED_ZFFramework_impl_ZFUIWebKit),1)
        $(info $(shell xcopy /s/e/y/r/h ..\..\..\..\..\_release\Android\ZF_impl_ZFUIWebKit . >nul 2>nul))
    endif
else
    $(info $(shell sh ../../../../../tools/release/release_Android_check.sh))

    $(info $(shell rm -rf assets/res >/dev/null 2>&1))
    $(info $(shell sh ../../../../../tools/spec/Android/res_copy.sh ZFFramework_test ZFFramework_test ../../../res assets/res))
    $(info $(shell sh ../../../../../tools/spec/Android/res_fix.sh assets/res))
    ifeq ($(ZF_NEED_ZFFramework),1)
        $(info $(shell cp -rf ../../../../../_release/Android/ZF/. ./ >/dev/null 2>&1))
    endif
    ifeq ($(ZF_NEED_ZFFramework_impl),1)
        $(info $(shell cp -rf ../../../../../_release/Android/ZF_impl/. ./ >/dev/null 2>&1))
    endif
    ifeq ($(ZF_NEED_ZFFramework_impl_ZFUIWebKit),1)
        $(info $(shell cp -rf ../../../../../_release/Android/ZF_impl_ZFUIWebKit/. ./ >/dev/null 2>&1))
    endif
endif

include jni/jni_ZFFramework_test/Android.mk

