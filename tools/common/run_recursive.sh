#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_DIR=$1
FILE_NAME=$2

if test "x$SRC_DIR" = "x" || test "x$FILE_NAME" = "x" ; then
    echo usage:
    echo   run_recursive.sh SRC_DIR FILE_NAME
    exit 1
fi

for f in `find "$SRC_DIR" -name "$FILE_NAME"`
do
    echo running $f
    sh $f "$WORK_DIR/../.."
done

