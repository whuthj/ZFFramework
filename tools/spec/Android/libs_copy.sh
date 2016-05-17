#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_DIR_NAME=$2
PROJ_PATH=$3
RELEASE_PATH=$4
if test "x$PROJ_NAME" = "x" || test "x$PROJ_DIR_NAME" = "x" || test "x$PROJ_PATH" = "x" || test "x$RELEASE_PATH" = "x" ; then
    echo usage:
    echo   libs_copy.sh PROJ_NAME PROJ_DIR_NAME PROJ_PATH RELEASE_PATH
    exit 1
fi

find "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_DIR_NAME" -name "*$PROJ_NAME.so" 2>&1 | xargs rm >/dev/null 2>&1
mkdir -p "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_DIR_NAME" >nul 2>nul
cp -rf "$PROJ_PATH/libs/." "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_DIR_NAME/" >/dev/null 2>&1
find "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_DIR_NAME" ! -name "*$PROJ_NAME.so" -type f 2>&1 | xargs rm >/dev/null 2>&1

