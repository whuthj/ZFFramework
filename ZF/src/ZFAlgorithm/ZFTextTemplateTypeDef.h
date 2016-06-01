/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTextTemplateTypeDef.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplateTypeDef_h_
#define _ZFI_ZFTextTemplateTypeDef_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateIndexFlagEnum value="value">
 *   </ZFTextTemplateIndexFlagEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFTextTemplateIndexFlagEnum zfText("ZFTextTemplateIndexFlagEnum")
/**
 * @brief index output flags for #ZFTextTemplateIndexData
 */
ZFENUM_BEGIN(ZFTextTemplateIndexFlag)
    ZFENUM_VALUE(LeadingZero)
    ZFENUM_VALUE(LeadingSpace)
    ZFENUM_VALUE(TailSpace)
ZFENUM_SEPARATOR(ZFTextTemplateIndexFlag)
    ZFENUM_VALUE_REGISTER(LeadingZero)
    ZFENUM_VALUE_REGISTER(LeadingSpace)
    ZFENUM_VALUE_REGISTER(TailSpace)
ZFENUM_END_WITH_DEFAULT(ZFTextTemplateIndexFlag, LeadingZero)

// ============================================================
/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateIndexData
{
public:
    /**
     * @brief start index, 0 by default
     */
    zfindex indexStart;
    /**
     * @brief increase or decrease the index, 1 by default,
     *   must not be 0
     */
    zfint indexOffset;
    /**
     * @brief radix, 10 by default
     */
    zfindex indexRadix;
    /**
     * @brief upper case or not, zftrue by default
     */
    zfbool indexUpperCase;
    /**
     * @brief specify width, 0 to disable, 0 by default
     */
    zfindex indexWidth;
    /**
     * @brief flag to use when output index,
     *   valid only if #indexWidth greater than 0,
     *   #ZFTextTemplateIndexFlag::e_Default by default
     */
    ZFTextTemplateIndexFlagEnum indexFlag;

public:
    ZFTextTemplateIndexData(void)
    : indexStart(0)
    , indexOffset(zftrue)
    , indexRadix(10)
    , indexUpperCase(zftrue)
    , indexWidth(0)
    , indexFlag(ZFTextTemplateIndexFlag::e_Default())
    {
    }

public:
    /** @brief copy from another */
    void copyFrom(ZF_IN const ZFTextTemplateIndexData &ref)
    {
        this->indexStart = ref.indexStart;
        this->indexOffset = ref.indexOffset;
        this->indexRadix = ref.indexRadix;
        this->indexUpperCase = ref.indexUpperCase;
        this->indexWidth = ref.indexWidth;
        this->indexFlag = ref.indexFlag;
    }

public:
    /** @brief see #ZFObject::objectInfoT */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #ZFObject::objectInfo */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
};

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateIndexData
 *       indexStart="0" // optional, 0 by default
 *       indexOffset="1" // optional, 1 by default
 *       indexRadix="10" // optional, 10 by default
 *       indexUpperCase="true" // optional, true by default
 *       indexWidth="0" // optional, 0 by default
 *       indexFlag="LeadingZero" // optional, ZFTextTemplateIndexFlag::e_Default() by default
 *   />
 * @endcode
 */
#define ZFPropertyTypeId_ZFTextTemplateIndexData zfText("ZFTextTemplateIndexData")
ZFPROPERTY_TYPE_DECLARE(ZFTextTemplateIndexData, ZFTextTemplateIndexData)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart zfText("indexStart")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset zfText("indexOffset")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix zfText("indexRadix")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase zfText("indexUpperCase")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth zfText("indexWidth")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag zfText("indexFlag")

ZFOUTPUT_TYPE(ZFTextTemplateIndexData, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFTextTemplateIndexData *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFTextTemplateIndexData *, {output << (const ZFTextTemplateIndexData *)v;})

// ============================================================
/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateParam
{
public:
    /**
     * @brief see #ZFTextTemplateApply
     *
     * map to zfstring, holds the replaced text
     */
    ZFCoreMap replaceDatas;
    /**
     * @brief see #ZFTextTemplateApply
     *
     * map to zfbool, shows whether the key should be enabled
     */
    ZFCoreMap enableIfDatas;
    /**
     * @brief see #ZFTextTemplateApply
     *
     * whether the key should be enabled if not exists, false by default
     */
    zfbool enableIfByDefault;
    /**
     * @brief see #ZFTextTemplateApply
     *
     * map to #ZFTextTemplateIndexData
     */
    ZFCoreMap indexDatas;
    /**
     * @brief default setting if #indexDatas not set
     */
    ZFTextTemplateIndexData indexDataDefault;

public:
    /** @cond ZFPrivateDoc */
    ZFTextTemplateParam(void)
    : replaceDatas()
    , enableIfDatas()
    , enableIfByDefault(zffalse)
    , indexDatas()
    , indexDataDefault()
    {
    }
    /** @endcond */

public:
    /** @brief copy from another */
    void copyFrom(ZF_IN const ZFTextTemplateParam &ref);

public:
    /** @brief see #ZFObject::objectInfoT */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #ZFObject::objectInfo */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
};

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateParam>
 *       <node category="replace"> // optional
 *           <zfstring name="key0" value="value0" />
 *           <zfstring name="key1" value="value1" />
 *           ...
 *       </node>
 *
 *       <node category="enableIf"> // optional
 *           <zfbool name="key0" value="value0" />
 *           <zfbool name="key1" value="value1" />
 *           ...
 *       </node>
 *
 *       <zfbool category="enableIfByDefault" ... /> // optional, false by default
 *
 *       <node category="index"> // optional
 *           <ZFTextTemplateIndexData name="key0" ... />
 *           <ZFTextTemplateIndexData name="key1" ... />
 *           ...
 *       </node>
 *
 *       <ZFTextTemplateIndexData category="indexDataDefault" ... /> // optional
 *   </ZFTextTemplateParam>
 * @endcode
 */
#define ZFPropertyTypeId_ZFTextTemplateParam zfText("ZFTextTemplateParam")
ZFPROPERTY_TYPE_DECLARE(ZFTextTemplateParam, ZFTextTemplateParam)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_replace zfText("replace")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_enableIf zfText("enableIf")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_enableIfByDefault zfText("enableIfByDefault")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_index zfText("index")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault zfText("indexDataDefault")

ZFOUTPUT_TYPE(ZFTextTemplateParam, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFTextTemplateParam *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFTextTemplateParam *, {output << (const ZFTextTemplateParam *)v;})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplateTypeDef_h_

