/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFJson.h"
#include "ZFCore/ZFString.h"

#include "../../_repo/rapidjson/document.h"
#include "../../_repo/rapidjson/reader.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFJsonImpl_default, ZFJson, ZFProtocolLevelDefault)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("rapidjson"))
public:
    virtual zfbool jsonParse(ZF_IN_OUT ZFJsonItem *jsonObject,
                             ZF_IN const zfchar *src,
                             ZF_IN_OPT zfindex size = zfindexMax)
    {
        rapidjson::Document implJsonDoc;
        implJsonDoc.Parse<rapidjson::kParseNumbersAsStringsFlag>(src, size);
        if(implJsonDoc.HasParseError())
        {
            return zffalse;
        }
        ZFJsonItem ret = this->jsonConvert(implJsonDoc);
        if(ret.jsonType() == ZFJsonType::e_JsonNull)
        {
            return zffalse;
        }
        *jsonObject = ret;
        return zftrue;
    }
    virtual zfbool jsonParse(ZF_IN_OUT ZFJsonItem *jsonObject,
                             ZF_IN const ZFInputCallback &inputCallback)
    {
        ZFBuffer buf = ZFInputCallbackReadToBuffer(inputCallback);
        if(buf.buffer() == zfnull)
        {
            return zffalse;
        }

        rapidjson::Document implJsonDoc;
        implJsonDoc.ParseInsitu<rapidjson::kParseNumbersAsStringsFlag>(buf.bufferAsString());
        if(implJsonDoc.HasParseError())
        {
            return zffalse;
        }
        ZFJsonItem ret = this->jsonConvert(implJsonDoc);
        if(ret.jsonType() == ZFJsonType::e_JsonNull)
        {
            return zffalse;
        }
        *jsonObject = ret;
        return zftrue;
    }
private:
    ZFJsonItem jsonConvert(ZF_IN const rapidjson::Value &implJsonItem)
    {
        /*
         kNullType = 0,      //!< null
         kFalseType = 1,     //!< false
         kTrueType = 2,      //!< true
         kObjectType = 3,    //!< object
         kArrayType = 4,     //!< array
         kStringType = 5,    //!< string
         kNumberType = 6     //!< number
         */
        switch(implJsonItem.GetType())
        {
            case rapidjson::kNullType:
                return ZFJsonItem();
            case rapidjson::kFalseType:
            case rapidjson::kTrueType:
            case rapidjson::kStringType:
            case rapidjson::kNumberType:
            {
                ZFJsonItem jsonValue(ZFJsonType::e_JsonValue);
                jsonValue.jsonValueSet(implJsonItem.GetString());
                return jsonValue;
            }
            case rapidjson::kArrayType:
            {
                ZFJsonItem jsonArray(ZFJsonType::e_JsonArray);
                for(rapidjson::Value::ConstValueIterator it = implJsonItem.Begin(); it != implJsonItem.End(); ++it)
                {
                    ZFJsonItem jsonChild = this->jsonConvert(*it);
                    if(jsonChild.jsonType() == ZFJsonType::e_JsonNull)
                    {
                        return ZFJsonItem();
                    }
                    jsonArray.jsonObjectAdd(jsonChild);
                }
                return jsonArray;
            }
            case rapidjson::kObjectType:
            {
                ZFJsonItem jsonObject(ZFJsonType::e_JsonObject);
                for(rapidjson::Value::ConstMemberIterator it = implJsonItem.MemberBegin(); it != implJsonItem.MemberEnd(); ++it)
                {
                    ZFJsonItem jsonChild = this->jsonConvert(it->value);
                    if(jsonChild.jsonType() == ZFJsonType::e_JsonNull)
                    {
                        return ZFJsonItem();
                    }
                    jsonObject.jsonItemSet(it->name.GetString(), jsonChild);
                }
                return jsonObject;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return ZFJsonItem();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFJsonImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFJsonImpl_default)

#undef min
#undef max

ZF_NAMESPACE_GLOBAL_END

