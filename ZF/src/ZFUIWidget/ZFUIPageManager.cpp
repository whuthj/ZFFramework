/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIPageManager.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// request
ZFOBJECT_REGISTER(ZFUIPageRequest)

zfclass _ZFP_ZFUIPageRequestPageCreate : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIPageRequestPageCreate, ZFUIPageRequest)

public:
    ZFUIPageManager::RequestPageCreateParam createParam;
};

zfclass _ZFP_ZFUIPageRequestPageResume : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIPageRequestPageResume, ZFUIPageRequest)

public:
    ZFPROPERTY_RETAIN(ZFUIPage *, page)
};

zfclass _ZFP_ZFUIPageRequestPageGroupResume : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIPageRequestPageGroupResume, ZFUIPageRequest)

public:
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, pageGroupId, ZFPropertyInitValue(ZFUIPageGroupIdInvalid))
};

zfclass _ZFP_ZFUIPageRequestPageDestroy : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIPageRequestPageDestroy, ZFUIPageRequest)

public:
    ZFPROPERTY_RETAIN(ZFUIPage *, page)
};

// ============================================================
zfclassNotPOD _ZFP_ZFUIPageManagerPrivate
{
public:
    ZFUIPageManager *pageManager;
    ZFCoreArrayPOD<ZFUIPage *> pageList;
    zfbool managerResumed;
    zfindex managerUIEnableFlag;
    zfindex managerBlockRequestFlag;
    ZFCoreQueuePOD<ZFUIPageRequest *> requestQueue;
    zfbool requestRunning;
    ZFListener requestOnResolveListener;
    ZFCoreArrayPOD<ZFUIPage *> pageDelayDestroyList;
    zfbool movePageFlag;

public:
    void requestDoPost(ZF_IN ZFObject *owner)
    {
        zfsynchronizedObject(owner);
        if(this->managerBlockRequestFlag > 0 || this->requestQueue.isEmpty() || this->requestRunning)
        {
            return ;
        }
        this->requestRunning = zftrue;
        ZFThreadTaskRequest(
            this->requestOnResolveListener,
            zfHint("userData")owner,
            zfHint("param0")zfnull,
            zfHint("param1")zfnull,
            zfHint("owner")owner,
            ZFThreadTaskRequestMergeCallbackIgnoreNewTask);
    }
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(requestOnResolve)
    {
        ZFUIPageManager *manager = userData->to<ZFUIPageManager *>();
        zfsynchronizedObject(manager->toObject());
        if(manager->d->managerBlockRequestFlag > 0 || manager->d->requestQueue.isEmpty())
        {
            manager->d->requestRunning = zffalse;
            return ;
        }

        manager->managerBlockRequest(zftrue);
        ZFUIPageRequest *request = manager->d->requestQueue.queueTake();
        manager->requestOnResolve(request);
        manager->managerUIEnableSet(zftrue);
        zfCoreAssertWithMessageTrim(request->requestResolved(), zfTextA("[ZFUIPageManager] unresolved request: %s"), zfsCoreZ2A(request->objectInfo().cString()));
        manager->requestOnResolveFinish(request);
        ZFLeakTestLogReleaseVerbose(request, ZF_CALLER_FILE, zfTextA("requestOnResolve"), ZF_CALLER_LINE);
        zfReleaseWithoutLeakTest(request);
        manager->managerBlockRequest(zffalse);
        manager->d->requestRunning = zffalse;

        if(manager->d->requestQueue.isEmpty())
        {
            ZFUIPage *foregroundPage = manager->pageForeground();
            if(!foregroundPage->pageResumed())
            {
                manager->d->pageResume(foregroundPage, ZFUIPageResumeReason::e_ByManagerResume);
            }
            if(manager->d->requestQueue.isEmpty())
            {
                manager->requestOnResolveFinishAll();
            }
        }
        else
        {
            manager->d->requestDoPost(manager->toObject());
        }
    }

public:
    void pageCreate(ZF_IN ZFUIPage *page, ZF_IN const ZFUIPageManager::RequestPageCreateParam &createParam)
    {
        page->pageCreateParamSet(createParam.pageCreateParam());

        page->_ZFP_ZFUIPage_pageCreate();
        this->pageManager->pageOnCreate(page, createParam);
        page->pageAfterCreate();
        this->pageManager->pageAfterCreate(page);
    }
    void pageResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        page->_ZFP_ZFUIPage_pageResume(reason);
        this->pageManager->pageOnResume(page, reason);
        page->pageAfterResume(reason);
        this->pageManager->pageAfterResume(page, reason);
    }
    void pagePause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->pageManager->pageBeforePause(page, reason);
        page->pageBeforePause(reason);
        this->pageManager->pageOnPause(page, reason);
        page->_ZFP_ZFUIPage_pagePause(reason);
    }
    void pageDestroy(ZF_IN ZFUIPage *page)
    {
        this->pageManager->pageBeforeDestroy(page);
        page->pageBeforeDestroy();
        this->pageManager->pageOnDestroy(page);
        page->_ZFP_ZFUIPage_pageDestroy();
    }

