/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIKit.hh"
#include "ZFUIImage.h"
#include "protocol/ZFProtocolZFUIImage.h"
#include "ZFUIGlobalStyle.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serializabel data
zfclassPOD _ZFP_ZFUIImageSerializeTypeData
{
public:
    ZFUIImageSerializeFromCallback fromCallback;
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageSerializeTypeHolder, ZFLevelZFFrameworkNormal)
{
}
ZFCoreMap types; // _ZFP_ZFUIImageSerializeTypeData
ZF_GLOBAL_INITIALIZER_END(ZFUIImageSerializeTypeHolder)
void ZFUIImageSerializeTypeRegister(ZF_IN const zfchar *name,
                                    ZF_IN ZFUIImageSerializeFromCallback fromCallback)
{
    if(name != zfnull && fromCallback != zfnull)
    {
        _ZFP_ZFUIImageSerializeTypeData *typeData = (_ZFP_ZFUIImageSerializeTypeData *)zfmalloc(sizeof(_ZFP_ZFUIImageSerializeTypeData));
        typeData->fromCallback = fromCallback;
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.set(name,
            ZFCorePointerForPOD<_ZFP_ZFUIImageSerializeTypeData *>(typeData));
    }
}
void ZFUIImageSerializeTypeUnregister(ZF_IN const zfchar *name)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.remove(name);
}

ZFCoreArrayPOD<const zfchar *> ZFUIImageSerializeTypeGetAll(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.allKey();
}

// ============================================================
// _ZFP_ZFUIImagePrivate
zfclassNotPOD _ZFP_ZFUIImagePrivate
{
public:
    void *nativeImage;
    ZFUISize imageSize;

public:
    zfchar *serializableType;
    const ZFSerializableData *serializableData;

public:
    void copyFrom(ZF_IN _ZFP_ZFUIImagePrivate *another)
    {
        if(this->nativeImage != zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(this->nativeImage);
            this->nativeImage = zfnull;
        }
        this->imageSize = ZFUISizeZero;
        zfsChange(this->serializableType, (const zfchar *)zfnull);
        if(this->serializableData != zfnull)
        {
            zfdelete(this->serializableData);
            this->serializableData = zfnull;
        }

        if(another->nativeImage != zfnull)
        {
            this->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(another->nativeImage);
            this->imageSize = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageSize(this->nativeImage);
        }
        zfsChange(this->serializableType, another->serializableType);
        if(another->serializableData != zfnull)
        {
            this->serializableData = zfnew(ZFSerializableData, *(another->serializableData));
        }
    }

public:
    _ZFP_ZFUIImagePrivate(void)
    : nativeImage(zfnull)
    , imageSize(ZFUISizeZero)
    , serializableType(zfnull)
    , serializableData(zfnull)
    {
    }
    ~_ZFP_ZFUIImagePrivate(void)
    {
        zffree(this->serializableType);
        zfdelete(this->serializableData);
    }
};

// ============================================================
// ZFUIImage
ZFOBJECT_REGISTER(ZFUIImage)

