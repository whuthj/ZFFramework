/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITypeDef_IODef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOUTPUT_TYPE_DEFINE(ZFUIPoint, {output << ZFUIPointToString(v);})
ZFINPUT_TYPE_DEFINE(ZFUIPoint, ZFUIPoint, {
    v = ZFUIPointZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.x = elementsTmp[0];
    v.y = elementsTmp[1];
    return zftrue;
})

ZFOUTPUT_TYPE_DEFINE(ZFUIMargin, {output << ZFUIMarginToString(v);})
ZFINPUT_TYPE_DEFINE(ZFUIMargin, ZFUIMargin, {
    v = ZFUIMarginZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.left = elementsTmp[0];
    v.top = elementsTmp[1];
    v.right = elementsTmp[2];
    v.bottom = elementsTmp[3];
    return zftrue;
})

ZFOUTPUT_TYPE_DEFINE(ZFUISize, {output << ZFUISizeToString(v);})
ZFINPUT_TYPE_DEFINE(ZFUISize, ZFUISize, {
    v = ZFUISizeZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 2))
    {
        return zffalse;
    }
    v.width = elementsTmp[0];
    v.height = elementsTmp[1];
    return zftrue;
})

ZFOUTPUT_TYPE_DEFINE(ZFUIRect, {output << ZFUIRectToString(v);})
ZFINPUT_TYPE_DEFINE(ZFUIRect, ZFUIRect, {
    v = ZFUIRectZero;
    ZFCoreArrayPOD<zfint> elementsTmp;
    if(!ZFInputCallbackReadDataPairInt(elementsTmp, input, 4))
    {
        return zffalse;
    }
    v.point.x = elementsTmp[0];
    v.point.y = elementsTmp[1];
    v.size.width = elementsTmp[2];
    v.size.height = elementsTmp[3];
    return zftrue;
})

ZFOUTPUT_TYPE_DEFINE(ZFUIColor, {output << ZFUIColorToString(v);})
ZFINPUT_TYPE_DEFINE(ZFUIColor, ZFUIColor, {
    v = ZFUIColorZero;
    zfchar buf[16] = {0};
    if((buf[0] = ZFInputCallbackSkipChars(input)) == 0)
    {
        return zffalse;
    }
    if(input.execute(buf + 1, 8) != 8)
    {
        return zffalse;
    }
    return (ZFUIColorFromString(v, buf) == zfnull);
})

ZF_NAMESPACE_GLOBAL_END