public:
    void pageDelayDestroyCheckFinish(ZF_IN ZFUIPage *page)
    {
        if(page->_ZFP_ZFUIPage_pageDelayDestroyFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag > 0)
        {
            return ;
        }
        zfindex index = this->pageDelayDestroyList.find(page);
        if(index == zfindexMax)
        {
            return ;
        }
        this->pageDelayDestroyList.remove(index);
        this->pageDestroy(page);
        ZFLeakTestLogReleaseVerbose(page, ZF_CALLER_FILE, zfText("pageDestroy"), ZF_CALLER_LINE);
        zfReleaseWithoutLeakTest(page);
    }

public:
    _ZFP_ZFUIPageManagerPrivate(void)
    : pageManager(zfnull)
    , pageList()
    , managerResumed(zffalse)
    , managerUIEnableFlag(0)
    , managerBlockRequestFlag(0)
    , requestQueue()
    , requestRunning(zffalse)
    , requestOnResolveListener()
    , pageDelayDestroyList()
    , movePageFlag(zffalse)
    {
        this->requestOnResolveListener = ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerPrivate::requestOnResolve);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIPageManager)

// ============================================================
// embeded logic
void ZFUIPageManager::embededCreate(void)
{
    zfCoreAssert(d == zfnull);
    d = zfpoolNew(_ZFP_ZFUIPageManagerPrivate);
    d->pageManager = this;
    this->managerOnCreate();
}
void ZFUIPageManager::embededResume(void)
{
    zfCoreAssert(!d->managerResumed);
    d->managerResumed = zftrue;

    this->managerOnResume();

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *foregroundPage = d->pageList.getLast();
        if(!foregroundPage->pageResumed())
        {
            d->pageResume(foregroundPage, ZFUIPageResumeReason::e_ByManagerResume);
        }
    }

    this->managerAfterResume();
}
void ZFUIPageManager::embededPause(void)
{
    zfCoreAssert(d->managerResumed);
    d->managerResumed = zffalse;

    this->managerBeforePause();

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *foregroundPage = d->pageList.getLast();
        if(foregroundPage->pageResumed())
        {
            d->pagePause(foregroundPage, ZFUIPagePauseReason::e_ByManagerPause);
        }
    }
    this->managerOnPause();
}
void ZFUIPageManager::embededDestroy(void)
{
    zfCoreAssert(d != zfnull);
    zfCoreAssert(!d->managerResumed);
    zfCoreAssertWithMessage(d->pageDelayDestroyList.isEmpty(), zfTextA("you must not destroy the manager if there are pages delaying destroy"));

    if(!d->pageList.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIPage *> pageList;
        pageList.copyFrom(d->pageList);
        d->pageList.removeAll();
        for(zfindex i = pageList.count() - 1; i != zfindexMax; --i)
        {
            ZFUIPage *page = pageList[i];
            d->pageDestroy(page);
            zfRelease(page);
        }
    }

    if(!d->requestQueue.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIPageRequest *> tmp;
        while(!d->requestQueue.isEmpty())
        {
            tmp.add(d->requestQueue.queueTake());
        }
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            this->managerUIEnableSet(zftrue);
            zfRelease(tmp[i]);
        }
    }

    this->managerOnDestroy();

    ZFThreadTaskCancelWithOwner(this->toObject());

    zfpoolDelete(d);
}

