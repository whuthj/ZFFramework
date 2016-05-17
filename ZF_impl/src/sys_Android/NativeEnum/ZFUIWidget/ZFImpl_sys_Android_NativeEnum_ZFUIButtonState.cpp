/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_NativeEnum_ZFUIWidget.h"
#include "ZFUIWidget/ZFUIButton.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIButtonState ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIWidget_ZFUIButtonState)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIButtonState ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIWidget.ZFUIButtonState)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIButtonState, ZFUIButtonState, Normal)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIButtonState, ZFUIButtonState, Highlighted)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIButtonState, ZFUIButtonState, Checked)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIButtonState, ZFUIButtonState, CheckedHighlighted)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIButtonState, ZFUIButtonState, Disabled)

#endif // #if ZF_ENV_sys_Android

