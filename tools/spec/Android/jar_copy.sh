#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_DIR_NAME=$2
PROJ_PATH=$3
RELEASE_PATH=$4
if test "x$PROJ_NAME" = "x" || test "x$PROJ_DIR_NAME" = "x" || test "x$PROJ_PATH" = "x" || test "x$RELEASE_PATH" = "x" ; then
    echo usage:
    echo   jar_copy.sh PROJ_NAME PROJ_DIR_NAME PROJ_PATH RELEASE_PATH
    exit 1
fi

mkdir -p "$RELEASE_PATH/$PROJ_DIR_NAME/libs" >/dev/null 2>&1
rm "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_NAME.jar" >/dev/null 2>&1
cp "$PROJ_PATH/bin/classes.jar" "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_NAME.jar"

