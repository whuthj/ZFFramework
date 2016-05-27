/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIPageManagerBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIPageManagerBasicPrivate
{
public:
    ZFUIPageManagerBasic *pageManager;

    zfbool pageAniOverrideForOnce;
    ZFAnimation *pageAniOverrideForOnceResumeAni;
    ZFAnimation *pageAniOverrideForOncePauseAni;
    zfbool pageAniOverrideForOncePauseAniHasHigherPriority;

    ZFUIPageBasic *pageLastResume;

    ZFAnimation *pageAniLastResume;
    ZFAnimation *pageAniLastPause;

    ZFListener pageAniOnStopOrOnInvalidListener;
    ZFListener pageAniOnStartListener;
    ZFListener pageAniOnStopListener;

private:
    static ZFLISTENER_PROTOTYPE_EXPAND(pageAniOnStopOrOnInvalid)
    {
        ZFUIPageBasic *page = userData->tagGet<ZFUIPageBasic *>(zfText("page"));
        ZFEnum *reason = userData->tagGet<ZFEnum *>(zfText("reason"));
        ZFUIPageManagerBasic *pageManager = page->pageManager<ZFUIPageManagerBasic *>();

        zfRetain(userData);
        zfblockedRelease(userData);
        page->pageManager<ZFUIPageManagerBasic *>()->d->pageAniListenerDetach(page);

        if(reason->classData()->classIsSubclassOf(ZFUIPageResumeReason::ClassData()))
        {
            pageManager->d->pageAniLastResume = zfnull;
        }
        else if(reason->classData()->classIsSubclassOf(ZFUIPagePauseReason::ClassData()))
        {
            pageManager->d->pageAniLastPause = zfnull;
        }

        page->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
        page->pageAniSet(zfnull);
        page->_ZFP_ZFUIPage_pageAniCanChange = zffalse;

        if(reason->classData()->classIsSubclassOf(ZFUIPagePauseReason::ClassData()))
        {
            ZFUIPagePauseReasonEnum pauseReasonTmp = reason->enumValue<ZFUIPagePauseReasonEnum>();
            pageManager->pageOnDetach(page, pauseReasonTmp);

            if(pauseReasonTmp == ZFUIPagePauseReason::e_BeforeDestroy
                && zfHint("aniValid") listenerData.param0->to<ZFValue *>()->boolValue())
            {
                page->pageManager<ZFUIPageManagerBasic *>()->pageDelayDestroyNotifyFinish(page);
            }
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(pageAniOnStart)
    {
        ZFUIPageBasic *page = userData->tagGet<ZFUIPageBasic *>(zfText("page"));
        ZFEnum *reason = userData->tagGet<ZFEnum *>(zfText("reason"));
        ZFUIPageManagerBasic *pageManager = page->pageManager<ZFUIPageManagerBasic *>();

        page->pageManager()->managerUIEnableSet(zffalse);
        ZFAnimation *pageAni = listenerData.sender->to<ZFAnimation *>();
        page->pageAniOnStart(pageAni, reason);
        pageManager->pageAniOnStart(page, pageAni, reason);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(pageAniOnStop)
    {
        ZFUIPageBasic *page = userData->tagGet<ZFUIPageBasic *>(zfText("page"));
        ZFEnum *reason = userData->tagGet<ZFEnum *>(zfText("reason"));
        ZFUIPageManagerBasic *pageManager = page->pageManager<ZFUIPageManagerBasic *>();

        ZFAnimation *pageAni = listenerData.sender->to<ZFAnimation *>();
        page->pageAniOnStop(pageAni, reason);
        pageManager->pageAniOnStop(page, pageAni, reason);

        page->pageManager()->managerUIEnableSet(zftrue);
    }

public:
    void pageAniUpdate(ZF_IN ZFUIPageBasic *resumePageOrNull,
                       ZF_IN ZFUIPageBasic *pausePageOrNull,
                       ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        // cleanup old animation
        if(this->pageAniLastResume != zfnull)
        {
            this->pageAniLastResume->aniStop();
            ZFPropertyChange(this->pageAniLastResume, zfnull);
        }
        if(this->pageAniLastPause != zfnull)
        {
            this->pageAniLastPause->aniStop();
            ZFPropertyChange(this->pageAniLastPause, zfnull);
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAni() != zfnull)
        {
            resumePageOrNull->pageAni()->aniStop();
        }
        if(pausePageOrNull != zfnull && pausePageOrNull->pageAni() != zfnull)
        {
            pausePageOrNull->pageAni()->aniStop();
        }

        // update new animation
        if(resumePageOrNull != zfnull)
        {
            resumePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
        }
        if(pausePageOrNull != zfnull)
        {
            pausePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
        }
        this->pageManager->pageAniOnUpdate(resumePageOrNull, pausePageOrNull, reason);

        // finally start new animation
        zfbool pausePageHasHigherPriority = zffalse;
        if(pausePageOrNull != zfnull && pausePageOrNull->pageAniPriorityNeedHigher)
        {
            pausePageHasHigherPriority = zftrue;
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAniPriorityNeedHigher)
        {
            pausePageHasHigherPriority = zffalse;
        }
        if(pausePageOrNull != zfnull && pausePageOrNull->pageAni() != zfnull)
        {
            pausePageOrNull->pageAniOnUpdateAniTarget(pausePageOrNull->pageAni());
            pausePageOrNull->pageAniPriorityOnUpdate(pausePageHasHigherPriority);
            this->pageAniLastPause = pausePageOrNull->pageAni();
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAni() != zfnull)
        {
            resumePageOrNull->pageAniOnUpdateAniTarget(resumePageOrNull->pageAni());
            resumePageOrNull->pageAniPriorityOnUpdate(!pausePageHasHigherPriority);
            this->pageAniLastResume = resumePageOrNull->pageAni();
        }
        if(resumePageOrNull != zfnull)
        {
            resumePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zffalse;
        }
        if(pausePageOrNull != zfnull)
        {
            pausePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zffalse;
        }

        if(pausePageOrNull != zfnull)
        {
            ZFUIPagePauseReasonEnum pauseReason = ZFUIPagePauseReason::e_ByManagerPause;
            switch(reason)
            {
                case ZFUIPageResumeReason::e_ByManagerResume:
                    pauseReason = ZFUIPagePauseReason::e_ByManagerPause;
                    break;
                case ZFUIPageResumeReason::e_ByRequest:
                    pauseReason = ZFUIPagePauseReason::e_ToBackground;
                    break;
                case ZFUIPageResumeReason::e_FromBackground:
                    pauseReason = ZFUIPagePauseReason::e_BeforeDestroy;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
            if(pausePageOrNull->pageAni() != zfnull)
            {
                this->pageAniListenerAttach(pausePageOrNull, zflineAlloc(ZFUIPagePauseReason, pauseReason));
                pausePageOrNull->pageAni()->aniStart();
            }
            else
            {
                pageManager->pageOnDetach(pausePageOrNull, pauseReason);
            }
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAni() != zfnull)
        {
            this->pageAniListenerAttach(resumePageOrNull, zflineAlloc(ZFUIPageResumeReason, reason));
            resumePageOrNull->pageAni()->aniStart();
        }
    }
    void pageAniListenerAttach(ZF_IN ZFUIPageBasic *page,
                               ZF_IN ZFEnum *reason)
    {
        zfblockedAlloc(ZFObject, userData);
        userData->tagSet(zfText("page"), page);
        userData->tagSet(zfText("reason"), reason);
        page->pageAni()->observerAdd(ZFAnimation::EventAniOnStopOrOnInvalid(), this->pageAniOnStopOrOnInvalidListener, userData);
        page->pageAni()->observerAdd(ZFAnimation::EventAniOnStart(), this->pageAniOnStartListener, userData);
        page->pageAni()->observerAdd(ZFAnimation::EventAniOnStop(), this->pageAniOnStopListener, userData);
    }
    void pageAniListenerDetach(ZF_IN ZFUIPageBasic *page)
    {
        page->pageAni()->observerRemove(ZFAnimation::EventAniOnStopOrOnInvalid(), this->pageAniOnStopOrOnInvalidListener);
        page->pageAni()->observerRemove(ZFAnimation::EventAniOnStart(), this->pageAniOnStartListener);
        page->pageAni()->observerRemove(ZFAnimation::EventAniOnStop(), this->pageAniOnStopListener);
    }

public:
    _ZFP_ZFUIPageManagerBasicPrivate(void)
    : pageManager(zfnull)
    , pageAniOverrideForOnce(zffalse)
    , pageAniOverrideForOnceResumeAni(zfnull)
    , pageAniOverrideForOncePauseAni(zfnull)
    , pageAniOverrideForOncePauseAniHasHigherPriority(zffalse)
    , pageLastResume(zfnull)
    , pageAniLastResume(zfnull)
    , pageAniLastPause(zfnull)
    , pageAniOnStopOrOnInvalidListener(ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasicPrivate::pageAniOnStopOrOnInvalid))
    , pageAniOnStartListener(ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasicPrivate::pageAniOnStart))
    , pageAniOnStopListener(ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerBasicPrivate::pageAniOnStop))
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIPageManagerBasic)

// ============================================================
// animation
void ZFUIPageManagerBasic::pageAniOverrideForOnce(ZF_IN ZFAnimation *pageAniResume,
                                                  ZF_IN ZFAnimation *pageAniPause,
                                                  ZF_IN_OPT zfbool pageAniPauseHasHigherPriority /* = zffalse */)
{
    d->pageAniOverrideForOnce = zftrue;
    ZFPropertyChange(d->pageAniOverrideForOnceResumeAni, pageAniResume);
    ZFPropertyChange(d->pageAniOverrideForOncePauseAni, pageAniPause);
    d->pageAniOverrideForOncePauseAniHasHigherPriority = pageAniPauseHasHigherPriority;
}
void ZFUIPageManagerBasic::pageAniOverrideForOnceCancel(void)
{
    d->pageAniOverrideForOnce = zffalse;
    ZFPropertyChange(d->pageAniOverrideForOnceResumeAni, zfnull);
    ZFPropertyChange(d->pageAniOverrideForOncePauseAni, zfnull);
}
zfbool ZFUIPageManagerBasic::pageAniOverrideForOnceCheckUpdate(ZF_IN ZFUIPageBasic *resumePageOrNull,
                                                               ZF_IN ZFUIPageBasic *siblingPausePageOrNull,
                                                               ZF_IN ZFUIPageResumeReasonEnum reason)
{
    if(!d->pageAniOverrideForOnce)
    {
        return zffalse;
    }
    d->pageAniOverrideForOnce = zffalse;
    if(resumePageOrNull != zfnull)
    {
        resumePageOrNull->pageAniSet(d->pageAniOverrideForOnceResumeAni);
        resumePageOrNull->pageAniPriorityNeedHigher = zffalse;
    }
    if(siblingPausePageOrNull != zfnull)
    {
        siblingPausePageOrNull->pageAniSet(d->pageAniOverrideForOncePauseAni);
        siblingPausePageOrNull->pageAniPriorityNeedHigher = d->pageAniOverrideForOncePauseAniHasHigherPriority;
    }
    ZFPropertyChange(d->pageAniOverrideForOnceResumeAni, zfnull);
    ZFPropertyChange(d->pageAniOverrideForOncePauseAni, zfnull);
    return zftrue;
}
void ZFUIPageManagerBasic::pageAniOnUpdate(ZF_IN ZFUIPageBasic *resumePageOrNull,
                                           ZF_IN ZFUIPageBasic *siblingPausePageOrNull,
                                           ZF_IN ZFUIPageResumeReasonEnum reason)
{
    if(this->pageAniOverrideForOnceCheckUpdate(resumePageOrNull, siblingPausePageOrNull, reason))
    {
        return ;
    }
    if(siblingPausePageOrNull != zfnull)
    {
        siblingPausePageOrNull->pageAniOnUpdateForSiblingPageResume(reason, resumePageOrNull);
    }
    if(resumePageOrNull != zfnull)
    {
        resumePageOrNull->pageAniOnUpdateForResume(reason, siblingPausePageOrNull);
    }
}

// ============================================================
// pageDelayDestroy
void ZFUIPageManagerBasic::pageDelayDestroyOnCheck(ZF_IN ZFUIPage *page)
{
    zfsuperI(ZFUIPageManager)::pageDelayDestroyOnCheck(page);
    ZFUIPageBasic *pageTmp = page->to<ZFUIPageBasic *>();
    if(pageTmp->pageAni() != zfnull && pageTmp->pageAni()->aniRunning())
    {
        this->pageDelayDestroyEnable(page);
    }
}

// ============================================================
// page manager life cycle
void ZFUIPageManagerBasic::managerOnCreate(void)
{
    zfsuperI(ZFUIPageManager)::managerOnCreate();
    d = zfpoolNew(_ZFP_ZFUIPageManagerBasicPrivate);
    d->pageManager = this;
}
void ZFUIPageManagerBasic::managerOnResume(void)
{
    zfsuperI(ZFUIPageManager)::managerOnResume();
    ZFUIPageBasic *foregroundPage = this->pageForeground<ZFUIPageBasic *>();
    if(foregroundPage != zfnull)
    {
        d->pageAniUpdate(foregroundPage, zfnull, ZFUIPageResumeReason::e_ByManagerResume);
    }
}
void ZFUIPageManagerBasic::managerOnPause(void)
{
    zfsuperI(ZFUIPageManager)::managerOnPause();
}
void ZFUIPageManagerBasic::managerOnDestroy(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuperI(ZFUIPageManager)::managerOnDestroy();
}

// ============================================================
// page life cycle
void ZFUIPageManagerBasic::pageOnCreate(ZF_IN ZFUIPage *page,
                                        ZF_IN const ZFUIPageManager::RequestPageCreateParam &createParam)
{
    zfsuperI(ZFUIPageManager)::pageOnCreate(page, createParam);
}
void ZFUIPageManagerBasic::pageOnResume(ZF_IN ZFUIPage *page,
                                        ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfsuperI(ZFUIPageManager)::pageOnResume(page, reason);
}
void ZFUIPageManagerBasic::pageOnAttach(ZF_IN ZFUIPage *page,
                                        ZF_IN ZFUIPageResumeReasonEnum reason)
{
    this->observerNotifyWithCustomSender(page->toObject(), ZFUIPageManagerBasic::EventPageOnAttach(), zflineAllocWithoutLeakTest(ZFUIPageResumeReason, reason));
}
void ZFUIPageManagerBasic::pageOnPause(ZF_IN ZFUIPage *page,
                                       ZF_IN ZFUIPagePauseReasonEnum reason)
{
    zfsuperI(ZFUIPageManager)::pageOnPause(page, reason);
}
void ZFUIPageManagerBasic::pageOnDetach(ZF_IN ZFUIPage *page,
                                        ZF_IN ZFUIPagePauseReasonEnum reason)
{
    this->observerNotifyWithCustomSender(page->toObject(), ZFUIPageManagerBasic::EventPageOnDetach(), zflineAllocWithoutLeakTest(ZFUIPagePauseReason, reason));
}
void ZFUIPageManagerBasic::pageOnDestroy(ZF_IN ZFUIPage *page)
{
    zfsuperI(ZFUIPageManager)::pageOnDestroy(page);
}

void ZFUIPageManagerBasic::pageAfterResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfsuperI(ZFUIPageManager)::pageAfterResume(page, reason);
    this->pageOnAttach(page, reason);

    ZFUIPageBasic *pageTmp = page->toAny();
    d->pageAniUpdate(pageTmp, (d->pageLastResume == pageTmp) ? zfnull : d->pageLastResume, reason);

    d->pageLastResume = pageTmp;
}

ZF_NAMESPACE_GLOBAL_END

