/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIImage.h
 * @brief image
 */

#ifndef _ZFI_ZFUIImage_h_
#define _ZFI_ZFUIImage_h_

#include "ZFUITypeDef.h"
#include "ZFUIGlobalStyle.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFUIImage;

// ============================================================
/**
 * @brief internal use only, see #ZFUIImageSerializeTypeRegister
 */
typedef zfbool (*ZFUIImageSerializeFromCallback)(ZF_IN_OUT ZFUIImage *image,
                                                 ZF_IN const ZFSerializableData &imageData,
                                                 ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                 ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */);

/**
 * @brief register a serializable logic for ZFUIImage
 *
 * ZFUIImage can be loaded from many sources,
 * so we supply a dynamic serializable logic depending on image's load logic,
 * use this methods along with #ZFUIImageSerializeTypeUnregister and #ZF_GLOBAL_INITIALIZER_INIT
 * to register your own serialize logic,
 * such as load from file or load from color\n
 * \n
 * typically you should use #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE to define your type for short:
 * @code
 *   // in header file
 *   #define ZFUIImageSerializeType_yourType yourType
 *   #define ZFUIImageSerializeTypeName_yourType ZFMACRO_TOSTRING(ZFUIImageSerializeType_yourType)
 *
 *   // in cpp file
 *   ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(ZFUIImageSerializeType_yourType)
 *   {
 *       // serialize callback from data
 *       return zftrue;
 *   }
 * @endcode
 * proto type:\n
 *   zfbool action(ZF_IN_OUT ZFUIImage *result,
 *                ZF_IN const ZFSerializableData &serializableData,
 *                ZF_OUT_OPT zfstring *outErrorHintToAppend,
 *                ZF_OUT_OPT const ZFSerializableData **outErrorPos);
 */
extern ZF_ENV_EXPORT void ZFUIImageSerializeTypeRegister(ZF_IN const zfchar *name,
                                                         ZF_IN ZFUIImageSerializeFromCallback fromCallback);
/**
 * @brief see #ZFUIImageSerializeTypeRegister
 */
