/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIPage.h"
#include "ZFUIPageManager.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIPage)

void ZFUIPage::pageDelayDestroyOnCheck(void)
{
    // nothing to do
}
void ZFUIPage::pageDelayDestroyEnable(void)
{
    zfCoreAssertWithMessage(this->_ZFP_ZFUIPage_pageDelayDestroyOverrideFlag, zfTextA("pageDelayDestroyEnable can only be called during pageDelayDestroyOnCheck"));
    ++(this->_ZFP_ZFUIPage_pageDelayDestroyFlag);
}
void ZFUIPage::pageDelayDestroyNotifyFinish(void)
{
    --(this->_ZFP_ZFUIPage_pageDelayDestroyFlag);
    if(this->_ZFP_ZFUIPage_pageDelayDestroyFlag == 0)
    {
        this->pageManager()->_ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(this);
    }
}

void ZFUIPage::_ZFP_ZFUIPage_pageCreate(void)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageCreated);
    this->_ZFP_ZFUIPage_pageOnCreateCalled = zffalse;
    this->pageOnCreate();
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnCreateCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnCreate"));
    this->_ZFP_ZFUIPage_pageCreated = zftrue;
}
void ZFUIPage::_ZFP_ZFUIPage_pageResume(ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageResumed);
    this->_ZFP_ZFUIPage_pageOnResumeCalled = zffalse;
    this->pageOnResume(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnResumeCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnResume"));
    this->_ZFP_ZFUIPage_pageResumed = zftrue;
}
void ZFUIPage::_ZFP_ZFUIPage_pagePause(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageResumed);
    this->_ZFP_ZFUIPage_pageOnPauseCalled = zffalse;
    this->pageOnPause(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnPauseCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnPause"));
    this->_ZFP_ZFUIPage_pageResumed = zffalse;
}
void ZFUIPage::_ZFP_ZFUIPage_pageDestroy(void)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageCreated);
    this->_ZFP_ZFUIPage_pageOnDestroyCalled = zffalse;
    this->pageOnDestroy();
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnDestroyCalled, zfTextA("[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnDestroy"));
    this->_ZFP_ZFUIPage_pageCreated = zffalse;
    ZFPropertyChange(this->_ZFP_ZFUIPage_pageCreateParam, zfnull);
}

ZF_NAMESPACE_GLOBAL_END

