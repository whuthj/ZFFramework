#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_DIR_NAME=$2
RES_PATH=$3
RELEASE_PATH=$4
if test "x$PROJ_NAME" = "x" || test "x$PROJ_DIR_NAME" = "x" || test "x$RES_PATH" = "x" || test "x$RELEASE_PATH" = "x" ; then
    echo usage:
    echo   res_copy.sh PROJ_NAME PROJ_DIR_NAME RES_PATH RELEASE_PATH
    exit 1
fi

mkdir -p "$RELEASE_PATH" >/dev/null 2>&1
cp -rf "$RES_PATH/." "$RELEASE_PATH/" >/dev/null 2>&1

