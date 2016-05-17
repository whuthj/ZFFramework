/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUITypeDef_IODef.h
 * @brief type io with io callbacks
 */

#ifndef _ZFI_ZFUITypeDef_IODef_h_
#define _ZFI_ZFUITypeDef_IODef_h_

#include "ZFUITypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOUTPUT_TYPE_DECLARE(ZFUIPoint)
ZFOUTPUT_TYPE(const ZFUIPoint *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIPoint *, {output << (const ZFUIPoint *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIPoint, ZFUIPoint)

ZFOUTPUT_TYPE_DECLARE(ZFUIMargin)
ZFOUTPUT_TYPE(const ZFUIMargin *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIMargin *, {output << (const ZFUIMargin *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIMargin, ZFUIMargin)

ZFOUTPUT_TYPE_DECLARE(ZFUISize)
ZFOUTPUT_TYPE(const ZFUISize *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUISize *, {output << (const ZFUISize *)v;})
ZFINPUT_TYPE_DECLARE(ZFUISize, ZFUISize)

ZFOUTPUT_TYPE_DECLARE(ZFUIRect)
ZFOUTPUT_TYPE(const ZFUIRect *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIRect *, {output << (const ZFUIRect *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIRect, ZFUIRect)

ZFOUTPUT_TYPE_DECLARE(ZFUIColor)
ZFOUTPUT_TYPE(const ZFUIColor *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIColor *, {output << (const ZFUIColor *)v;})
ZFINPUT_TYPE_DECLARE(ZFUIColor, ZFUIColor)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITypeDef_IODef_h_