zfbool ZFUIImage::serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                          ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                          ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryFromData(ownerData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    { // scale
        this->imageScaleSet(0);
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(ownerData, ZFSerializableKeyword_ZFUIImage_scale);
        if(categoryData != zfnull)
        {
            zffloat scale = 0;
            if(!zffloatFromSerializableData(scale, *categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            this->imageScaleSet(scale);

            if(categoryData->referenceInfoExist())
            {
                this->serializableRefInfoSetForCategory(ZFSerializableKeyword_ZFUIImage_scale, categoryData->referenceInfoCopy());
            }
        }
    }

    { // bin
        const ZFSerializableData *categoryData = ZFSerializableUtil::checkElementByCategory(ownerData, ZFSerializableKeyword_ZFUIImage_bin);
        if(categoryData != zfnull)
        {
            const zfchar *value = ZFSerializableUtil::requirePropertyValue(*categoryData, outErrorHintToAppend, outErrorPos);
            if(value == zfnull)
            {
                return zffalse;
            }
            if(!ZFUIImageEncodeFromBase64(this, ZFInputCallbackForBuffer(value)))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, *categoryData,
                    zfText("fail to load image from base64 data: \"%s\""), value);
                return zffalse;
            }

            categoryData->resolveMark();

            if(categoryData->referenceInfoExist())
            {
                this->serializableRefInfoSetForCategory(ZFSerializableKeyword_ZFUIImage_bin, categoryData->referenceInfoCopy());
            }

            return zftrue;
        }
    }

    _ZFP_ZFUIImageSerializeTypeData *typeData = zfnull;
    const zfchar *typeName = zfnull;
    { // type
        const ZFSerializableData *categoryData = ZFSerializableUtil::requireElementByCategory(ownerData, ZFSerializableKeyword_ZFUIImage_type);
        if(categoryData == zfnull)
        {
            return zffalse;
        }
        if(!zfstringFromSerializableData(typeName, *categoryData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }

        typeData = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageSerializeTypeHolder)->types.get<_ZFP_ZFUIImageSerializeTypeData *>(typeName);
        if(typeData == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, *categoryData,
                zfText("no such image serializable type registered: \"%s\""), typeName);
            return zffalse;
        }

        categoryData->resolveMark();
    }

    ZFSerializableData imageData;
    { // data
        const ZFSerializableData *categoryData = ZFSerializableUtil::requireElementByCategory(ownerData, ZFSerializableKeyword_ZFUIImage_data);
        if(categoryData == zfnull)
        {
            return zffalse;
        }
        if(!typeData->fromCallback(this, *categoryData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }

        imageData = categoryData->copy();
        imageData.categorySet(zfnull);

        categoryData->resolveMark();
    }

    // check
    if(d->nativeImage == zfnull)
    {
        d->imageSize = ZFUISizeZero;
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData, zfText("nativeImage not set"));
        return zffalse;
    }

    this->imageSerializableTypeSet(typeName);
    this->imageSerializableDataSet(&imageData);

    return zftrue;
}
zfbool ZFUIImage::serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryToData(ownerData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    // check
    if(d->nativeImage == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("serialize an image whose nativeImage not set"));
        return zffalse;
    }

    if((ref == zfnull && this->imageScale() > 0)
        || (ref != zfnull && ref->imageScale() != this->imageScale()))
    { // scale
        ZFSerializableData categoryData;
        if(!zffloatToSerializableData(categoryData, this->imageScale(), outErrorHintToAppend))
        {
            return zffalse;
        }
        categoryData.categorySet(ZFSerializableKeyword_ZFUIImage_scale);

        zfbool needAdd = zftrue;
        if(needAdd)
        {
            const ZFSerializableData *refInfo = this->serializableRefInfoGetForCategory(ZFSerializableKeyword_ZFUIImage_scale);
            if(refInfo != zfnull)
            {
                ZFSerializableData tmpData;
                if(!refInfo->referenceLoadTo(tmpData, outErrorHintToAppend))
                {
                    return zffalse;
                }
                ZFSerializableUtil::resolveReferencedData(categoryData, *refInfo);
            }
        }
        if(needAdd)
        {
            ownerData.elementAdd(categoryData);
        }
    }

    if(this->imageSerializableType() != zfnull)
    {
        if(ref != zfnull
            && this->imageSerializableType() == ref->imageSerializableType()
            && this->imageSerializableData() != zfnull && this->imageSerializableData()->objectCompare(*(ref->imageSerializableData())))
        {
            return zftrue;
        }

        { // type
            ZFSerializableData categoryData;
            if(!zfstringToSerializableData(categoryData, this->imageSerializableType(), outErrorHintToAppend))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImage_type);
            ownerData.elementAdd(categoryData);
        }

        { // data
            if(this->imageSerializableData() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                    zfText("missing image data for type: \"%s\""), this->imageSerializableType());
                return zffalse;
            }

            ZFSerializableData categoryData = this->imageSerializableData()->copy();
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImage_data);
            ownerData.elementAdd(categoryData);
        }

        return zftrue;
    }

    { // bin
        ZFSerializableData categoryData;
        const zfchar *binDataTmp = zfnull;
        {
            zfstring binData;
            if(!ZFUIImageEncodeToBase64(ZFOutputCallbackForString(binData), this))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("save image to base64 failed"));
                return zffalse;
            }

            if(!zfstringToSerializableData(categoryData, binData, outErrorHintToAppend))
            {
                return zffalse;
            }
            categoryData.categorySet(ZFSerializableKeyword_ZFUIImage_bin);
            binDataTmp = categoryData.propertyValue();
        }

        zfbool needAdd = zftrue;
        if(ref != zfnull)
        {
            zfstring binDataRef;
            if(ZFUIImageEncodeToBase64(ZFOutputCallbackForString(binDataRef), ref)
                && zfscmpTheSame(binDataRef.cString(), binDataTmp))
            {
                needAdd = zffalse;
            }
        }
        if(needAdd)
        {
            const ZFSerializableData *refInfo = this->serializableRefInfoGetForCategory(ZFSerializableKeyword_ZFUIImage_bin);
            if(refInfo != zfnull)
            {
                ZFSerializableData tmpData;
                if(!refInfo->referenceLoadTo(tmpData, outErrorHintToAppend))
                {
                    return zffalse;
                }
                ZFSerializableUtil::resolveReferencedData(categoryData, *refInfo);
            }
        }
        if(needAdd)
        {
            ownerData.elementAdd(categoryData);
        }
    }

    return zftrue;
}

