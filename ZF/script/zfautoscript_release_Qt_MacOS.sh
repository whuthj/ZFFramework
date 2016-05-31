#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
QT_TYPE=Qt_MacOS
RELEASE_PATH=$ZF_ROOT_PATH/_release/$QT_TYPE

sh "$ZF_TOOLS_PATH/spec/Qt/release_MacOS.sh" ZFFramework ZF

