#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

rm -rf "$WORK_DIR/../src/_repo" >/dev/null 2>&1
rm -rf "$WORK_DIR/_repo" >/dev/null 2>&1
rm -rf "$WORK_DIR/_tmp" >/dev/null 2>&1

