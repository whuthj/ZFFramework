/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIImage.h
 * @brief protocol for ZFUIImage
 */

#ifndef _ZFI_ZFProtocolZFUIImage_h_
#define _ZFI_ZFProtocolZFUIImage_h_

#include "../ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIImage
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIImage)
public:
    /**
     * @brief load image from input as binary data,
     *   created image would be released by #nativeImageRelease
     */
    virtual void *nativeImageFromInput(ZF_IN const ZFInputCallback &inputCallback) = 0;
    /**
     * @brief save image to output as binary data
     */
    virtual zfbool nativeImageToOutput(ZF_IN void *nativeImage,
                                       ZF_OUT const ZFOutputCallback &outputCallback) = 0;

    /**
     * @brief copy the nativeImage and return the copied one,
     *   copied image would be released by #nativeImageRelease
     */
    virtual void *nativeImageCopy(ZF_IN void *nativeImage) = 0;

    /**
     * @brief retain the image,
     *   you may return a different one
     */
    virtual void *nativeImageRetain(ZF_IN void *nativeImage) = 0;
    /**
     * @brief release the image
     *
     * you should implements retain count logic internally
     */
    virtual void nativeImageRelease(ZF_IN void *nativeImage) = 0;

    /**
     * @brief get size of the image (in pixel)
     */
    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage) = 0;
ZFPROTOCOL_INTERFACE_END(ZFUIImage)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIImageImplNinePatchPos value="value">
 *   </ZFUIImageImplNinePatchPos>
 * @endcode
 */
#define ZFPropertyTypeId_ZFUIImageImplNinePatchPosEnum zfText("ZFUIImageImplNinePatchPos")
/**
 * @brief see #ZFUIImageImplNinePatchCalc
 */
ZFENUM_BEGIN(ZFUIImageImplNinePatchPos)
    ZFENUM_VALUE(TopLeft)
    ZFENUM_VALUE(TopCenter)
    ZFENUM_VALUE(TopRight)
    ZFENUM_VALUE(CenterLeft)
    ZFENUM_VALUE(CenterCenter)
    ZFENUM_VALUE(CenterRight)
    ZFENUM_VALUE(BottomLeft)
    ZFENUM_VALUE(BottomCenter)
    ZFENUM_VALUE(BottomRight)
ZFENUM_SEPARATOR(ZFUIImageImplNinePatchPos)
    ZFENUM_VALUE_REGISTER(TopLeft)
    ZFENUM_VALUE_REGISTER(TopCenter)
    ZFENUM_VALUE_REGISTER(TopRight)
    ZFENUM_VALUE_REGISTER(CenterLeft)
    ZFENUM_VALUE_REGISTER(CenterCenter)
    ZFENUM_VALUE_REGISTER(CenterRight)
    ZFENUM_VALUE_REGISTER(BottomLeft)
    ZFENUM_VALUE_REGISTER(BottomCenter)
    ZFENUM_VALUE_REGISTER(BottomRight)
ZFENUM_END(ZFUIImageImplNinePatchPos)

/**
 * @brief see #ZFUIImageImplNinePatchCalc
 */
zfclassPOD ZF_ENV_EXPORT ZFUIImageImplNinePatchDrawData
{
public:
    ZFUIImageImplNinePatchPosEnum position; /**< @brief position of the nine patch piece */
    ZFUIRect src; /**< @brief src rect to draw bitmap from, ensured valid */
    ZFUIRect dst; /**< @brief dst rect to draw bitmapt to, ensured valid */
};
ZFOUTPUT_TYPE_DECLARE(ZFUIImageImplNinePatchDrawData)
ZFOUTPUT_TYPE(const ZFUIImageImplNinePatchDrawData *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUIImageImplNinePatchDrawData *, {output << (const ZFUIImageImplNinePatchDrawData *)v;})

/**
 * @brief util method for implementation to calculate nine patch areas
 *
 * return draw areas containing src and dst rect info,
 * only valid rects would be stored in result,
 * outBuf should be at least (9 * sizeof(ZFUIImageImplNinePatchDrawData)),
 * typically you may use "ZFUIImageImplNinePatchDrawData buf[9];"
 * to supply the buf
 * @note all size use raw pixel as size unit
 */
extern ZF_ENV_EXPORT zfindex ZFUIImageImplNinePatchCalc(ZF_OUT ZFUIImageImplNinePatchDrawData *outBuf,
                                                        ZF_IN const ZFUISize &srcSize,
                                                        ZF_IN const ZFUIMargin &srcNinePatch,
                                                        ZF_IN const ZFUISize &dstSize);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIImage_h_

