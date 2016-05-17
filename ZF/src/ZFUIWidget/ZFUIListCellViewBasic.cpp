/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIWidget.hh"
#include "ZFUIListCellViewBasic.h"
#include "ZFUIViewLayout.h"
#include "ZFUILinearLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFUIListCellViewBasic_cellIconInit(ZF_IN ZFUIViewStyle *cellIcon)
{
    cellIcon->viewSizeMaxSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeIcon()));
}
void _ZFP_ZFUIListCellViewBasic_cellLabelMainInit(ZF_IN ZFUITextViewStyle *cellLabelMain)
{
    // nothing to do
}
void _ZFP_ZFUIListCellViewBasic_cellLabelSubInit(ZF_IN ZFUITextViewStyle *cellLabelSub)
{
    cellLabelSub->textColorSet(ZFUIGlobalStyle::DefaultStyle()->colorTextSecondary());
    cellLabelSub->textSizeSet(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
}

ZFSTYLE_DEFAULT_DEFINE(ZFUIListCellViewBasicStyle, ZFUIViewStyle)

// ============================================================
zfclassNotPOD _ZFP_ZFUIListCellViewBasicPrivate
{
public:
    ZFUIViewLayout *cellIconContainer;
    ZFUILinearLayout *cellCenterContainer;
    ZFUIViewLayout *cellLabelMainContainer;
    ZFUIViewLayout *cellLabelSubContainer;
    ZFUIViewLayout *cellAccessoryContainer;
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCellViewBasic)

ZFUIView *ZFUIListCellViewBasic::cellIconContainer(void)
{
    return d->cellIconContainer;
}
ZFUIView *ZFUIListCellViewBasic::cellCenterContainer(void)
{
    return d->cellCenterContainer;
}
ZFUIView *ZFUIListCellViewBasic::cellLabelMainContainer(void)
{
    return d->cellLabelMainContainer;
}
ZFUIView *ZFUIListCellViewBasic::cellLabelSubContainer(void)
{
    return d->cellLabelSubContainer;
}
ZFUIView *ZFUIListCellViewBasic::cellAccessoryContainer(void)
{
    return d->cellAccessoryContainer;
}

ZFObject *ZFUIListCellViewBasic::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIListCellViewBasicPrivate);

    d->cellIconContainer = zfAllocInternal(ZFUIViewLayout);

    {
        d->cellCenterContainer = zfAllocInternal(ZFUILinearLayout);
        d->cellCenterContainer->layoutOrientationSet(ZFUIOrientation::e_Top);

        d->cellLabelMainContainer = zfAllocInternal(ZFUIViewLayout);
        d->cellCenterContainer->childAdd(d->cellLabelMainContainer);
        d->cellLabelMainContainer->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);

        d->cellLabelSubContainer = zfAllocInternal(ZFUIViewLayout);
        d->cellCenterContainer->childAdd(d->cellLabelSubContainer);
        d->cellLabelSubContainer->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
    }

    d->cellAccessoryContainer = zfAllocInternal(ZFUIViewLayout);

    return this;
}
void ZFUIListCellViewBasic::objectOnDealloc(void)
{
    zfReleaseInternal(d->cellIconContainer);
    zfReleaseInternal(d->cellCenterContainer);
    zfReleaseInternal(d->cellLabelMainContainer);
    zfReleaseInternal(d->cellLabelSubContainer);
    zfReleaseInternal(d->cellAccessoryContainer);

    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIListCellViewBasic::objectPropertyValueOnInit(ZF_IN const ZFProperty *property)
{
    zfsuper::objectPropertyValueOnInit(property);

    if(zffalse)
    {
    }
    else if(property == ZFPropertyAccess(ZFUIListCellViewBasicStyle, cellIcon))
    {
        ZFUIView *cellIconView = this->cellIcon()->to<ZFUIView *>();
        d->cellIconContainer->childAdd(cellIconView);
        cellIconView->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    }
    else if(property == ZFPropertyAccess(ZFUIListCellViewBasicStyle, cellLabelMain))
    {
        ZFUIView *cellLabelMainView = this->cellLabelMain()->to<ZFUIView *>();
        d->cellLabelMainContainer->childAdd(cellLabelMainView);
        cellLabelMainView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
    }
    else if(property == ZFPropertyAccess(ZFUIListCellViewBasicStyle, cellLabelSub))
    {
        ZFUIView *cellLabelSubView = this->cellLabelSub()->to<ZFUIView *>();
        d->cellLabelSubContainer->childAdd(cellLabelSubView);
        cellLabelSubView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
    }
}

ZF_NAMESPACE_GLOBAL_END

