/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFJson.h
 * @brief JSON processing
 */

#ifndef _ZFI_ZFJson_h_
#define _ZFI_ZFJson_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * all types:
 *     JsonNull
 *     JsonValue
 *     JsonObject
 *     JsonArray
 *
 * here is a typical json data:
    "{"
    "    \"k0\" : \"v0\","
    ""
    "    \"object0\" : {},"
    "    \"object1\" : {"
    "        \"k0\" : \"v0\","
    "        \"k1\" : \"v1\""
    "    },"
    "    \"object2\" : {"
    "        \"k0\" : {\"k0\" : \"v0\"}"
    "    },"
    ""
    "    \"array0\" : [],"
    "    \"array1\" : ["
    "        {\"k0\" : \"v0\"},"
    "        {\"k0\" : \"v0\"}"
    "    ],"
    ""
    "    \"k1\" : \"v1\""
    "}"
 */

// ============================================================
// ZFJsonType
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFJsonTypeEnum value="value">
 *   </ZFJsonTypeEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFJsonTypeEnum zfText("ZFJsonTypeEnum")
/**
 * @brief json item type
 */
ZFENUM_BEGIN(ZFJsonType)
    ZFENUM_VALUE(JsonNull)
    ZFENUM_VALUE(JsonValue)
    ZFENUM_VALUE(JsonObject)
    ZFENUM_VALUE(JsonArray)
ZFENUM_SEPARATOR(ZFJsonType)
    ZFENUM_VALUE_REGISTER(JsonNull)
    ZFENUM_VALUE_REGISTER(JsonValue)
    ZFENUM_VALUE_REGISTER(JsonObject)
    ZFENUM_VALUE_REGISTER(JsonArray)
ZFENUM_END(ZFJsonType)

// ============================================================
/**
 * @brief token for output json
 */
zfclassLikePOD ZF_ENV_EXPORT ZFJsonOutputToken
{
public:
    zfstring jsonNewLineToken; /**< @brief "\n" by default */
    zfstring jsonIndentToken; /**< @brief "  " by default */

    zfstring jsonValueSeparatorToken; /**< @brief " : " by default */
    zfstring jsonSeparatorToken; /**< @brief "," by default */
    zfstring jsonSeparatorInSameLineToken; /**< @brief ", " by default */

    zfstring jsonKeyTagLeft; /**< @brief "\"" by default */
    zfstring jsonKeyTagRight; /**< @brief "\"" by default */

    zfstring jsonValueTagLeft; /**< @brief "\"" by default */
    zfstring jsonValueTagRight; /**< @brief "\"" by default */

    zfstring jsonObjectTagLeft; /**< @brief "{" by default */
    zfstring jsonObjectTagRight; /**< @brief "}" by default */

    zfstring jsonArrayTagLeft; /**< @brief "[" by default */
    zfstring jsonArrayTagRight; /**< @brief "]" by default */

public:
    /** @cond ZFPrivateDoc */
    ZFJsonOutputToken(void)
    : jsonNewLineToken(zfText("\n"))
    , jsonIndentToken(zfText("  "))
    , jsonValueSeparatorToken(zfText(" : "))
    , jsonSeparatorToken(zfText(","))
    , jsonSeparatorInSameLineToken(zfText(", "))
    , jsonKeyTagLeft(zfText("\""))
    , jsonKeyTagRight(zfText("\""))
    , jsonValueTagLeft(zfText("\""))
    , jsonValueTagRight(zfText("\""))
    , jsonObjectTagLeft(zfText("{"))
    , jsonObjectTagRight(zfText("}"))
    , jsonArrayTagLeft(zfText("["))
    , jsonArrayTagRight(zfText("]"))
    {
    }
    /** @endcond */
};
/**
 * @brief flags to output json items
 */
