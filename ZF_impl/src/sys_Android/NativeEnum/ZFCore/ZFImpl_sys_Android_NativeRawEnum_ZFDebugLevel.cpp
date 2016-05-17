/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_NativeEnum_ZFCore.h"
#include "ZFCore/ZFDebug.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFDebugLevel ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFDebugLevel)
#define ZFImpl_sys_Android_JNI_NAME_ZFDebugLevel ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFDebugLevel)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevelVerbose)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevelDebug)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevelInfo)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevelWarning)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevelError)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFDebugLevel, ZFDebugLevelAssert)

#endif // #if ZF_ENV_sys_Android

