#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/git_check.sh" "https://github.com/ZSaberLv0/stack_trace" "$WORK_DIR/_repo/stack_trace"

sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$WORK_DIR/_repo/stack_trace/include" "$WORK_DIR/_tmp/stack_trace"
sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$WORK_DIR/_repo/stack_trace/src" "$WORK_DIR/_tmp/stack_trace"

sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$WORK_DIR/zfautoscript_thirdparty_stack_trace_template.hpp" "$WORK_DIR/../src/_repo/stack_trace/stack.hpp"
sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$WORK_DIR/zfautoscript_thirdparty_stack_trace_template.cpp" "$WORK_DIR/../src/_repo/stack_trace/stack.cpp"

