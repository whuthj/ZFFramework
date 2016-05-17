#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_DIR_NAME=$2
if test "x$PROJ_NAME" = "x" || test "x$PROJ_DIR_NAME" = "x" ; then
    echo usage:
    echo   release.command PROJ_NAME PROJ_DIR_NAME
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/iOS

_OLD_DIR=$(pwd)
cd "$ZF_ROOT_PATH/$PROJ_DIR_NAME/proj/iOS/$PROJ_NAME"
xcodebuild -configuration "Release" -target "${PROJ_NAME}_aggregate" SYMROOT="$ZF_ROOT_PATH/_tmp/build/iOS"
cd "$_OLD_DIR"