zfclassLikePOD ZF_ENV_EXPORT ZFJsonOutputFlags
{
public:
    /**
     * @brief token to output json data
     */
    ZFJsonOutputToken jsonToken;

    /**
     * @brief strings added to head of each new line, empty by default
     */
    zfstring jsonGlobalLineBeginToken;

    /**
     * @brief whether add new line for content of a json object, true by default
     */
    zfbool jsonObjectAddNewLineForContent;
    /**
     * @brief whether to put right tag in same line if no content, true by default
     */
    zfbool jsonObjectTagInSameLineIfNoContent;

    /**
     * @brief whether add new line for content of a json array, true by default
     */
    zfbool jsonArrayAddNewLineForContent;
    /**
     * @brief whether trim element tag in same line for a json array,
     *   valid only if jsonArrayAddNewLineForContent is true,
     *   true by default
     */
    zfbool jsonArrayContentTagInSameLine;
    /**
     * @brief whether add new line for content of a json array, true by default
     */
    zfbool jsonArrayTagInSameLineIfNoContent;

public:
    /** @cond ZFPrivateDoc */
    ZFJsonOutputFlags(void)
    : jsonGlobalLineBeginToken()
    , jsonObjectAddNewLineForContent(zftrue)
    , jsonObjectTagInSameLineIfNoContent(zftrue)
    , jsonArrayAddNewLineForContent(zftrue)
    , jsonArrayContentTagInSameLine(zftrue)
    , jsonArrayTagInSameLineIfNoContent(zftrue)
    {
    }
    /** @endcond */
};
extern ZF_ENV_EXPORT const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsDefault();
extern ZF_ENV_EXPORT const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsTrim();
/**
 * @brief default output flags for #ZFJsonToOutput
 */
#define ZFJsonOutputFlagsDefault _ZFP_ZFJsonOutputFlagsDefault()
/**
 * @brief trim output flags for #ZFJsonToOutput
 */
#define ZFJsonOutputFlagsTrim _ZFP_ZFJsonOutputFlagsTrim()

// ============================================================
zfclassFwd _ZFP_ZFJsonItemPrivate;
/**
 * @brief JSON item
 */
zfclassLikePOD ZF_ENV_EXPORT ZFJsonItem
{
    // ============================================================
private:
    void _ZFP_ZFJsonItemOnDelete(void);
public:
    /** @cond ZFPrivateDoc */
    ZFJsonItem(ZF_IN _ZFP_ZFJsonItemPrivate *d);
    /** @endcond */
    /** @brief construct a null item */
    ZFJsonItem(void);
    /** @brief construct with specified type */
    ZFJsonItem(ZF_IN ZFJsonTypeEnum jsonType);
    /** @brief retain from other item */
    ZFJsonItem(ZF_IN const ZFJsonItem &ref);
    /** @brief retain from other item */
    ZFJsonItem(ZF_IN const ZFJsonItem *ref);
    virtual ~ZFJsonItem(void);

    /** @cond ZFPrivateDoc */
    ZFJsonItem &operator = (ZF_IN const ZFJsonItem &ref);
    ZFJsonItem &operator = (ZF_IN const ZFJsonItem *ref);
    zfbool operator == (ZF_IN const ZFJsonItem &ref) const;
    zfbool operator == (ZF_IN const ZFJsonItem *ref) const;
    inline operator ZFJsonItem *(void) {return this;}
    inline operator const ZFJsonItem *(void) const {return this;}
    inline ZFJsonItem *operator ->(void) {return this;}
    inline const ZFJsonItem *operator ->(void) const {return this;}
    /** @endcond */

    // ============================================================
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

public:
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const;

    // ============================================================
public:
    /**
     * @brief for impl to store additional data, null to remove
     */
    void implTagSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value);
    /**
     * @brief see #implTagSet
     */
    const zfchar *implTag(ZF_IN const zfchar *key) const;

    // ============================================================
public:
    /**
     * @brief type of this item
     */
    ZFJsonTypeEnum jsonType(void) const;

    /**
     * @brief access parent of this node, return true if parent exists
     */
    zfbool jsonParent(ZF_OUT ZFJsonItem &ret) const;

    // ============================================================
    // for value type
