#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
RES_PATH=$1
if test "x$RES_PATH" = "x" ; then
    echo usage:
    echo   res_fix.sh RES_PATH
    exit 1
fi

find "$RES_PATH" -type f 2>&1 | xargs -i mv {} {}.mp2 >/dev/null 2>&1

