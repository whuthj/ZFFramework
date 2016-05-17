/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIWidget.hh"
#include "ZFUIHint_Basic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// hint with simple text and icon
ZFSTYLE_DEFAULT_DEFINE(ZFUIHintContentBasicStyle, ZFUIButtonBasicStyle)

zfautoObject ZFUIHintMake(ZF_IN const zfchar *text,
                          ZF_IN_OPT ZFUIImage *icon /* = zfnull */)
{
    zfblockedAllocWithLeakTest(ZFUIHint, hint);

    zfblockedAllocWithLeakTest(ZFAnimationNativeView, hintAniShow);
    hint->hintAniShowSet(hintAniShow);
    hintAniShow->aniAlphaFromSet(0);
    zfblockedAllocWithLeakTest(ZFAnimationNativeView, hintAniHide);
    hint->hintAniHideSet(hintAniHide);
    hintAniHide->aniAlphaToSet(0);

    zfblockedAllocWithLeakTest(ZFUIHintContentBasic, hintContent);
    hint->hintContentSet(hintContent);
    hintContent->buttonLabelStyleNormal()->textContentStringSet(text);
    hintContent->buttonIconStyleNormal()->imageContentSet(icon);

    return zfautoObjectCreateWithLeakTest(hint);
}

ZF_NAMESPACE_GLOBAL_END

