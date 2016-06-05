/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIDialogForInput.h"
#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogForInputStyle, ZFUIDialogStyle)

ZFObject *ZFUIDialogForInput::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfblockedAlloc(ZFUILinearLayout, mainContainer);
    this->dialogContentContainer()->childAdd(mainContainer);
    mainContainer->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);
    mainContainer->layoutOrientationSet(ZFUIOrientation::e_Top);

    ZFUIView *inputHintView = this->inputHintView()->to<ZFUIView *>();
    mainContainer->childAdd(inputHintView);
    inputHintView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);

    ZFUIView *inputView = this->inputView()->to<ZFUIView *>();
    mainContainer->childAdd(inputView);
    inputView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthWrapHeight);

    return this;
}
void ZFUIDialogForInput::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

