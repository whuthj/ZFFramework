#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
QT_TYPE=Qt_Posix
RELEASE_PATH=$ZF_ROOT_PATH/_release/$QT_TYPE

sh "$ZF_TOOLS_PATH/spec/Qt/release_Posix.sh" ZFFramework_impl_ZFUIWebKit ZF_impl_ZFUIWebKit

