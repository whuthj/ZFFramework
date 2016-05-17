#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_DIR_NAME=$2
JAR_PROJ_DIR_NAME_OR_DUMMY=$3
LIB_PROJ_DIR_NAME_OR_DUMMY=$4
if test "x$PROJ_NAME" = "x" || test "x$PROJ_DIR_NAME" = "x" || test "x$JAR_PROJ_DIR_NAME_OR_DUMMY" = "x" || test "x$LIB_PROJ_DIR_NAME_OR_DUMMY" = "x" ; then
    echo usage:
    echo   release.sh PROJ_NAME PROJ_DIR_NAME JAR_PROJ_DIR_NAME_OR_DUMMY LIB_PROJ_DIR_NAME_OR_DUMMY
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/Android

if test ! "$JAR_PROJ_DIR_NAME_OR_DUMMY" = "dummy" ; then
    sh "$ZF_TOOLS_PATH/spec/Android/jar_build.sh" $PROJ_NAME $PROJ_DIR_NAME "$ZF_ROOT_PATH/$PROJ_DIR_NAME/proj/Android/$JAR_PROJ_DIR_NAME_OR_DUMMY"
    sh "$ZF_TOOLS_PATH/spec/Android/jar_copy.sh" $PROJ_NAME $PROJ_DIR_NAME "$ZF_ROOT_PATH/$PROJ_DIR_NAME/proj/Android/$JAR_PROJ_DIR_NAME_OR_DUMMY" "$RELEASE_PATH" >/dev/null 2>&1
fi

if "$LIB_PROJ_DIR_NAME_OR_DUMMY" != "dummy" (
    sh "$ZF_TOOLS_PATH/spec/Android/libs_build.sh" $PROJ_NAME $PROJ_DIR_NAME "$ZF_ROOT_PATH/$PROJ_DIR_NAME/proj/Android/$LIB_PROJ_DIR_NAME_OR_DUMMY"
    sh "$ZF_TOOLS_PATH/spec/Android/libs_copy.sh" $PROJ_NAME $PROJ_DIR_NAME "$ZF_ROOT_PATH/$PROJ_DIR_NAME/proj/Android/$LIB_PROJ_DIR_NAME_OR_DUMMY" "$RELEASE_PATH" >/dev/null 2>&1
)

rm -rf "$RELEASE_PATH/$PROJ_DIR_NAME/assets/res" >/dev/null 2>&1
sh "$ZF_TOOLS_PATH/spec/Android/res_copy.sh" $PROJ_NAME $PROJ_DIR_NAME "$ZF_ROOT_PATH/$PROJ_DIR_NAME/res" "$RELEASE_PATH/$PROJ_DIR_NAME/assets/res" >/dev/null 2>&1
sh "$ZF_TOOLS_PATH/spec/Android/res_fix.sh" "$RELEASE_PATH/$PROJ_DIR_NAME/assets/res" >/dev/null 2>&1
sh "$ZF_TOOLS_PATH/common/copy_header.sh" "$ZF_ROOT_PATH/$PROJ_DIR_NAME/src" "$RELEASE_PATH/$PROJ_DIR_NAME/libs/$PROJ_DIR_NAME/include" >/dev/null 2>&1

