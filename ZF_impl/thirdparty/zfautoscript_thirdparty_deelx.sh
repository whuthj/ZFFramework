#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/git_check.sh" "https://github.com/sswater/deelx" "$WORK_DIR/_repo/deelx"
sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$WORK_DIR/_repo/deelx/deelx.h" "$WORK_DIR/../src/_repo/deelx/deelx.h"