public:
    /**
     * @brief value of the node, valid only for value type
     */
    void jsonValueSet(ZF_IN const zfchar *value);
    /**
     * @brief see #jsonValueSet
     */
    const zfchar *jsonValue(void) const;

    // ============================================================
public:
    /**
     * @brief return a copy of this node as well as all of its content
     *
     * the newly created tree has no parent
     */
    ZFJsonItem jsonCloneTree(void) const;

    // ============================================================
    // for object type
public:
    /**
     * @brief get json item for key
     */
    ZFJsonItem *jsonItem(ZF_IN const zfchar *key) const;
    /**
     * @brief set json item for key
     */
    void jsonItemSet(ZF_IN const zfchar *key,
                     ZF_IN ZFJsonItem *jsonItem);

    /**
     * @brief util method to access json value, return a null type item if no such item or not value type
     */
    const zfchar *jsonItemValue(ZF_IN const zfchar *key) const;
    /**
     * @brief util method to add a json value item
     */
    void jsonItemValueSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value);

    /**
     * @brief remove json item
     */
    void jsonItemRemove(ZF_IN const zfchar *key);
    /**
     * @brief remove all json item
     */
    void jsonItemRemoveAll(void);

public:
    /**
     * @brief json item count
     */
    zfindex jsonItemCount(void) const;

public:
    /** @brief see #zfiterator */
    zfiterator jsonItemIterator(void) const;
    /** @brief see #zfiterator */
    zfiterator jsonItemIteratorForKey(ZF_IN const zfchar *key) const;
    /** @brief see #zfiterator */
    zfbool jsonItemIteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zfbool jsonItemIteratorIsEqual(ZF_IN const zfiterator &it0,
                                   ZF_IN const zfiterator &it1) const;
    /** @brief see #zfiterator */
    void jsonItemIteratorSet(ZF_IN_OUT zfiterator &it, ZF_IN ZFJsonItem *jsonItem);
    /** @brief see #zfiterator */
    void jsonItemIteratorRemove(ZF_IN_OUT zfiterator &it);
    /** @brief see #zfiterator */
    const zfchar *jsonItemIteratorGetKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    ZFJsonItem *jsonItemIteratorGet(ZF_IN const zfiterator &it);
    /** @brief see #zfiterator */
    const ZFJsonItem *jsonItemIteratorGet(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    const zfchar *jsonItemIteratorNextKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    ZFJsonItem *jsonItemIteratorNext(ZF_IN_OUT zfiterator &it);
    /** @brief see #zfiterator */
    const ZFJsonItem *jsonItemIteratorNext(ZF_IN_OUT zfiterator &it) const;

    // ============================================================
    // for array type
public:
    /**
     * @brief json object count
     */
    zfindex jsonObjectCount(void) const;
    /**
     * @brief get json object at index
     */
    ZFJsonItem *jsonObjectAtIndex(ZF_IN zfindex index);
    /**
     * @brief get json object at index
     */
    const ZFJsonItem *jsonObjectAtIndex(ZF_IN zfindex index) const;

public:
    /**
     * @brief add json object
     */
    void jsonObjectAdd(ZF_IN ZFJsonItem *jsonObject);
    /**
     * @brief add json object to specified index (ranged in [0, count])
     */
    void jsonObjectAdd(ZF_IN ZFJsonItem *jsonObject,
                       ZF_IN zfindex atIndexOrZFIndexMax);
    /**
     * @brief remove json object
     */
    void jsonObjectRemove(ZF_IN ZFJsonItem *jsonObject);
    /**
     * @brief remove json object at index
     */
    void jsonObjectRemoveAtIndex(ZF_IN zfindex index);
    /**
     * @brief remove all json object
     */
    void jsonObjectRemoveAll(void);
    /**
     * @brief find json object
     */
    zfindex jsonObjectFind(ZF_IN const ZFJsonItem *jsonObject) const;

    // ============================================================
    // quick access
public:
    /** @cond ZFPrivateDoc */
    // value
    operator const zfchar *(void) const {return this->jsonValue();}
    // object
    inline ZFJsonItem *operator [] (ZF_IN const zfchar *key) {return this->jsonItem(key);}
    inline const ZFJsonItem *operator [] (ZF_IN const zfchar *key) const {return this->jsonItem(key);}
    // array
    inline ZFJsonItem *operator [] (ZF_IN zfindex const &jsonObjectIndex) {return this->jsonObjectAtIndex(jsonObjectIndex);}
    inline const ZFJsonItem *operator [] (ZF_IN zfindex const &jsonObjectIndex) const {return this->jsonObjectAtIndex(jsonObjectIndex);}
    /** @endcond */

private:
    _ZFP_ZFJsonItemPrivate *d;
};

