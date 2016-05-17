# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

LOCAL_PATH := $(call my-dir)
include jni/jni_ZFFramework/Android.mk
include jni/jni_ZFFramework_impl/Android.mk
include jni/jni_TestZF2048/Android.mk

ifeq ($(OS),Windows_NT)
    $(info $(shell call ..\..\..\..\..\thirdparty_setup.bat))

    $(info $(shell rmdir /s/q assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\tools\spec\Android\res_copy.bat TestZF2048 TestZF2048 ..\..\..\..\..\ZF\res assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\tools\spec\Android\res_copy.bat TestZF2048 TestZF2048 ..\..\..\..\..\ZF_impl\res assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\tools\spec\Android\res_copy.bat TestZF2048 TestZF2048 ..\..\..\..\..\demo\ZF2048\res assets\res >nul 2>nul))
    $(info $(shell call ..\..\..\..\..\tools\spec\Android\res_fix.bat assets\res >nul 2>nul))
else
    $(info $(shell sh ../../../../../thirdparty_setup.sh))

    $(info $(shell rm -rf assets/res >/dev/null 2>&1))
    $(info $(shell sh ../../../../../tools/spec/Android/res_copy.sh TestZF2048 TestZF2048 ../../../../../ZF/res assets/res))
    $(info $(shell sh ../../../../../tools/spec/Android/res_copy.sh TestZF2048 TestZF2048 ../../../../../ZF_impl/res assets/res))
    $(info $(shell sh ../../../../../tools/spec/Android/res_copy.sh TestZF2048 TestZF2048 ../../../../../demo/ZF2048/res assets/res))
    $(info $(shell sh ../../../../../tools/spec/Android/res_fix.sh assets/res))
endif
