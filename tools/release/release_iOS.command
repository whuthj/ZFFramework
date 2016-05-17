#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/iOS

rm -rf "$RELEASE_PATH" >/dev/null 2>&1

sh "$ZF_ROOT_PATH/thirdparty_setup.sh"
sh "$ZF_TOOLS_PATH/common/run_recursive.sh" "$ZF_ROOT_PATH" "zfautoscript_release_iOS*.command"

