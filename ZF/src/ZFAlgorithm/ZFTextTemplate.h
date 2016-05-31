/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTextTemplate.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplate_h_
#define _ZFI_ZFTextTemplate_h_

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
    zfindex indexUpperCase;
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
};

// ============================================================
/**
 * @brief util method to apply template to plain text
 *
 * data would be applied accorrding to #ZFTextTemplateParam,
 * and result would be written to output,
 * return newly size written if success,
 * or zfindexMax if failed\n
 * \n
 * params of #ZFTextTemplateParam:
 * -  replaceDatas: a <string, string> map\n
 *   replace the content with specified string\n
 *   assume we have a <"myKey", "myValue"> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_R_myKey}, and this is {ZFTT_R_not_set}
 *
 *     // applied:
 *     this is myValue, and this is {ZFTT_R_not_set}
 *   @endcode
 * -  enableIfDatas: a <string, bool> map\n
 *   whether to enable text block\n
 *   assume we have a <"myCond0", true> map and a <"myCond1", false> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     aaa {ZFTT_C_myCond0}text in myCond0{ZFTT_CE} bbb
 *     ccc {ZFTT_C_myCond1}text in myCond1{ZFTT_CE} ddd
 *
 *     // result:
 *     aaa text in myCond0 bbb
 *     ccc  ddd
 *   @endcode
 * -  enableIfByDefault: for entries not exist in enableIfDatas,
 *   whether we should enable it, false by default
 * -  indexDatas: a <string, ZFTextTemplateIndexData> map\n
 *   replace the content with specified index value\n
 *   the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_I_myKey} item
 *     this is {ZFTT_I_myKey} item
 *
 *     // applied:
 *     this is 0 item
 *     this is 1 item
 *   @endcode
 * -  indexDataDefault: default setting if not set in indexDatas
 */
extern ZF_ENV_EXPORT zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN const ZFOutputCallback &output,
                                                 ZF_IN const zfchar *data,
                                                 ZF_IN_OPT zfindex dataSize = zfindexMax);
/** @brief see #ZFTextTemplateApply */
extern ZF_ENV_EXPORT zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN const ZFOutputCallback &output,
                                                 ZF_IN const ZFInputCallback &input);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplate_h_

