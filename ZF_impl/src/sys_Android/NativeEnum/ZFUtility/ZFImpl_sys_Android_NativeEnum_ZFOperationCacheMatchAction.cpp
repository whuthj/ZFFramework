/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_NativeEnum_ZFUtility.h"
#include "ZFUtility/ZFOperation.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFOperationCacheMatchAction ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUtility_ZFOperationCacheMatchAction)
#define ZFImpl_sys_Android_JNI_NAME_ZFOperationCacheMatchAction ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUtility.ZFOperationCacheMatchAction)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationCacheMatchAction, ZFOperationCacheMatchAction, Unspecified)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationCacheMatchAction, ZFOperationCacheMatchAction, NotifyFinish)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationCacheMatchAction, ZFOperationCacheMatchAction, NotifyFinishThenRemove)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationCacheMatchAction, ZFOperationCacheMatchAction, StartNew)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationCacheMatchAction, ZFOperationCacheMatchAction, StartNewThenRemove)

#endif // #if ZF_ENV_sys_Android

