/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_NativeEnum_ZFUIWidget.h"
#include "ZFUIWidget/ZFUIDialogForChoice.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIDialogForChoiceMode ZFImpl_sys_Android_JNI_ID_NativeEnum(ZFUIWidget_ZFUIDialogForChoiceMode)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIDialogForChoiceMode ZFImpl_sys_Android_JNI_NAME_NativeEnum(ZFUIWidget.ZFUIDialogForChoiceMode)

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_GLOBAL_END

ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogForChoiceMode, ZFUIDialogForChoiceMode, ChoiceSingle)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogForChoiceMode, ZFUIDialogForChoiceMode, ChoiceSingleQuickly)
ZFImpl_sys_Android_NativeEnum_EXPAND(ZFImpl_sys_Android_JNI_ID_ZFUIDialogForChoiceMode, ZFUIDialogForChoiceMode, ChoiceMultiple)

#endif // #if ZF_ENV_sys_Android

