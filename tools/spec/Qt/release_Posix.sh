#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_DIR_NAME=$2
if test "x$PROJ_NAME" = "x" || test "x$PROJ_DIR_NAME" = "x" ; then
    echo usage:
    echo   release_Posix.command PROJ_NAME PROJ_DIR_NAME
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
QT_TYPE=Qt_Posix
RELEASE_PATH=$ZF_ROOT_PATH/_release/$QT_TYPE

_OLD_DIR=$(pwd)
mkdir -p "$ZF_ROOT_PATH/_tmp/$QT_TYPE/$PROJ_DIR_NAME" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/$QT_TYPE/$PROJ_DIR_NAME"
qmake "$ZF_ROOT_PATH/$PROJ_DIR_NAME/proj/Qt/$PROJ_NAME/$PROJ_NAME.pro" -r -spec linux-g++
make -j2
cd "$_OLD_DIR"

