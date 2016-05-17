#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/git_check.sh" "https://github.com/zeux/pugixml" "$WORK_DIR/_repo/pugixml"
sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$WORK_DIR/_repo/pugixml/src" "$WORK_DIR/../src/_repo/pugixml"

