# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

# ======================================================================
# app template for ZFFramework
# ======================================================================

# ======================================================================
# ZF settings
# ======================================================================

# whether the lib is build as static lib
ZF_BUILD_STATIC_LIB = 0

# whether to use unity builds
# NOTE: you must ensure no Q_OBJECT used while unity builds enabled
ZF_UNITY_BUILD = 0

# ZFFramework's root path
ZF_ROOT_PATH = $$_PRO_FILE_PWD_/../../../../..
ZF_TOOLS_PATH = $$ZF_ROOT_PATH/tools

# name of your project
ZF_PROJ_NAME = TestZF

# build path
ZF_BUILD_PATH = $$_PRO_FILE_PWD_/../../../../../_tmp

# src path of your project
# can hold one or more paths, separated by space
ZF_PROJ_SRC_PATH =
ZF_PROJ_SRC_PATH += $$ZF_ROOT_PATH/ZF/src
ZF_PROJ_SRC_PATH += $$ZF_ROOT_PATH/ZF_impl/src
ZF_PROJ_SRC_PATH += $$ZF_ROOT_PATH/ZF_impl_ZFUIWebKit/src
ZF_PROJ_SRC_PATH += $$ZF_ROOT_PATH/demo/ZFFramework_test/src

# res path of your project
# can hold one or more paths, separated by space
ZF_PROJ_RES_PATH =
ZF_PROJ_RES_PATH += $$ZF_ROOT_PATH/ZF/res
ZF_PROJ_RES_PATH += $$ZF_ROOT_PATH/ZF_impl/res
ZF_PROJ_RES_PATH += $$ZF_ROOT_PATH/ZF_impl_ZFUIWebKit/res
ZF_PROJ_RES_PATH += $$ZF_ROOT_PATH/demo/ZFFramework_test/res


# ======================================================================
# your custom project settings here
# ======================================================================
QT += gui widgets webkitwidgets
INCLUDEPATH += $$ZF_ROOT_PATH/ZF/src
INCLUDEPATH += $$ZF_ROOT_PATH/ZF_impl/src
INCLUDEPATH += $$ZF_ROOT_PATH/ZF_impl_ZFUIWebKit/src

win32 {
    system(call $$shell_path($$ZF_ROOT_PATH/thirdparty_setup.bat))
}
unix {
    system(sh $$shell_path($$ZF_ROOT_PATH/thirdparty_setup.sh))
}




# ======================================================================
# Qt project settings, no need to change for most case
# ======================================================================
QT += core

TARGET = $$ZF_PROJ_NAME
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-unused-parameter
CONFIG += warn_off

exists($${ZF_PROJ_NAME}_icon.ico) {
    RC_ICONS = $${ZF_PROJ_NAME}_icon.ico
}
exists($${ZF_PROJ_NAME}_icon.icns) {
    ICON = $${ZF_PROJ_NAME}_icon.icns
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

DESTDIR = $$ZF_BUILD_PATH/$$ZF_PROJ_NAME/$$_ZF_QT_TYPE/$$_ZF_BUILD_TYPE
OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

win32 {
    _ZF_copy_res = $$shell_path($$ZF_TOOLS_PATH/common/copy_res.bat)
    _ZF_install_lib = $$shell_path($$ZF_TOOLS_PATH/common/install_lib.bat)
} else {
    _ZF_copy_res = sh $$shell_path($$ZF_TOOLS_PATH/common/copy_res.sh)
    _ZF_install_lib = sh $$shell_path($$ZF_TOOLS_PATH/common/install_lib.sh)
}

equals(ZF_BUILD_STATIC_LIB, 1) {
    _ZF_install_lib = echo
}

win32 {
    _ZF_RES_DEPLOY_PATH=$$shell_path($$DESTDIR/res)
    _ZF_LIB_DEPLOY_PATH=$$shell_path($$DESTDIR/.)
}
unix:!macx {
    _ZF_RES_DEPLOY_PATH=$$shell_path($$DESTDIR/res)
    _ZF_LIB_DEPLOY_PATH=$$shell_path($$DESTDIR/.)
}
macx {
    _ZF_RES_DEPLOY_PATH=$$shell_path($$DESTDIR/"$$TARGET".app/Contents/Resources/res)
    _ZF_LIB_DEPLOY_PATH=$$shell_path($$DESTDIR/"$$TARGET".app/Contents/Frameworks)
}

# ======================================================================

exists(qt_main.cpp) {
    SOURCES += qt_main.cpp
}

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

equals(ZF_BUILD_STATIC_LIB, 1) {
    macx {
        _ZF_LINKER_FLAGS = -Wl,-all_load
    } else {
        _ZF_LINKER_FLAGS = -Wl,--whole-archive
    }
} else {
    _ZF_LINKER_FLAGS =
}

for(path, ZF_PROJ_RES_PATH) {
    QMAKE_POST_LINK += $$_ZF_copy_res $$shell_path($$path) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
}

equals(ZF_BUILD_STATIC_LIB, 0) {
    unix:!macx {
        QMAKE_LFLAGS += -Wl,--rpath=${ORIGIN}
    }
}

macx {
    QMAKE_POST_LINK += macdeployqt $$shell_path($$DESTDIR/"$$TARGET".app) >/dev/null 2>&1 $$escape_expand(\\n\\t)
}

