#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$1
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/Android

sh "$ZF_TOOLS_PATH/spec/Android/release.sh" ZFFramework_impl_ZFUIWebKit ZF_impl_ZFUIWebKit ZFFramework_impl_ZFUIWebKit ZFFramework_impl_ZFUIWebKit_jni

