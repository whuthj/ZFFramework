/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_NativeEnum_ZFCore.h"
#include "ZFCore/ZFLog.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFLogLevel ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFCore_ZFLogLevel)
#define ZFImpl_sys_Android_JNI_NAME_ZFLogLevel ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFCore.ZFLogLevel)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFLogLevel, ZFLogLevelVerbose)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFLogLevel, ZFLogLevelDebug)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFLogLevel, ZFLogLevelInfo)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFLogLevel, ZFLogLevelWarning)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFLogLevel, ZFLogLevelError)
ZFImpl_sys_Android_NativeRawEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFLogLevel, ZFLogLevelAssert)

#endif // #if ZF_ENV_sys_Android

