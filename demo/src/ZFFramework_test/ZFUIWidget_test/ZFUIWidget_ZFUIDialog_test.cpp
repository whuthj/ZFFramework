/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIDialog_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialog_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogBasic *, dialog, ZFPropertyInitValue(zflineAlloc(ZFUIDialogBasic)))

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButton_Cancel());
        this->dialog()->dialogButton_Cancel()->to<ZFUIButtonBasic *>()->buttonLabelTextStringSet(zfText("cancel"));
        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButton_Yes());
        this->dialog()->dialogButton_Yes()->to<ZFUIButtonBasic *>()->buttonLabelTextStringSet(zfText("confirm"));
        ZFLISTENER_LOCAL(yesOnClick, {
            zfLogT() << zfText("onConfirm") << listenerData.sender;
        })
        this->dialog()->dialogButton_Yes()->observerAdd(ZFUIButton::EventButtonOnClick(), yesOnClick);
        this->dialog()->dialogTitleTextSet(zfText("i'm title"));
        this->dialog()->dialogContentTextSet(zfText("i'm content"));

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialog *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialog *dialog)
    {
        zfblockedAlloc(ZFArrayEditable, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, dialog, ZFPropertyAccess(ZFUIDialog, dialogHideWhenTouchOutside));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, dialog, ZFPropertyAccess(ZFUIDialog, dialogHideWhenClickBack));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialog_test)

// ============================================================
#if 1
ZF_GLOBAL_INITIALIZER_INIT(ZFUIWidget_ZFUIDialog_debug_LogEvent)
{
    ZFLISTENER_LOCAL(dialogBeforeShow, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender
            << zfText("BeforeShow");
    })
    this->dialogBeforeShowListener = dialogBeforeShow;
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIDialog::EventDialogBeforeShow(), this->dialogBeforeShowListener);

    ZFLISTENER_LOCAL(dialogAfterShow, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender
            << zfText("AfterShow");
    })
    this->dialogAfterShowListener = dialogAfterShow;
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIDialog::EventDialogAfterShow(), this->dialogAfterShowListener);

    ZFLISTENER_LOCAL(dialogBeforeHide, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender
            << zfText("BeforeHide");
    })
    this->dialogBeforeHideListener = dialogBeforeHide;
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIDialog::EventDialogBeforeHide(), this->dialogBeforeHideListener);

    ZFLISTENER_LOCAL(dialogAfterHide, {
        zfLogTrimT()
            << zfLogCurTimeString()
            << listenerData.sender
            << zfText("AfterHide");
    })
    this->dialogAfterHideListener = dialogAfterHide;
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIDialog::EventDialogAfterHide(), this->dialogAfterHideListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIWidget_ZFUIDialog_debug_LogEvent)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIDialog::EventDialogBeforeShow(), this->dialogBeforeShowListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIDialog::EventDialogAfterShow(), this->dialogAfterShowListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIDialog::EventDialogBeforeHide(), this->dialogBeforeHideListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIDialog::EventDialogAfterHide(), this->dialogAfterHideListener);
}
private:
ZFListener dialogBeforeShowListener;
ZFListener dialogAfterShowListener;
ZFListener dialogBeforeHideListener;
ZFListener dialogAfterHideListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIWidget_ZFUIDialog_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END