void ZFUIImage::copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
{
    zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);

    d->copyFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj)->d);
}

ZFUISize ZFUIImage::imageSize(void)
{
    return ZFUISizeApplyScaleReversely(d->imageSize, this->imageScaleFixed());
}
const ZFUISize &ZFUIImage::imageSizeFixed(void)
{
    return d->imageSize;
}

ZFObject *ZFUIImage::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIImagePrivate);
    return this;
}
void ZFUIImage::objectOnDealloc(void)
{
    if(d != zfnull)
    {
        if(d->nativeImage != zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(d->nativeImage);
            d->nativeImage = zfnull;
        }
        zfpoolDelete(d);
        d = zfnull;
    }
    zfsuper::objectOnDealloc();
}

zfidentity ZFUIImage::objectHash(void)
{
    return zfidentityCalcPointer(d->nativeImage);
}
ZFCompareResult ZFUIImage::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(d->nativeImage == another->d->nativeImage
        && this->imageNinePatch() == another->imageNinePatch()
        && zfscmpTheSame(d->serializableType, another->d->serializableType)
        && ((d->serializableData == zfnull && another->d->serializableData == zfnull)
            || (d->serializableData != zfnull && another->d->serializableData != zfnull
                && d->serializableData->objectCompare(*(another->d->serializableData)) == ZFCompareTheSame)))
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}
void ZFUIImage::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->className();
    ret += zfText(" ");
    zfsFromPointerT(ret, this);
    ret += zfText(" ");
    ZFUISizeToString(ret, this->imageSize());
}

void *ZFUIImage::nativeImage(void)
{
    return d->nativeImage;
}

void ZFUIImage::nativeImageSet(ZF_IN void *nativeImage)
{
    void *toRelease = d->nativeImage;

    if(nativeImage != zfnull)
    {
        d->nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRetain(nativeImage);
        d->imageSize = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageSize(d->nativeImage);
    }
    else
    {
        d->nativeImage = zfnull;
        d->imageSize = ZFUISizeZero;
    }

    if(toRelease != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(toRelease);
    }
}

