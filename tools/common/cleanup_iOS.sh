#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x$PROJ_ROOT" = "x" || test "x$PROJ_NAME" = "x" ; then
    echo usage:
    echo   cleanup_iOS.sh PROJ_ROOT PROJ_NAME
    exit 1
fi

find $PROJ_ROOT -name "xcuserdata*" -type d 2>&1 | xargs rm -rf >/dev/null 2>&1
find $PROJ_ROOT -name "xcshareddata*" -type d 2>&1 | xargs rm -rf >/dev/null 2>&1
find $PROJ_ROOT -name ".DS_Store" -type f 2>&1 | xargs rm >/dev/null 2>&1

