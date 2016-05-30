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
    sh "$WORK_DIR/timestamp_check.sh" "$DST_PATH/.git" 3600

    if test ! $? = "0" ; then
        cd "$DST_PATH"
        git checkout .
        git reset --hard
        git clean -xdf
        git pull
        cd "$_OLD_DIR"

        sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git" 3600
    fi
else
    rm -rf "$DST_PATH" >/dev/null 2>&1
    git clone "$PROJ_GIT" "$DST_PATH"
    sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git" 3600
fi

