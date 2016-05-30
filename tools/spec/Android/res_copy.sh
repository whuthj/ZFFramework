#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
RES_PATH=$1
RELEASE_PATH=$2
if test "x$RES_PATH" = "x" || test "x$RELEASE_PATH" = "x" ; then
    echo usage:
    echo   res_copy.sh RES_PATH RELEASE_PATH
    exit 1
fi

mkdir -p "$RELEASE_PATH" >/dev/null 2>&1
cp -rf "$RES_PATH/." "$RELEASE_PATH/" >/dev/null 2>&1

