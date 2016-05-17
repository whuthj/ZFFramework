#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/iOS

sh "$ZF_TOOLS_PATH/spec/iOS/release.command" ZFFramework ZF

