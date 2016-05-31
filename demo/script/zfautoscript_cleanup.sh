#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/cleanup_proj.sh" "$WORK_DIR/../proj" "ZFFramework_test"