void ZFUIImage::imageSerializableTypeSet(ZF_IN const zfchar *typeName)
{
    zfsChange(d->serializableType, typeName);
}
const zfchar *ZFUIImage::imageSerializableType(void)
{
    return d->serializableType;
}
void ZFUIImage::imageSerializableDataSet(ZF_IN const ZFSerializableData *serializableData)
{
    if(d->serializableData != zfnull)
    {
        zfdelete(d->serializableData);
        d->serializableData = zfnull;
    }
    if(serializableData != zfnull)
    {
        d->serializableData = zfnew(ZFSerializableData, *serializableData);
    }
}
const ZFSerializableData *ZFUIImage::imageSerializableData(void)
{
    return d->serializableData;
}

// ============================================================
zfbool ZFUIImageEncodeFromBase64(ZF_IN_OUT ZFUIImage *image,
                                 ZF_IN const ZFInputCallback &inputCallback)
{
    ZFIOBridgeCallbackUsingTmpFile io;
    if(image != zfnull && ZFBase64Decode(io.outputCallback(), inputCallback))
    {
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(io.inputCallback());
        if(nativeImage != zfnull)
        {
            image->imageSerializableTypeSet(zfnull);
            image->imageSerializableDataSet(zfnull);
            image->nativeImageSet(nativeImage);
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
            return zftrue;
        }
    }
    return zffalse;
}
zfautoObject ZFUIImageEncodeFromBase64(ZF_IN const ZFInputCallback &inputCallback)
{
    zfautoObject ret = ZFUIImage::ClassData()->newInstanceWithLeakTest(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(ZFUIImageEncodeFromBase64(ret.to<ZFUIImage *>(), inputCallback))
    {
        return ret;
    }
    else
    {
        return zfautoObjectNull;
    }
}
zfbool ZFUIImageEncodeToBase64(ZF_OUT const ZFOutputCallback &outputCallback,
                               ZF_IN ZFUIImage *image)
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback.callbackIsValid())
    {
        ZFIOBridgeCallbackUsingTmpFile io;
        if(!ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), io.outputCallback()))
        {
            return zffalse;
        }
        return ZFBase64Encode(outputCallback, io.inputCallback());
    }
    return zffalse;
}
zfbool ZFUIImageEncodeFromBinary(ZF_IN_OUT ZFUIImage *image,
                                 ZF_IN const ZFInputCallback &inputCallback)
{
    if(image != zfnull && inputCallback.callbackIsValid())
    {
        void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageFromInput(inputCallback);
        if(nativeImage != zfnull)
        {
            image->imageSerializableTypeSet(zfnull);
            image->imageSerializableDataSet(zfnull);
            image->nativeImageSet(nativeImage);
            ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageRelease(nativeImage);
            return zftrue;
        }
    }
    return zffalse;
}
zfautoObject ZFUIImageEncodeFromBinary(ZF_IN const ZFInputCallback &inputCallback)
{
    zfautoObject ret = ZFUIImage::ClassData()->newInstanceWithLeakTest(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(ZFUIImageEncodeFromBinary(ret.to<ZFUIImage *>(), inputCallback))
    {
        return ret;
    }
    else
    {
        return zfautoObjectNull;
    }
}
zfbool ZFUIImageEncodeToBinary(ZF_OUT const ZFOutputCallback &outputCallback,
                               ZF_IN ZFUIImage *image)
{
    if(image != zfnull && image->nativeImage() != zfnull && outputCallback.callbackIsValid())
    {
        return ZFPROTOCOL_ACCESS(ZFUIImage)->nativeImageToOutput(image->nativeImage(), outputCallback);
    }
    return zffalse;
}

zffloat ZFUIImageGlobalScale()
{
    return ZFUIGlobalStyle::DefaultStyle()->imageScale();
}
void ZFUIImageGlobalScaleSet(ZF_IN zffloat scale)
{
    if(scale > 0)
    {
        ZFUIGlobalStyle::DefaultStyle()->imageScaleSet(scale);
    }
}

ZF_NAMESPACE_GLOBAL_END

