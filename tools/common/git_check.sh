#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_GIT=$1
DST_PATH=$2
if test "x$PROJ_GIT" = "x" || test "x$DST_PATH" = "x" ; then
    echo usage:
    echo   git_check.sh PROJ_GIT DST_PATH
    exit 1
fi

_OLD_DIR=$(pwd)
if test -e "$DST_PATH/.git"; then
    cd "$DST_PATH"
    git checkout .
    git reset --hard
    git clean -xdf
    git pull
else
    rm -rf "$DST_PATH" >/dev/null 2>&1
    git clone "$PROJ_GIT" "$DST_PATH"
fi
cd "$_OLD_DIR"

