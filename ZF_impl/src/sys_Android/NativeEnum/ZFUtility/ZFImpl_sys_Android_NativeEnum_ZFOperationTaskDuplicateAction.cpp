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

#define ZFImpl_sys_Android_JNI_ID_ZFOperationTaskDuplicateAction ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUtility_ZFOperationTaskDuplicateAction)
#define ZFImpl_sys_Android_JNI_NAME_ZFOperationTaskDuplicateAction ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUtility.ZFOperationTaskDuplicateAction)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationTaskDuplicateAction, ZFOperationTaskDuplicateAction, Unspecified)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationTaskDuplicateAction, ZFOperationTaskDuplicateAction, Merge)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationTaskDuplicateAction, ZFOperationTaskDuplicateAction, NewTask)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFOperationTaskDuplicateAction, ZFOperationTaskDuplicateAction, Ignore)

#endif // #if ZF_ENV_sys_Android