// ============================================================
/**
 * @brief parse json, or return an item with null type if fail
 */
extern ZF_ENV_EXPORT ZFJsonItem ZFJsonFromInput(ZF_IN const ZFInputCallback &input);
/**
 * @brief parse json, or return an item with null type if fail
 */
extern ZF_ENV_EXPORT ZFJsonItem ZFJsonFromString(ZF_IN const zfchar *src,
                                                 ZF_IN_OPT zfindex length = zfindexMax);

/**
 * @brief convert json to output
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
extern ZF_ENV_EXPORT zfbool ZFJsonToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                                           ZF_IN const ZFJsonItem *jsonItem,
                                           ZF_IN_OPT const ZFJsonOutputFlags &outputFlags = ZFJsonOutputFlagsDefault);
/**
 * @brief convert json to string
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
inline zfbool ZFJsonToString(ZF_IN_OUT zfstring &ret,
                             ZF_IN const ZFJsonItem *jsonItem,
                             ZF_IN_OPT const ZFJsonOutputFlags &outputFlags = ZFJsonOutputFlagsDefault)
{
    return ZFJsonToOutput(ZFOutputCallbackForString(ret), jsonItem, outputFlags);
}
/** @brief see #ZFJsonToString */
inline zfstring ZFJsonToString(ZF_IN const ZFJsonItem *jsonItem,
                               ZF_IN_OPT const ZFJsonOutputFlags &outputFlags = ZFJsonOutputFlagsDefault)
{
    zfstring ret;
    ZFJsonToString(ret, jsonItem, outputFlags);
    return ret;
}

// ============================================================
// escape chars
/**
 * @brief encode json chars
 *
 * @code
 *   \\b        \b
 *   \\t        \t
 *   \\n        \n
 *   \\f        \f
 *   \\r        \r
 *   \"         "
 *   \\         \
 * @endcode
 *
 * result would be appended to dst
 */
extern ZF_ENV_EXPORT void ZFJsonEscapeCharEncode(ZF_OUT zfstring &dst,
                                                 ZF_IN const zfchar *src,
                                                 ZF_IN_OPT zfindex count = zfindexMax);
/**
 * @brief see #ZFJsonEscapeCharEncode
 */
extern ZF_ENV_EXPORT void ZFJsonEscapeCharEncode(ZF_OUT const ZFOutputCallback &dst,
                                                 ZF_IN const zfchar *src,
                                                 ZF_IN_OPT zfindex count = zfindexMax);

/**
 * @brief see #ZFJsonEscapeCharEncode
 */
extern ZF_ENV_EXPORT void ZFJsonEscapeCharDecode(ZF_OUT zfstring &dst,
                                                 ZF_IN const zfchar *src,
                                                 ZF_IN_OPT zfindex count = zfindexMax);
/**
 * @brief see #ZFJsonEscapeCharEncode
 */
extern ZF_ENV_EXPORT void ZFJsonEscapeCharDecode(ZF_OUT const ZFOutputCallback &dst,
                                                 ZF_IN const zfchar *src,
                                                 ZF_IN_OPT zfindex count = zfindexMax);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJson_h_

#include "ZFJsonHandle.h"
#include "ZFJsonSerializableConverterDef.h"