extern ZF_ENV_EXPORT void ZFUIImageSerializeTypeUnregister(ZF_IN const zfchar *name);
#define _ZFP_ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(type) \
    static zfbool _ZFP_ZFUIImageSerializeFromCallback_##type(ZF_IN_OUT ZFUIImage *result, \
                                                             ZF_IN const ZFSerializableData &serializableData, \
                                                             ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
                                                             ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageSerializeCallbackRegister_##type, ZFLevelZFFrameworkNormal) \
    { \
        ZFUIImageSerializeTypeRegister(ZFUIImageSerializeTypeName_##type, _ZFP_ZFUIImageSerializeFromCallback_##type); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIImageSerializeCallbackRegister_##type) \
    { \
        ZFUIImageSerializeTypeUnregister(ZFUIImageSerializeTypeName_##type); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFUIImageSerializeCallbackRegister_##type) \
    static zfbool _ZFP_ZFUIImageSerializeFromCallback_##type(ZF_IN_OUT ZFUIImage *result, \
                                                             ZF_IN const ZFSerializableData &serializableData, \
                                                             ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
                                                             ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
/**
 * @brief see #ZFUIImageSerializeTypeRegister
 */
#define ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(type) \
    _ZFP_ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(type)

/**
 * @brief usually for debug use only
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<const zfchar *> ZFUIImageSerializeTypeGetAll(void);

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_scale zfText("scale")

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_type zfText("type")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_data zfText("data")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_bin zfText("bin")

// ============================================================
// ZFUIImage
zfclassFwd _ZFP_ZFUIImagePrivate;
/**
 * @brief image storage
 * @note #ZFUIImage always use custom scale value, see #ZFUIGlobalStyle::imageScale
 *
 * \n
 * serializable data:
 * @code
 *   <ZFUIImage>
 *       <zfstring category="type" value="type name for ZFUIImageSerializeTypeRegister" />
 *       <anything category="data" ... /> // depends on load logic, see #ZFUIImageSerializeTypeRegister
 *
 *       // if nothing supplied, we will serialize by binary data
 *       <zfstring category="bin" value="image binary data, base64 encoded" />
 *   </ZFUIImage>
 * @endcode
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIImage : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFUIImage, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                           ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                         ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj);

    // ============================================================
    // property
public:
    /**
     * @brief image's scale
     *
     * 0 or less means not set, and #ZFUIGlobalStyle::imageScale would be used
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, imageScale, ZFPropertyInitValue(0))
public:
    /**
     * @brief see #imageScale
     */
    virtual zffloat imageScaleFixed(void)
    {
        return (this->imageScale() > 0 ? this->imageScale() : ZFUIGlobalStyle::DefaultStyle()->imageScale());
    }
    /**
     * @brief nine patch described by a margin value, disabled if zero margin or margin exceeds image size
     * @note #ZFUIImage always use custom scale value, see #ZFUIGlobalStyle::imageScale
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, imageNinePatch)
    ZFPROPERTY_CUSTOM_SETTER_DECLARE(ZFUIMargin, imageNinePatch)
    {
        if(newValue.left >= 0 && newValue.top >= 0 && newValue.right >= 0 && newValue.bottom >= 0)
        {
            this->imageNinePatchSetInternal(newValue);
        }
    }

    /**
     * @brief get size of the image
     * @note #ZFUIImage always use custom scale value, see #ZFUIGlobalStyle::imageScale
     */
    virtual ZFUISize imageSize(void);
    /**
     * @brief get raw size of the image without scale (in pixel unit)
     */
    virtual const ZFUISize &imageSizeFixed(void);

    // ============================================================
    // other
protected:
    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFUIImageLoadXxx")
    zfoverride
    virtual ZFObject *objectOnInit(void);
public:
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief get native image object
     */
    virtual void *nativeImage(void);

    // ============================================================
    // internal methods
public:
    /**
     * @brief internal use only, set the native image
     *
     * the image would be retained, actual retain logic is depending on the implementation
     */
    virtual void nativeImageSet(ZF_IN void *nativeImage);

    /**
     * @brief see #ZFUIImageSerializeTypeRegister
     */
    virtual void imageSerializableTypeSet(ZF_IN const zfchar *typeName);
    /**
     * @brief see #imageSerializableTypeSet
     */
    virtual const zfchar *imageSerializableType(void);
    /**
     * @brief see #ZFUIImageSerializeTypeRegister
     */
    virtual void imageSerializableDataSet(ZF_IN const ZFSerializableData *serializableData);
    /**
     * @brief see #imageSerializableDataSet
     */
    virtual const ZFSerializableData *imageSerializableData(void);

private:
    _ZFP_ZFUIImagePrivate *d;
};

// ============================================================
// image's binary load
/**
 * @brief load image from binary data (base64 encoded)
 * @note this method have no serialize logic,
 *   result image would be serialized by base64 value while serializing to data
 */
extern ZF_ENV_EXPORT zfbool ZFUIImageEncodeFromBase64(ZF_IN_OUT ZFUIImage *image,
                                                      ZF_IN const ZFInputCallback &inputCallback);
/**
 * @brief see #ZFUIImageEncodeFromBase64
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageEncodeFromBase64(ZF_IN const ZFInputCallback &inputCallback);
/**
 * @brief save image as binary data (base64 encoded)
 * @note this method have no serialize logic,
 *   result image would be serialized by base64 value while serializing to data
 */
extern ZF_ENV_EXPORT zfbool ZFUIImageEncodeToBase64(ZF_OUT const ZFOutputCallback &outputCallback,
                                                    ZF_IN ZFUIImage *image);

/**
 * @brief load image from binary data
 * @note this method have no serialize logic,
 *   result image would be serialized by base64 value while serializing to data
 */
extern ZF_ENV_EXPORT zfbool ZFUIImageEncodeFromBinary(ZF_IN_OUT ZFUIImage *image,
                                                      ZF_IN const ZFInputCallback &inputCallback);
/**
 * @brief see #ZFUIImageEncodeFromBinary
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIImageEncodeFromBinary(ZF_IN const ZFInputCallback &inputCallback);
/**
 * @brief save image as binary data
 * @note this method have no serialize logic,
 *   result image would be serialized by base64 value while serializing to data
 */
extern ZF_ENV_EXPORT zfbool ZFUIImageEncodeToBinary(ZF_OUT const ZFOutputCallback &outputCallback,
                                                    ZF_IN ZFUIImage *image);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImage_h_

#include "ZFUIImageCache.h"
#include "ZFUIImageCache_common.h"
#include "ZFUIImageIO.h"
#include "ZFUIImageRes.h"

