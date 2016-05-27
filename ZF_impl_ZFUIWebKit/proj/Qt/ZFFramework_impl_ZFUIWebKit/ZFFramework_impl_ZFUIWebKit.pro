# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

# ======================================================================
# lib template to build ZFFramework lib
# ======================================================================

# ======================================================================
# ZF settings
# ======================================================================

# whether to build static lib
ZF_BUILD_STATIC_LIB = 0

# whether to use unity builds
# NOTE: you must ensure no Q_OBJECT used while unity builds enabled
ZF_UNITY_BUILD = 0

# ZFFramework's root path
ZF_ROOT_PATH = $$_PRO_FILE_PWD_/../../../..
ZF_TOOLS_PATH = $$ZF_ROOT_PATH/tools

# name of your project
ZF_PROJ_NAME = ZFFramework_impl_ZFUIWebKit

# distribute path name
ZF_PROJ_DIR_NAME = ZF_impl_ZFUIWebKit

# src path of your project
# can hold one or more paths, separated by space
ZF_PROJ_SRC_PATH = $$_PRO_FILE_PWD_/../../../src

# header path to copy to output
ZF_PROJ_HEADER_PATH += $$_PRO_FILE_PWD_/../../../src

# res path of your project
# can hold one or more paths, separated by space
ZF_PROJ_RES_PATH = $$_PRO_FILE_PWD_/../../../res

# whether need this module
ZF_NEED_ZFFramework = 1

# whether need this module
ZF_NEED_ZFFramework_impl = 1

# whether need this module
ZF_NEED_ZFFramework_impl_ZFUIWebKit = 0


# ======================================================================
# your custom project settings here
# ======================================================================
QT += webkitwidgets



# ======================================================================
# Qt project settings, no need to change for most case
# ======================================================================
INCLUDEPATH += $$_PRO_FILE_PWD_/../../../src

QT += core
equals(ZF_NEED_ZFFramework, 1) {
}
equals(ZF_NEED_ZFFramework_impl, 1) {
    QT += gui widgets
}
equals(ZF_NEED_ZFFramework_impl_ZFUIWebKit, 1) {
    QT += webkitwidgets
}

TARGET = $$ZF_PROJ_NAME
TEMPLATE = lib
equals(ZF_BUILD_STATIC_LIB, 1) {
    CONFIG += staticlib
}

QMAKE_CXXFLAGS += -Wno-unused-parameter
CONFIG += warn_off

win32 {
    DEFINES += ZF_ENV_EXPORT=__declspec(dllexport)
}


# ======================================================================
# no need to change these
# ======================================================================
win32 {
    _ZF_QT_TYPE=Qt_Windows
}
unix:!macx {
    _ZF_QT_TYPE=Qt_Posix
}
macx {
    _ZF_QT_TYPE=Qt_MacOS
}

CONFIG(debug, debug|release) {
    _ZF_BUILD_TYPE=debug
} else {
    _ZF_BUILD_TYPE=release
}

win32 {
    _ZF_copy_lib = $$shell_path($$ZF_TOOLS_PATH/common/copy_lib.bat)
    _ZF_copy_header = $$shell_path($$ZF_TOOLS_PATH/common/copy_header.bat)
    _ZF_copy_res = $$shell_path($$ZF_TOOLS_PATH/common/copy_res.bat)
} else {
    _ZF_copy_lib = sh $$shell_path($$ZF_TOOLS_PATH/common/copy_lib.sh)
    _ZF_copy_header = sh $$shell_path($$ZF_TOOLS_PATH/common/copy_header.sh)
    _ZF_copy_res = sh $$shell_path($$ZF_TOOLS_PATH/common/copy_res.sh)
}

win32 {
    _ZF_LIB_SRC_PATH = $$shell_path($$OUT_PWD/$$_ZF_BUILD_TYPE)
} else {
    _ZF_LIB_SRC_PATH = $$shell_path($$OUT_PWD)
}

# ======================================================================

equals(ZF_UNITY_BUILD, 1) {
    win32 {
        _ZF_unity_build_cmd = call $$shell_path($$ZF_TOOLS_PATH/common/unity_build.bat)
    } else {
        _ZF_unity_build_cmd = sh $$shell_path($$ZF_TOOLS_PATH/common/unity_build.sh)
    }
    for(src_path, ZF_PROJ_SRC_PATH) {
        _ZF_COMPILE_MODULE_NAME = $$basename(src_path)
        _ZF_UNITY_BUILD_FILE = $$_PRO_FILE_PWD_/zfgensrc_$${ZF_PROJ_NAME}_$${_ZF_COMPILE_MODULE_NAME}.cpp
        system($$_ZF_unity_build_cmd $$shell_path($$_ZF_UNITY_BUILD_FILE) $$shell_path($$src_path))
        SOURCES += $$shell_path($$_ZF_UNITY_BUILD_FILE)
    }
} else {
    win32 {
        for(path, ZF_PROJ_SRC_PATH) {
            SOURCES += $$system("dir /s /b $$shell_path($$path\\*.cpp)")
            HEADERS += $$system("dir /s /b $$shell_path($$path\\*.h)")
            HEADERS += $$system("dir /s /b $$shell_path($$path\\*.hh)")
            HEADERS += $$system("dir /s /b $$shell_path($$path\\*.hpp)")
        }
    } else {
        for(path, ZF_PROJ_SRC_PATH) {
            SOURCES += $$system("find $$shell_path($$path) -name \*.cpp")
            HEADERS += $$system("find $$shell_path($$path) -name \*.h")
            HEADERS += $$system("find $$shell_path($$path) -name \*.hh")
            HEADERS += $$system("find $$shell_path($$path) -name \*.hpp")
        }
    }
}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

equals(ZF_NEED_ZFFramework, 1) {
    INCLUDEPATH += $$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/ZF/include
    equals(ZF_BUILD_STATIC_LIB, 0) {
        LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/ZF/lib -lZFFramework
    }
}
equals(ZF_NEED_ZFFramework_impl, 1) {
    INCLUDEPATH += $$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/ZF_impl/include
    equals(ZF_BUILD_STATIC_LIB, 0) {
        LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/ZF_impl/lib -lZFFramework_impl
    }
}
equals(ZF_NEED_ZFFramework_impl_ZFUIWebKit, 1) {
    INCLUDEPATH += $$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/ZF_impl_ZFUIWebKit/include
    equals(ZF_BUILD_STATIC_LIB, 0) {
        LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/ZF_impl_ZFUIWebKit/lib -lZFFramework_impl_ZFUIWebKit
    }
}

QMAKE_POST_LINK += $$_ZF_copy_lib $$ZF_PROJ_NAME $$ZF_PROJ_DIR_NAME $$shell_path($$_ZF_LIB_SRC_PATH) $$shell_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/$$ZF_PROJ_DIR_NAME/lib) $$escape_expand(\\n\\t)
for(path, ZF_PROJ_HEADER_PATH) {
    QMAKE_POST_LINK += $$_ZF_copy_header $$shell_path($$path) $$shell_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/$$ZF_PROJ_DIR_NAME/include) $$escape_expand(\\n\\t)
}
for(path, ZF_PROJ_RES_PATH) {
    QMAKE_POST_LINK += $$_ZF_copy_res $$shell_path($$path) $$shell_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/$$ZF_PROJ_DIR_NAME/res) $$escape_expand(\\n\\t)
}

