/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_NativeEnum_ZFUIWidget.h"
#include "ZFUIWidget/ZFUIDialogBasic.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIDialogButtonType ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIWidget_ZFUIDialogButtonType)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIDialogButtonType ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIWidget.ZFUIDialogButtonType)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogButtonType, ZFUIDialogButtonType, Normal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogButtonType, ZFUIDialogButtonType, Yes)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogButtonType, ZFUIDialogButtonType, No)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogButtonType, ZFUIDialogButtonType, Cancel)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogButtonType, ZFUIDialogButtonType, Destructive)

#endif // #if ZF_ENV_sys_Android

