/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIListCell.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIListCellStyle, ZFUIViewStyle)

// ============================================================
ZFOBJECT_REGISTER(ZFUIListCell)
ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIListCell, ZFUIView *, cellView)
{
    if(this->cellView() != zfnull)
    {
        this->internalForegroundViewRemove(this->cellView());
    }
    this->cellViewSetInternal(newValue);
    if(this->cellView() != zfnull)
    {
        this->internalForegroundViewAdd(this->cellView(), this->cellViewLayoutParam());
    }
}
void ZFUIListCell::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    if(this->cellView() == zfnull)
    {
        zfsuper::layoutOnMeasure(ret, sizeHint, sizeParam);
        return ;
    }

    this->cellView()->layoutMeasure(sizeHint, ZFUISizeParamWrapWidthWrapHeight);
    ret = ZFUIViewLayoutParam::sizeHintApply(this->cellView()->layoutMeasuredSize(), sizeHint, sizeParam);
}

ZF_NAMESPACE_GLOBAL_END

