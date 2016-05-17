#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

rm -rf "$ZF_ROOT_PATH/_release" >/dev/null 2>&1
rm -rf "$ZF_ROOT_PATH/_tmp" >/dev/null 2>&1

sh "$ZF_TOOLS_PATH/common/run_recursive.sh" "$ZF_ROOT_PATH" "zfautoscript_cleanup*.sh"

