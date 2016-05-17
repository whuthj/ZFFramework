# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

#============================================================
# project settings
#============================================================

ZF_NEED_ZFFramework = 1
ZF_NEED_ZFFramework_impl = 1
ZF_NEED_ZFFramework_impl_ZFUIWebKit = 0

# module name
# use System.loadLibrary("module_name"); to load it
ZF_MODULE_NAME          = ZF2048

# source directories to search, under project_path,
# set to "jni" to search whole project_path/jni path
# support relative path
# path must not contain spaces
# path must use '/' as path separator
# may contain more than one directories, separated by space
ZF_SRC_DIRS             = ../../../src

# whether use unity build
ZF_UNITY_BUILD          = true
ifeq ($(OS),Windows_NT)
ZF_UNITY_BUILD_SCRIPT   = ../../../../../tools/common/unity_build.bat
else
ZF_UNITY_BUILD_SCRIPT   = ../../../../../tools/common/unity_build.sh
endif

# source extensions, separated by space
ZF_SRC_EXTS             = c cpp cxx

# true or false
ZF_BUILD_SHARED         = true

# extra include path setting, separated by space
ZF_INCLUDES             =
ifeq ($(ZF_NEED_ZFFramework),1)
    ZF_INCLUDES         += libs/ZF/include
endif
ifeq ($(ZF_NEED_ZFFramework_impl),1)
    ZF_INCLUDES         += libs/ZF_impl/include
endif
ifeq ($(ZF_NEED_ZFFramework_impl_ZFUIWebKit),1)
    ZF_INCLUDES         += libs/ZF_impl_ZFUIWebKit/include
endif


# compiler flags
ZF_CFLAGS               = -Os

# linker flags
ZF_LFLAGS               = -landroid -llog

# third-party libs to load
ZF_LOAD_STATIC_LIB      =
ZF_LOAD_SHARED_LIB      =
ifeq ($(ZF_NEED_ZFFramework),1)
    ZF_LOAD_SHARED_LIB  += ZFFramework
endif
ifeq ($(ZF_NEED_ZFFramework_impl),1)
    ZF_LOAD_SHARED_LIB  += ZFFramework_impl
endif
ifeq ($(ZF_NEED_ZFFramework_impl_ZFUIWebKit),1)
    ZF_LOAD_SHARED_LIB  += ZFFramework_impl_ZFUIWebKit
endif


#============================================================
# LOCAL_PATH set only once, before any other code
#============================================================
#LOCAL_PATH := $(call my-dir)

#============================================================
# other custom settings
#============================================================
ifeq ($(ZF_NEED_ZFFramework),1)
    include $(CLEAR_VARS)
    LOCAL_MODULE := ZFFramework
    LOCAL_SRC_FILES := ../libs/ZF/$(TARGET_ARCH_ABI)/libZFFramework.so
    include $(PREBUILT_SHARED_LIBRARY)
endif

ifeq ($(ZF_NEED_ZFFramework_impl),1)
    include $(CLEAR_VARS)
    LOCAL_MODULE := ZFFramework_impl
    LOCAL_SRC_FILES := ../libs/ZF_impl/$(TARGET_ARCH_ABI)/libZFFramework_impl.so
    include $(PREBUILT_SHARED_LIBRARY)
endif

ifeq ($(ZF_NEED_ZFFramework_impl_ZFUIWebKit),1)
    include $(CLEAR_VARS)
    LOCAL_MODULE := ZFFramework_impl_ZFUIWebKit
    LOCAL_SRC_FILES := ../libs/ZF_impl_ZFUIWebKit/$(TARGET_ARCH_ABI)/libZFFramework_impl_ZFUIWebKit.so
    include $(PREBUILT_SHARED_LIBRARY)
endif







#============================================================
# no need to change these
#============================================================

#============================================================
# find source files recursively
# usage: $(call zf_find_src_files,$(src_dirs),$(src_exts))
# e.g.:
#   # dirs to search, relative to project_path
#   # set to jni to use whole project_path/jni path
#   SRCDIRS := dir1 dir2
#   # file exts to search
#   SRCEXTS := c cpp
#   # result
#   FILES   := $(call zf_find_src_files, $(SRCDIRS), $(SRCEXTS))
# note:
#   * assume your project is under "c:\project\",
#     with jni lib whose path is "c:\project\jni\some_path\my_lib",
#     then you should cd to "c:\project\",
#     and set SRCDIRS to "jni\some_path\my_lib",
#     then ndk-build
ifeq ($(OS),Windows_NT)
_zf_ls_win = ../$(subst \,/,$(1))$(subst \,/,$(subst $(abspath $(1)),,$(2)))
_zf_ls = $(foreach file,$(shell dir $(subst /,\,$(1)) /a-d /b /s),$(call _zf_ls_win,$(subst \,/,$(1)),$(subst \,/,$(file))))
else
_zf_ls = $(shell find $(1) -type f)
endif
_zf_find_src_files = $(subst \,/,$(filter %.$(2),$(call _zf_ls, $(1))))
zf_find_src_files = $(foreach file,$(foreach srcdir,$(1),$(foreach srcext,$(2),$(call _zf_find_src_files,$(subst \,/,$(srcdir)),$(srcext)))),../$(file))
#============================================================

include $(CLEAR_VARS)

LOCAL_MODULE := $(ZF_MODULE_NAME)
LOCAL_C_INCLUDES := $(ZF_INCLUDES)
ifeq ($(ZF_UNITY_BUILD),true)
ifeq ($(OS),Windows_NT)
LOCAL_SRC_FILES := $(foreach src_path,$(ZF_SRC_DIRS),$(subst $(info $(shell call $(subst /,\,$(ZF_UNITY_BUILD_SCRIPT)) $(subst /,\,gen/zfgensrc_$(ZF_MODULE_NAME)_$(notdir $(src_path)).cpp) $(subst /,\,$(src_path)))),,../gen/zfgensrc_$(ZF_MODULE_NAME)_$(notdir $(src_path)).cpp))
else
LOCAL_SRC_FILES := $(foreach src_path,$(ZF_SRC_DIRS),$(subst $(info $(shell sh $(ZF_UNITY_BUILD_SCRIPT) gen/zfgensrc_$(ZF_MODULE_NAME)_$(notdir $(src_path)).cpp $(src_path))),,../gen/zfgensrc_$(ZF_MODULE_NAME)_$(notdir $(src_path)).cpp))
endif
else
LOCAL_SRC_FILES := $(call zf_find_src_files,$(ZF_SRC_DIRS),$(ZF_SRC_EXTS))
endif
LOCAL_CFLAGS := $(ZF_CFLAGS)
LOCAL_LDLIBS := $(ZF_LFLAGS)

#LOCAL_SHORT_COMMANDS = true

LOCAL_STATIC_LIBRARIES := $(ZF_LOAD_STATIC_LIB)
LOCAL_SHARED_LIBRARIES := $(ZF_LOAD_SHARED_LIB)
ifeq ($(ZF_BUILD_SHARED),true)
include $(BUILD_SHARED_LIBRARY)
else
include $(BUILD_STATIC_LIBRARY)
endif