// ============================================================
// manager control
void ZFUIPageManager::managerUIEnableSet(ZF_IN zfbool value)
{
    if(value)
    {
        --(d->managerUIEnableFlag);
        if(d->managerUIEnableFlag == 0)
        {
            this->managerUIEnableOnChange();
        }
    }
    else
    {
        ++(d->managerUIEnableFlag);
        if(d->managerUIEnableFlag == 1)
        {
            this->managerUIEnableOnChange();
        }
    }
}
zfbool ZFUIPageManager::managerUIEnable(void)
{
    return (d->managerUIEnableFlag == 0);
}

// ============================================================
// page access
zfindex ZFUIPageManager::pageCount(void)
{
    return d->pageList.count();
}
ZFCoreArrayPOD<ZFUIPage *> &ZFUIPageManager::pageList(void)
{
    return d->pageList;
}

// ============================================================
// page request
void ZFUIPageManager::requestPageCreate(ZF_IN const RequestPageCreateParam &createParam)
{
    zfblockedAlloc(_ZFP_ZFUIPageRequestPageCreate, request);
    request->createParam = createParam;
    this->requestPost(request);
}
void ZFUIPageManager::requestPageCreate(ZF_IN const ZFClass *pageClass,
                                        ZF_IN_OPT ZFObject *pageCreateParam /* = zfnull */,
                                        ZF_IN_OPT zfbool pageAutoResume /* = zftrue */)
{
    if(pageClass == zfnull)
    {
        return ;
    }
    zfblockedAlloc(_ZFP_ZFUIPageRequestPageCreate, request);
    request->createParam
        .pageClassSet(pageClass)
        .pageCreateParamSet(pageCreateParam)
        .pageAutoResumeSet(pageAutoResume);
    this->requestPost(request);
}
void ZFUIPageManager::requestPageResume(ZF_IN ZFUIPage *page)
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(_ZFP_ZFUIPageRequestPageResume, request);
    request->pageSet(page);
    this->requestPost(request);
}
void ZFUIPageManager::requestPageGroupResume(ZF_IN const zfchar *pageGroupId)
{
    zfblockedAlloc(_ZFP_ZFUIPageRequestPageGroupResume, request);
    request->pageGroupIdSet(zfstring(pageGroupId));
    this->requestPost(request);
}
void ZFUIPageManager::requestPageDestroy(ZF_IN ZFUIPage *page)
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(_ZFP_ZFUIPageRequestPageDestroy, request);
    request->pageSet(page);
    this->requestPost(request);
}
void ZFUIPageManager::requestPost(ZF_IN ZFUIPageRequest *request)
{
    if(request != zfnull)
    {
        zfsynchronizedObject(this->toObject());
        this->managerUIEnableSet(zffalse);
        d->requestQueue.queuePut(zfRetain(request));
        d->requestDoPost(this->toObject());
    }
}
void ZFUIPageManager::managerBlockRequest(ZF_IN zfbool blockRequest)
{
    zfsynchronizedObjectLock(this->toObject());
    if(blockRequest)
    {
        ++(d->managerBlockRequestFlag);
        if(d->managerBlockRequestFlag == 1)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->managerBlockRequestOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
    else
    {
        --(d->managerBlockRequestFlag);
        if(d->managerBlockRequestFlag == 0)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->managerBlockRequestOnChange();
            d->requestDoPost(this->toObject());
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
}

// ============================================================
// request resolve
void ZFUIPageManager::requestOnResolve(ZF_IN ZFUIPageRequest *request)
{
    const ZFClass *cls = request->classData();
    if(cls->classIsSubclassOf(_ZFP_ZFUIPageRequestPageResume::ClassData()))
    {
        this->requestOnResolvePageResume(request->to<_ZFP_ZFUIPageRequestPageResume *>()->page());
        request->requestResolvedSet(zftrue);
    }
    else if(cls->classIsSubclassOf(_ZFP_ZFUIPageRequestPageCreate::ClassData()))
    {
        this->requestOnResolvePageCreate(request->to<_ZFP_ZFUIPageRequestPageCreate *>()->createParam);
        request->requestResolvedSet(zftrue);
    }
    else if(cls->classIsSubclassOf(_ZFP_ZFUIPageRequestPageDestroy::ClassData()))
    {
        this->requestOnResolvePageDestroy(request->to<_ZFP_ZFUIPageRequestPageDestroy *>()->page());
        request->requestResolvedSet(zftrue);
    }
    else if(cls->classIsSubclassOf(_ZFP_ZFUIPageRequestPageGroupResume::ClassData()))
    {
        this->requestOnResolvePageGroupResume(request->to<_ZFP_ZFUIPageRequestPageGroupResume *>()->pageGroupId());
        request->requestResolvedSet(zftrue);
    }
}
void ZFUIPageManager::requestOnResolvePageCreate(ZF_IN const ZFUIPageManager::RequestPageCreateParam &createParam)
{
    this->resolvePageCreate(createParam);
}
void ZFUIPageManager::resolvePageCreate(ZF_IN const ZFUIPageManager::RequestPageCreateParam &createParam)
{
    zfCoreAssertWithMessageTrim(createParam.pageClass() != zfnull && createParam.pageClass()->classIsTypeOf(ZFUIPage::ClassData()),
        zfTextA("[ZFUIPageManager] pageClass %s not type of %s"),
        zfsCoreZ2A((createParam.pageClass() == zfnull) ? ZFTOKEN_zfnull : createParam.pageClass()->objectInfo().cString()),
        zfsCoreZ2A(ZFUIPage::ClassData()->objectInfo().cString()));

    zfautoObject pageObj = createParam.pageClass()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    ZFUIPage *page = pageObj.to<ZFUIPage *>();
    page->_ZFP_ZFUIPage_pageManager = this;
    zfRetain(page);
    d->pageList.add(0, page);

    d->pageCreate(page, createParam);

    if(createParam.pageAutoResume())
    {
        this->requestPageResume(page);
    }
}
void ZFUIPageManager::requestOnResolvePageResume(ZF_IN ZFUIPage *page)
{
    this->resolvePageResume(page);
}
void ZFUIPageManager::resolvePageResume(ZF_IN ZFUIPage *page)
{
    zfindex index = d->pageList.find(page);
    if(index == zfindexMax || page->pageResumed())
    {
        return ;
    }
    if(index != d->pageList.count() - 1)
    {
        d->pageList.remove(index);
        d->pageList.add(page);
    }
    if(d->pageList.count() >= 2)
    {
        ZFUIPage *old = d->pageList[d->pageList.count() - 2];
        if(old->pageResumed())
        {
            d->pagePause(old, ZFUIPagePauseReason::e_ToBackground);
        }
    }
    d->pageResume(page, ZFUIPageResumeReason::e_ByRequest);
}
void ZFUIPageManager::requestOnResolvePageGroupResume(ZF_IN const zfchar *pageGroupId)
{
    this->resolvePageGroupResume(pageGroupId);
}
void ZFUIPageManager::resolvePageGroupResume(ZF_IN const zfchar *pageGroupId)
{
    this->movePageBegin();
    ZFCoreArrayPOD<ZFUIPage *> &pageList = this->pageList();
    ZFCoreArrayPOD<ZFUIPage *> pageListTmp;
    for(zfindex i = 0; i < pageList.count(); ++i)
    {
        ZFUIPage *page = pageList[i];
        if(page->pageGroupId().compare(pageGroupId) == 0)
        {
            pageList.remove(i);
            --i;
            pageListTmp.add(page);
        }
    }
    pageList.addFrom(pageListTmp);
    this->movePageEnd();
}
void ZFUIPageManager::requestOnResolvePageDestroy(ZF_IN ZFUIPage *page)
{
    this->resolvePageDestroy(page);
}
void ZFUIPageManager::resolvePageDestroy(ZF_IN ZFUIPage *page)
{
    zfindex index = d->pageList.find(page);
    if(index == zfindexMax)
    {
        return ;
    }
    if(index == d->pageList.count() - 1)
    {
        d->pageList.remove(index);
        if(page->pageResumed())
        {
            d->pagePause(page, ZFUIPagePauseReason::e_BeforeDestroy);
        }

        if(!d->pageList.isEmpty())
        {
            ZFUIPage *foregroundPage = d->pageList.getLast();
            if(!foregroundPage->pageResumed())
            {
                d->pageResume(foregroundPage, ZFUIPageResumeReason::e_FromBackground);
            }
        }

        page->_ZFP_ZFUIPage_pageDelayDestroyOverrideFlag = zftrue;
        page->pageDelayDestroyOnCheck();
        this->pageDelayDestroyOnCheck(page);
        page->_ZFP_ZFUIPage_pageDelayDestroyOverrideFlag = zffalse;
        if(page->_ZFP_ZFUIPage_pageDelayDestroyFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag > 0)
        {
            d->pageDelayDestroyList.add(page);
        }
        else
        {
            d->pageDestroy(page);
            zfRelease(page);
        }
    }
    else
    {
        d->pageList.remove(index);

        if(!d->pageList.isEmpty())
        {
            ZFUIPage *foregroundPage = d->pageList.getLast();
            if(!foregroundPage->pageResumed())
            {
                d->pageResume(foregroundPage, ZFUIPageResumeReason::e_FromBackground);
            }
        }

        d->pageDestroy(page);
        zfRelease(page);
    }
}

// ============================================================
// pageDelayDestroy
void ZFUIPageManager::pageDelayDestroyOnCheck(ZF_IN ZFUIPage *page)
{
    // nothing to do
}
void ZFUIPageManager::pageDelayDestroyEnable(ZF_IN ZFUIPage *page)
{
    zfCoreAssertWithMessage(page->_ZFP_ZFUIPage_pageDelayDestroyOverrideFlag, zfTextA("pageDelayDestroyEnable can only be called during pageDelayDestroyOnCheck"));
    ++(page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag);
}
void ZFUIPageManager::pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page)
{
    --(page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag);
    d->pageDelayDestroyCheckFinish(page);
}
void ZFUIPageManager::_ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page)
{
    d->pageDelayDestroyCheckFinish(page);
}

// ============================================================
// page move
void ZFUIPageManager::movePageBegin(void)
{
    zfCoreAssertWithMessageTrim(!d->movePageFlag, zfTextA("[ZFUIPageManager] move page not paired, have you forgot movePageEnd?"));
    d->movePageFlag = zftrue;
}
void ZFUIPageManager::movePageEnd(void)
{
    zfCoreAssertWithMessageTrim(d->movePageFlag, zfTextA("[ZFUIPageManager] move page not paired, have you forgot movePageBegin?"));
    d->movePageFlag = zffalse;

    for(zfindex i = d->pageList.count() - 1; i != zfindexMax; --i)
    {
        ZFUIPage *page = d->pageList[i];
        if(page->pageResumed())
        {
            if(i != d->pageList.count() - 1)
            {
                d->pagePause(page, ZFUIPagePauseReason::e_ToBackground);
            }
            break;
        }
    }
    if(!d->pageList.isEmpty())
    {
        ZFUIPage *page = d->pageList.getLast();
        if(!page->pageResumed())
        {
            d->pageResume(page, ZFUIPageResumeReason::e_ByRequest);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

