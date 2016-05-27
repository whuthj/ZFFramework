/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFXml.h"
#include "protocol/ZFProtocolZFXml.h"
#include "protocol/ZFProtocolZFXmlEscapeChar.h"

#include "../ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "../ZFCore/ZFSTLWrapper/zfstl_deque.h"
#include "../ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFXmlType)
ZFENUM_DEFINE(ZFXmlVisitType)

// ============================================================
const ZFXmlParseFlags ZFXmlParseFlagsDefault = {
};

static ZFXmlOutputFlags _ZFP_ZFXmlOutputFlagsDefaultInstance;
const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsDefault(void)
{
    return _ZFP_ZFXmlOutputFlagsDefaultInstance;
}
static ZFXmlOutputFlags _ZFP_ZFXmlOutputFlagsTrimInstance;
const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsTrim(void)
{
    return _ZFP_ZFXmlOutputFlagsTrimInstance;
}
static ZFXmlOutputFlags _ZFP_ZFXmlOutputFlagsDetailedInstance;
const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsDetailed(void)
{
    return _ZFP_ZFXmlOutputFlagsDetailedInstance;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFXmlOutputFlagsInit, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFXmlOutputFlagsTrimInstance.xmlElementAttributeCountBeforeAddNewLine = zfindexMax;
    _ZFP_ZFXmlOutputFlagsTrimInstance.xmlElementTrimTagIfNoChildren = zftrue;
    _ZFP_ZFXmlOutputFlagsTrimInstance.xmlToken.xmlNewLineToken.removeAll();
    _ZFP_ZFXmlOutputFlagsTrimInstance.xmlToken.xmlIndentToken.removeAll();

    _ZFP_ZFXmlOutputFlagsDetailedInstance.xmlElementAddNewLineAtHeadIfNotSingleLine = zftrue;
    _ZFP_ZFXmlOutputFlagsDetailedInstance.xmlElementAttributeCountBeforeAddNewLine = 1;
    _ZFP_ZFXmlOutputFlagsDetailedInstance.xmlElementEndTagAtSameLineIfNoChildElement = zftrue;
}
ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFXmlOutputFlagsInit)

// ============================================================
static zfbool _ZFP_ZFXmlOutputElementUseSingleTag(ZF_IN const ZFXmlItem *element,
                                                  ZF_IN const ZFXmlOutputFlags &flags,
                                                  ZF_IN const ZFXmlVisitData &data)
{
    return (flags.xmlElementTrimTagIfNoChildren && element->xmlChildFirst() == zfnull);
}
static zfbool _ZFP_ZFXmlOutputElementAttributeNeedNewLine(ZF_IN const ZFXmlItem *element,
                                                          ZF_IN const ZFXmlOutputFlags &flags,
                                                          ZF_IN const ZFXmlVisitData &data)
{
    if(element == zfnull)
    {
        return zffalse;
    }
    const ZFXmlItem *xmlAttribute = element->xmlAttributeFirst();
    if(flags.xmlElementAttributeCountBeforeAddNewLine == zfindexMax)
    {
        return zffalse;
    }
    if(flags.xmlElementAttributeCountBeforeAddNewLine == 0)
    {
        return (xmlAttribute != zfnull);
    }
    zfindex xmlAttributeCount = 0;
    while(xmlAttribute != zfnull)
    {
        ++xmlAttributeCount;
        if(xmlAttributeCount > flags.xmlElementAttributeCountBeforeAddNewLine)
        {
            return zftrue;
        }
        xmlAttribute = xmlAttribute->xmlAttributeNext();
    }
    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputElementChildNeedNewLine(ZF_IN const ZFXmlItem *xmlChild,
                                                      ZF_IN const ZFXmlOutputFlags &flags,
                                                      ZF_IN const ZFXmlVisitData &data)
{
    if(flags.xmlElementTrimTagIfNoChildren && xmlChild == zfnull)
    {
        return zffalse;
    }
    if(!flags.xmlElementEndTagAtSameLineIfNoChildElement)
    {
        return zftrue;
    }

    if(xmlChild->xmlType() != ZFXmlType::e_XmlText)
    {
        return zftrue;
    }
    for(const ZFXmlItem *t = xmlChild->xmlSiblingNext(); t != zfnull; t = t->xmlSiblingNext())
    {
        if(t->xmlType() != ZFXmlType::e_XmlText)
        {
            return zftrue;
        }
    }
    for(const ZFXmlItem *t = xmlChild->xmlSiblingPrev(); t != zfnull; t = t->xmlSiblingPrev())
    {
        if(t->xmlType() != ZFXmlType::e_XmlText)
        {
            return zftrue;
        }
    }

    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputAttributeNeedNewLine(ZF_IN const ZFXmlItem *xmlAttribute,
                                                   ZF_IN const ZFXmlOutputFlags &flags,
                                                   ZF_IN const ZFXmlVisitData &data)
{
    return (flags.xmlElementAttributeCountBeforeAddNewLine == 0
        || (flags.xmlElementAttributeCountBeforeAddNewLine != zfindexMax
            && data.siblingIndex > 0
            && (data.siblingIndex % flags.xmlElementAttributeCountBeforeAddNewLine) == 0));
}
zfclass _ZFP_ZFXmlOutputOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFXmlOutputOwner, ZFObject)

public:
    ZFOutputCallback outputCallback;
    ZFXmlOutputFlags flags;

public:
    ZFMETHOD_MEMBER_DECLARE_1(zfbool, onVisit,
                              const ZFXmlVisitData &, data)
    {
        switch(data.xmlItem->xmlType())
        {
            case ZFXmlType::e_XmlElement:
                return this->onVisitXmlElement(data);
            case ZFXmlType::e_XmlAttribute:
                return this->onVisitXmlAttribute(data);
            case ZFXmlType::e_XmlText:
                return this->onVisitXmlText(data);
            case ZFXmlType::e_XmlComment:
                return this->onVisitXmlComment(data);
            case ZFXmlType::e_XmlDocument:
                return this->onVisitXmlDocument(data);
            case ZFXmlType::e_XmlDeclaration:
                return this->onVisitXmlDeclaration(data);
            case ZFXmlType::e_XmlDOCTYPE:
                return this->onVisitXmlDOCTYPE(data);
            case ZFXmlType::e_XmlProcessingInstruction:
                return this->onVisitXmlProcessingInstruction(data);
            default:
                return zffalse;
        }
    }
private:
    zfbool onVisitXmlElement(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                if((data.depth > 0 && data.siblingIndex > 0) && flags.xmlElementAddNewLineAtHeadIfNotSingleLine
                    && (_ZFP_ZFXmlOutputElementAttributeNeedNewLine(data.xmlItem, flags, data)
                        || _ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem->xmlChildFirst(), flags, data)))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                }
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                if(_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlElementSingleTagLeft);
                }
                else
                {
                    this->add(flags.xmlToken.xmlElementBeginTagLeft);
                }
                this->add(data.xmlItem->xmlName());
                break;
            case ZFXmlVisitType::e_Exit:
                if(data.xmlItem->xmlAttributeFirst() != zfnull)
                {
                    this->add(zfText(" "));
                }
                if(_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlElementSingleTagRight);
                }
                else
                {
                    this->add(flags.xmlToken.xmlElementBeginTagRight);
                }
                break;
            case ZFXmlVisitType::e_ExitChildren:
                if(!_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem->xmlChildFirst(), flags, data)
                        || _ZFP_ZFXmlOutputElementAttributeNeedNewLine(data.xmlItem, flags, data))
                    {
                        this->add(flags.xmlToken.xmlNewLineToken);
                        this->add(flags.xmlGlobalLineBeginToken);
                        this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                    }
                    this->add(flags.xmlToken.xmlElementEndTagLeft);
                    this->add(data.xmlItem->xmlName());
                    this->add(flags.xmlToken.xmlElementEndTagRight);
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlAttribute(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
            {
                if(_ZFP_ZFXmlOutputAttributeNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                }
                else if(data.depth > 0 || data.siblingIndex > 0)
                {
                        this->add(zfText(" "));
                }
                this->add(data.xmlItem->xmlName());
                this->add(flags.xmlToken.xmlAttributeEqualTag);
                if(flags.xmlAttributeUseSingleQuote)
                {
                    this->add(flags.xmlToken.xmlAttributeSingleQuoteTagLeft);
                    ZFXmlEscapeCharEncode(this->outputCallback, data.xmlItem->xmlValue());
                    this->add(flags.xmlToken.xmlAttributeSingleQuoteTagRight);
                }
                else
                {
                    this->add(flags.xmlToken.xmlAttributeQuoteTagLeft);
                    ZFXmlEscapeCharEncode(this->outputCallback, data.xmlItem->xmlValue());
                    this->add(flags.xmlToken.xmlAttributeQuoteTagRight);
                }
            }
                break;
            case ZFXmlVisitType::e_Exit:
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlText(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                }
                if(data.xmlItem->xmlTextCDATA())
                {
                    this->add(flags.xmlToken.xmlTextCDATATagLeft);
                }
                this->add(data.xmlItem->xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                if(data.xmlItem->xmlTextCDATA())
                {
                    this->add(flags.xmlToken.xmlTextCDATATagRight);
                }
                if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth - 1);
                }
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlComment(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
            {
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlCommentTagLeft);
                this->add(data.xmlItem->xmlValue());
                break;
            }
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlCommentTagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlDocument(ZF_IN const ZFXmlVisitData &data)
    {
        return zftrue;
    }
    zfbool onVisitXmlDeclaration(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlDeclarationTagLeft);
                break;
            case ZFXmlVisitType::e_Exit:
            {
                if(data.xmlItem->xmlAttributeFirst() == zfnull)
                {
                    this->add(zfText(" "));
                }
                this->add(flags.xmlToken.xmlDeclarationTagRight);
            }
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlDOCTYPE(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlDOCTYPETagLeft);
                this->add(zfText(" "));
                this->add(data.xmlItem->xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlDOCTYPETagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlProcessingInstruction(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlPITagLeft);
                this->add(data.xmlItem->xmlName());
                this->add(zfText(" "));
                this->add(data.xmlItem->xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlPITagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }

private:
    inline void add(const zfchar *s)
    {
        if(s != zfnull)
        {
            this->outputCallback.execute(s);
        }
    }
    void addIndent(const zfchar *xmlIndentToken, zfindex indentLevel = 1)
    {
        if(xmlIndentToken != zfnull && *xmlIndentToken != '\0')
        {
            for(zfindex i = 0; i < indentLevel; ++i)
            {
                this->outputCallback.execute(xmlIndentToken);
            }
        }
    }
};

ZFXmlVisitCallback _ZFP_ZFXmlVisitCallbackForOutput(ZF_IN const zfcharA *ownerFilePath,
                                                    ZF_IN const zfcharA *ownerFunctionName,
                                                    ZF_IN zfindex ownerFileLine,
                                                    ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
                                                    ZF_IN_OPT const ZFXmlOutputFlags &flags /* = ZFXmlOutputFlagsDefault */)
{
    if(!outputCallback.callbackIsValid())
    {
        return ZFCallbackNullDetail(ownerFilePath, ownerFunctionName, ownerFileLine);
    }
    _ZFP_ZFXmlOutputOwner *owner = zfAlloc(_ZFP_ZFXmlOutputOwner);
    owner->outputCallback = outputCallback;
    owner->flags = flags;
    ZFXmlVisitCallback callback = ZFCallbackForMemberMethodDetail(
        owner, ZFMethodAccessClassMember(_ZFP_ZFXmlOutputOwner, onVisit),
        ownerFilePath, ownerFunctionName, ownerFileLine);
    callback.callbackOwnerObjectRetain();
    zfRelease(owner);
    return callback;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFXmlVisitCallbackDefaultDataHolder, ZFLevelZFFrameworkNormal)
{
    xmlVisitCallbackDefault = ZFXmlVisitCallbackForOutput();
}
ZFXmlVisitCallback xmlVisitCallbackDefault;
ZF_GLOBAL_INITIALIZER_END(ZFXmlVisitCallbackDefaultDataHolder)
#define _ZFP_ZFXml_xmlVisitCallbackDefault (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFXmlVisitCallbackDefaultDataHolder)->xmlVisitCallbackDefault)

ZFXmlVisitCallback &_ZFP_ZFXmlVisitCallbackDefault(void)
{
    return _ZFP_ZFXml_xmlVisitCallbackDefault;
}

// ============================================================
typedef zfstlmap<zfstlstringZ, zfstring> _ZFP_ZFXmlItemImplTagMapType;
typedef zfstlmap<_ZFP_ZFXmlItemPrivate *, ZFXmlItem *> _ZFP_ZFXmlItemHolderType;
zfclassNotPOD _ZFP_ZFXmlItemPrivate
{
public:
    zfindex refCount;
    _ZFP_ZFXmlItemImplTagMapType implTag;
    ZFXmlTypeEnum xmlType;
    _ZFP_ZFXmlItemPrivate *xmlParent;
    zfstlstringZ xmlName;
    zfstlstringZ xmlValue;

    /*
     * used to hold reference for attribute and child
     */
    _ZFP_ZFXmlItemHolderType xmlAttributeHolder;
    _ZFP_ZFXmlItemHolderType xmlChildHolder;

    ZFXmlItem *xmlAttributeFirst;
    ZFXmlItem *xmlAttributeLast;
    ZFXmlItem *xmlChildFirst;
    ZFXmlItem *xmlChildLast;

    ZFXmlItem *xmlAttributePrev;
    ZFXmlItem *xmlAttributeNext;
    ZFXmlItem *xmlSiblingPrev;
    ZFXmlItem *xmlSiblingNext;

    zfbool xmlAttributeNeedSort;
    zfbool xmlTextCDATA;

public:
    _ZFP_ZFXmlItemPrivate(void)
    : refCount(1)
    , implTag()
    , xmlType(ZFXmlType::e_XmlNull)
    , xmlParent(zfnull)
    , xmlName()
    , xmlValue()
    , xmlAttributeHolder()
    , xmlChildHolder()
    , xmlAttributeFirst(zfnull)
    , xmlAttributeLast(zfnull)
    , xmlChildFirst(zfnull)
    , xmlChildLast(zfnull)
    , xmlAttributePrev(zfnull)
    , xmlAttributeNext(zfnull)
    , xmlSiblingPrev(zfnull)
    , xmlSiblingNext(zfnull)
    , xmlAttributeNeedSort(zftrue)
    , xmlTextCDATA(zffalse)
    {
    }
};

static void _ZFP_ZFXmlCopyNode(ZF_IN ZFXmlItem *to, ZF_IN const ZFXmlItem *from)
{
    to->xmlNameSet(from->xmlName());
    to->xmlValueSet(from->xmlValue());
    switch(from->xmlType())
    {
        case ZFXmlType::e_XmlText:
        {
            to->xmlTextCDATASet(from->xmlTextCDATA());
            break;
        }
        case ZFXmlType::e_XmlElement:
        case ZFXmlType::e_XmlDeclaration:
        {
            const ZFXmlItem *fromAttribute = from->xmlAttributeFirst();
            while(fromAttribute != zfnull)
            {
                to->xmlAttributeAdd(fromAttribute->xmlClone());
                fromAttribute = fromAttribute->xmlAttributeNext();
            }
            break;
        }
        default:
            break;
    }
}
static void _ZFP_ZFXmlCopyTree(ZF_IN ZFXmlItem *to, ZF_IN const ZFXmlItem *from)
{
    _ZFP_ZFXmlCopyNode(to, from);

    switch(from->xmlType())
    {
        case ZFXmlType::e_XmlElement:
        {
            const ZFXmlItem *fromChild = from->xmlChildFirst();
            while(fromChild != zfnull)
            {
                to->xmlChildAdd(fromChild->xmlCloneTree());
                fromChild = fromChild->xmlSiblingNext();
            }
            break;
        }
        case ZFXmlType::e_XmlDocument:
        {
            const ZFXmlItem *fromChild = from->xmlChildFirst();
            while(fromChild != zfnull)
            {
                to->xmlChildAdd(fromChild->xmlCloneTree());
                fromChild = fromChild->xmlSiblingNext();
            }
            break;
        }
        default:
            break;
    }
}

#define _ZFP_ZFXmlAssertCanHaveAttribute(item) \
    zfCoreAssert(item->xmlType() == ZFXmlType::e_XmlElement || item->xmlType() == ZFXmlType::e_XmlDeclaration)
#define _ZFP_ZFXmlAssertCanHaveChild(item) \
    zfCoreAssert(item->xmlType() == ZFXmlType::e_XmlElement || item->xmlType() == ZFXmlType::e_XmlDocument)
#define _ZFP_ZFXmlAssertCanHaveSiblingAttribute(item) \
    zfCoreAssert(item->xmlType() == ZFXmlType::e_XmlAttribute)
#define _ZFP_ZFXmlAssertCanHaveSibling(item) \
    zfCoreAssert(item->xmlType() != ZFXmlType::e_XmlAttribute && item->xmlType() != ZFXmlType::e_XmlNull)
#define _ZFP_ZFXmlAssertNullOrChildAttribute(item) \
    zfCoreAssert(item == zfnull || item->xmlType() == ZFXmlType::e_XmlAttribute)
#define _ZFP_ZFXmlAssertNullOrChild(item) \
    zfCoreAssert(item == zfnull || (item->xmlType() != ZFXmlType::e_XmlAttribute && item->xmlType() != ZFXmlType::e_XmlNull))

// ============================================================
// ZFXmlItem
void ZFXmlItem::_ZFP_ZFXmlItemOnDelete(void)
{
    this->xmlAttributeRemoveAll();
    this->xmlChildRemoveAll();

    zfdelete(d);
    d = zfnull;
}

/** @cond ZFPrivateDoc */
ZFXmlItem::ZFXmlItem(ZF_IN _ZFP_ZFXmlItemPrivate *d)
: d(d)
{
    ++(d->refCount);
}
/** @endcond */
ZFXmlItem::ZFXmlItem(void)
: d(zfnew(_ZFP_ZFXmlItemPrivate))
{
}
ZFXmlItem::ZFXmlItem(ZF_IN ZFXmlTypeEnum xmlType)
: d(zfnew(_ZFP_ZFXmlItemPrivate))
{
    d->xmlType = xmlType;
}
ZFXmlItem::ZFXmlItem(ZF_IN const ZFXmlItem &ref)
{
    d = ref.d;
    ++(d->refCount);
}
ZFXmlItem::ZFXmlItem(ZF_IN const ZFXmlItem *ref)
{
    if(ref == zfnull)
    {
        d = zfnew(_ZFP_ZFXmlItemPrivate);
    }
    else
    {
        d = ref->d;
        ++(d->refCount);
    }
}
ZFXmlItem::~ZFXmlItem(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        this->_ZFP_ZFXmlItemOnDelete();
    }
}

/** @cond ZFPrivateDoc */
ZFXmlItem &ZFXmlItem::operator = (ZF_IN const ZFXmlItem &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        this->_ZFP_ZFXmlItemOnDelete();
    }
    d = ref.d;
    return *this;
}
ZFXmlItem &ZFXmlItem::operator = (ZF_IN const ZFXmlItem *ref)
{
    if(ref == zfnull)
    {
        this->operator = (ZFXmlItem());
    }
    else
    {
        this->operator = (*ref);
    }
    return *this;
}
zfbool ZFXmlItem::operator == (ZF_IN const ZFXmlItem &ref) const
{
    return (d == ref.d || (d->xmlType == ZFXmlType::e_XmlNull && ref.d->xmlType == ZFXmlType::e_XmlNull));
}
zfbool ZFXmlItem::operator == (ZF_IN const ZFXmlItem *ref) const
{
    if(ref == zfnull)
    {
        return (d->xmlType == ZFXmlType::e_XmlNull);
    }
    else
    {
        return (d == ref->d);
    }
}
/** @endcond */

// ============================================================
void ZFXmlItem::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += ZFXmlType::EnumNameForValue(this->xmlType());
    if(this->xmlName() != zfnull)
    {
        ret += zfText(", name: ");
        ret += this->xmlName();
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

zfindex ZFXmlItem::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
void ZFXmlItem::implTagSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    zfCoreAssert(this->xmlType() != ZFXmlType::e_XmlNull);
    if(key == zfnull)
    {
        return ;
    }
    if(value == zfnull)
    {
        d->implTag.erase(key);
    }
    else
    {
        _ZFP_ZFXmlItemImplTagMapType::iterator it = d->implTag.find(key);
        if(it != d->implTag.end())
        {
            it->second = value;
        }
        else
        {
            d->implTag[key] = value;
        }
    }
}
const zfchar *ZFXmlItem::implTag(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFXmlItemImplTagMapType::iterator it = d->implTag.find(key);
        if(it != d->implTag.end())
        {
            return it->second;
        }
    }
    return zfnull;
}

// ============================================================
ZFXmlTypeEnum ZFXmlItem::xmlType(void) const
{
    return d->xmlType;
}

zfbool ZFXmlItem::xmlParent(ZF_OUT ZFXmlItem &ret) const
{
    if(d->xmlParent != zfnull)
    {
        ret = ZFXmlItem(d->xmlParent);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

void ZFXmlItem::xmlNameSet(ZF_IN const zfchar *name)
{
    zfCoreAssert(this->xmlType() != ZFXmlType::e_XmlNull);
    d->xmlName = name;
}
const zfchar *ZFXmlItem::xmlName(void) const
{
    return (d->xmlName.empty() ? zfnull : d->xmlName.c_str());
}

void ZFXmlItem::xmlValueSet(ZF_IN const zfchar *value)
{
    zfCoreAssert(this->xmlType() != ZFXmlType::e_XmlNull);
    d->xmlValue = value;
}
const zfchar *ZFXmlItem::xmlValue(void) const
{
    return (d->xmlValue.empty() ? zfnull : d->xmlValue.c_str());
}

// ============================================================
static inline ZFXmlVisitData _ZFP_ZFXmlVisitDataCreate(ZF_IN const ZFXmlItem *xmlItem,
                                                       ZF_IN ZFXmlVisitTypeEnum xmlVisitType,
                                                       ZF_IN zfindex depth,
                                                       ZF_IN zfindex siblingIndex)
{
    ZFXmlVisitData data = {xmlItem, xmlVisitType, depth, siblingIndex};
    return data;
}
void ZFXmlItem::xmlVisit(ZF_IN const ZFXmlVisitCallback &callback /* = ZFXmlVisitCallbackForOutput() */) const
{
    if(!callback.callbackIsValid() || this->xmlType() == ZFXmlType::e_XmlNull)
    {
        return ;
    }

    ZFCoreArrayPOD<ZFXmlVisitData> datas;
    datas.add(_ZFP_ZFXmlVisitDataCreate(this, ZFXmlVisitType::e_Enter, 0, 0));

    while(datas.count() > 0)
    {
        ZFXmlVisitData data = datas.getLast();
        datas.removeLast();
        if(data.xmlVisitType == ZFXmlVisitType::e_Enter)
        {
            if(callback.execute(data))
            {
                ZFXmlTypeEnum xmlType = data.xmlItem->xmlType();
                if(xmlType == ZFXmlType::e_XmlElement || xmlType == ZFXmlType::e_XmlDocument)
                {
                    if(data.xmlItem->xmlType() == ZFXmlType::e_XmlElement)
                    {
                        datas.add(_ZFP_ZFXmlVisitDataCreate(data.xmlItem, ZFXmlVisitType::e_ExitChildren, data.depth, data.siblingIndex));
                    }

                    const ZFXmlItem *xmlChild = data.xmlItem->xmlChildLast();
                    if(xmlChild != zfnull)
                    {
                        zfindex xmlChildDepth = ((data.xmlItem->xmlType() == ZFXmlType::e_XmlDocument)
                            ? data.depth
                            : data.depth + 1);
                        zfindex startIndex = ((zfindex)datas.count()) - 1;
                        do
                        {
                            datas.add(_ZFP_ZFXmlVisitDataCreate(xmlChild, ZFXmlVisitType::e_Enter, xmlChildDepth, 0));
                            xmlChild = xmlChild->xmlSiblingPrev();
                        } while(xmlChild != zfnull);
                        for(zfindex i = ((zfindex)datas.count()) - 1, xmlChildIndex = 0;
                            i != startIndex;
                            --i, ++xmlChildIndex)
                        {
                            datas[i].siblingIndex = xmlChildIndex;
                        }
                    }
                }

                datas.add(_ZFP_ZFXmlVisitDataCreate(data.xmlItem, ZFXmlVisitType::e_Exit, data.depth, data.siblingIndex));

                if(xmlType == ZFXmlType::e_XmlElement || xmlType == ZFXmlType::e_XmlDeclaration)
                {
                    const ZFXmlItem *xmlAttribute = data.xmlItem->xmlAttributeLast();
                    if(xmlAttribute != zfnull)
                    {
                        zfindex xmlAttributeDepth = data.depth + 1;
                        zfindex startIndex = ((zfindex)datas.count()) - 1;
                        do
                        {
                            datas.add(_ZFP_ZFXmlVisitDataCreate(xmlAttribute, ZFXmlVisitType::e_Enter, xmlAttributeDepth, 0));
                            xmlAttribute = xmlAttribute->xmlAttributePrev();
                        } while(xmlAttribute != zfnull);
                        for(zfindex i = ((zfindex)datas.count()) - 1, xmlChildIndex = 0;
                            i != startIndex;
                            --i, ++xmlChildIndex)
                        {
                            datas[i].siblingIndex = xmlChildIndex;
                        }
                    }
                }
            }
        }
        else
        {
            if(!callback.execute(data))
            {
                for(zfindex i = ((zfindex)datas.count()) - 1; i != zfindexMax; --i)
                {
                    if(datas[i].depth == data.depth)
                    {
                        datas.remove(i);
                    }
                }
            }
        }
    }
}

// ============================================================
ZFXmlItem ZFXmlItem::xmlClone(void) const
{
    ZFXmlItem ret(this->xmlType());
    if(this->xmlType() != ZFXmlType::e_XmlNull)
    {
        _ZFP_ZFXmlCopyNode(&ret, this);
    }
    return ret;
}
ZFXmlItem ZFXmlItem::xmlCloneTree(void) const
{
    ZFXmlItem ret(this->xmlType());
    if(this->xmlType() != ZFXmlType::e_XmlNull)
    {
        _ZFP_ZFXmlCopyTree(&ret, this);
    }
    return ret;
}
zfbool ZFXmlItem::xmlIsEqual(ZF_IN const ZFXmlItem *anotherNode) const
{
    if(anotherNode == zfnull)
    {
        return zffalse;
    }
    if(anotherNode->d == d)
    {
        return zftrue;
    }
    if(anotherNode->xmlType() != this->xmlType()
        || !zfscmpTheSame(this->xmlName(), anotherNode->xmlName())
        || !zfscmpTheSame(this->xmlValue(), anotherNode->xmlValue()))
    {
        return zffalse;
    }
    if(this->xmlType() == ZFXmlType::e_XmlElement || this->xmlType() == ZFXmlType::e_XmlDeclaration)
    {
        const ZFXmlItem *thisAttribute = this->xmlAttributeFirst();
        const ZFXmlItem *anotherAttribute = anotherNode->xmlAttributeFirst();
        while(thisAttribute != zfnull && anotherAttribute != zfnull
            && thisAttribute->xmlIsEqual(anotherAttribute))
        {
            thisAttribute = thisAttribute->xmlAttributeNext();
            anotherAttribute = anotherAttribute->xmlAttributeNext();
        }
        if(thisAttribute != zfnull || anotherAttribute != zfnull)
        {
            return zffalse;
        }
    }
    return zftrue;
}
zfbool ZFXmlItem::xmlIsEqualTree(ZF_IN const ZFXmlItem *anotherNode) const
{
    if(anotherNode == zfnull)
    {
        return zffalse;
    }
    if(anotherNode->d == d)
    {
        return zftrue;
    }
    if(!this->xmlIsEqual(anotherNode))
    {
        return zffalse;
    }
    if(this->xmlType() == ZFXmlType::e_XmlElement || this->xmlType() == ZFXmlType::e_XmlDocument)
    {
        const ZFXmlItem *selfChild = this->xmlChildFirst();
        const ZFXmlItem *anotherChild = anotherNode->xmlChildFirst();
        while(selfChild != zfnull
            && anotherChild != zfnull
            && selfChild->xmlIsEqualTree(anotherChild))
        {
            selfChild = selfChild->xmlSiblingNext();
            anotherChild = anotherChild->xmlSiblingNext();
        }
        if(selfChild != zfnull || anotherChild != zfnull)
        {
            return zffalse;
        }
    }
    return zftrue;
}

// ============================================================
void ZFXmlItem::xmlAttributeAdd(ZF_IN ZFXmlItem *addThis,
                                ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */)
{
    _ZFP_ZFXmlAssertCanHaveAttribute(this);
    if(addThis == zfnull)
    {
        return ;
    }
    zfCoreAssertWithMessage(addThis->d->xmlParent == zfnull, zfTextA("adding a attribute that already has parent, remove it first"));
    addThis = zfnew(ZFXmlItem, *addThis);
    d->xmlAttributeHolder[addThis->d] = addThis;
    d->xmlAttributeNeedSort = zftrue;
    addThis->d->xmlParent = d;

    if(beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() == ZFXmlType::e_XmlAttribute)
    {
        if(beforeThis->d->xmlAttributePrev != zfnull)
        {
            addThis->d->xmlAttributePrev = beforeThis->d->xmlAttributePrev;
            beforeThis->d->xmlAttributePrev->d->xmlAttributeNext = addThis;
        }
        addThis->d->xmlAttributeNext = beforeThis;
        beforeThis->d->xmlAttributePrev = addThis;
    }
    else
    {
        if(d->xmlAttributeLast != zfnull)
        {
            d->xmlAttributeLast->d->xmlAttributeNext = addThis;
        }
        addThis->d->xmlAttributePrev = d->xmlAttributeLast;
    }

    if(addThis->d->xmlAttributePrev == zfnull)
    {
        d->xmlAttributeFirst = addThis;
    }
    if(addThis->d->xmlAttributeNext == zfnull)
    {
        d->xmlAttributeLast = addThis;
    }
}
void ZFXmlItem::xmlAttributeAdd(ZF_IN const zfchar *key,
                                ZF_IN const zfchar *value,
                                ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */)
{
    if(!zfscmpTheSame(key, zfText("")))
    {
        ZFXmlItem xmlAttribute(ZFXmlType::e_XmlAttribute);
        xmlAttribute.xmlNameSet(key);
        xmlAttribute.xmlValueSet(value);
        this->xmlAttributeAdd(&xmlAttribute, beforeThis);
    }
}
void ZFXmlItem::xmlAttributeRemove(ZF_IN ZFXmlItem *removeThis)
{
    if(removeThis == zfnull || removeThis->d->xmlParent != d || removeThis->xmlType() != ZFXmlType::e_XmlAttribute)
    {
        return ;
    }

    d->xmlAttributeHolder.erase(removeThis->d);

    d->xmlAttributeNeedSort = zftrue;
    removeThis->d->xmlParent = zfnull;

    if(removeThis->d->xmlAttributePrev != zfnull)
    {
        removeThis->d->xmlAttributePrev->d->xmlAttributeNext = removeThis->d->xmlAttributeNext;
    }
    if(removeThis->d->xmlAttributeNext != zfnull)
    {
        removeThis->d->xmlAttributeNext->d->xmlAttributePrev = removeThis->d->xmlAttributePrev;
    }
    if(d->xmlAttributeFirst == removeThis)
    {
        d->xmlAttributeFirst = removeThis->d->xmlAttributeNext;
    }
    if(d->xmlAttributeLast == removeThis)
    {
        d->xmlAttributeLast = removeThis->d->xmlAttributePrev;
    }
    zfdelete(removeThis);
}
void ZFXmlItem::xmlAttributeRemoveAll(ZF_IN const zfchar *name /* = zfnull */)
{
    if(d->xmlAttributeFirst == zfnull)
    {
        return ;
    }
    if(name == zfnull)
    {
        d->xmlAttributeFirst = zfnull;
        d->xmlAttributeLast = zfnull;
        for(_ZFP_ZFXmlItemHolderType::iterator it = d->xmlAttributeHolder.begin(); it != d->xmlAttributeHolder.end(); ++it)
        {
            it->second->d->xmlParent = zfnull;
            it->second->d->xmlAttributePrev = zfnull;
            it->second->d->xmlAttributeNext = zfnull;
            zfdelete(it->second);
        }
        d->xmlAttributeHolder.clear();
        return ;
    }

    ZFXmlItem *xmlAttribute = d->xmlAttributeFirst;
    while(xmlAttribute != zfnull)
    {
        if(name != zfnull && !zfscmpTheSame(xmlAttribute->xmlName(), name))
        {
            xmlAttribute = xmlAttribute->xmlAttributeNext();
            continue;
        }

        ZFXmlItem *tmp = xmlAttribute;
        xmlAttribute = xmlAttribute->xmlAttributeNext();
        this->xmlAttributeRemove(tmp);
    }
}

ZFXmlItem *ZFXmlItem::xmlAttributeFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                        ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */)
{
    ZFXmlItem *xmlAttribute = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() == ZFXmlType::e_XmlAttribute)
        ? xmlAttribute = afterThis->xmlAttributeNext()
        : d->xmlAttributeFirst;

    if(name == zfnull)
    {
        return xmlAttribute;
    }
    else
    {
        while(xmlAttribute != zfnull && !zfscmpTheSame(xmlAttribute->xmlName(), name))
        {
            xmlAttribute = xmlAttribute->d->xmlAttributeNext;
        }
        return xmlAttribute;
    }
}
const ZFXmlItem *ZFXmlItem::xmlAttributeFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                              ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */) const
{
    ZFXmlItem *xmlAttribute = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() == ZFXmlType::e_XmlAttribute)
        ? xmlAttribute = afterThis->xmlAttributeNext()
        : d->xmlAttributeFirst;

    if(name == zfnull)
    {
        return xmlAttribute;
    }
    else
    {
        while(xmlAttribute != zfnull && !zfscmpTheSame(xmlAttribute->xmlName(), name))
        {
            xmlAttribute = xmlAttribute->d->xmlAttributeNext;
        }
        return xmlAttribute;
    }
}
ZFXmlItem *ZFXmlItem::xmlAttributeLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                       ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */)
{
    ZFXmlItem *xmlAttribute = (beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() == ZFXmlType::e_XmlAttribute)
        ? beforeThis->xmlAttributePrev()
        : d->xmlAttributeLast;

    if(name == zfnull)
    {
        return xmlAttribute;
    }
    else
    {
        while(xmlAttribute != zfnull && !zfscmpTheSame(xmlAttribute->xmlName(), name))
        {
            xmlAttribute = xmlAttribute->d->xmlAttributePrev;
        }
        return xmlAttribute;
    }
}
const ZFXmlItem *ZFXmlItem::xmlAttributeLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                             ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */) const
{
    ZFXmlItem *xmlAttribute = (beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() == ZFXmlType::e_XmlAttribute)
        ? beforeThis->xmlAttributePrev()
        : d->xmlAttributeLast;

    if(name == zfnull)
    {
        return xmlAttribute;
    }
    else
    {
        while(xmlAttribute != zfnull && !zfscmpTheSame(xmlAttribute->xmlName(), name))
        {
            xmlAttribute = xmlAttribute->d->xmlAttributePrev;
        }
        return xmlAttribute;
    }
}
const zfchar *ZFXmlItem::xmlAttributeValueFirst(ZF_IN const zfchar *key,
                                                ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */) const
{
    const ZFXmlItem *xmlAttribute = this->xmlAttributeFirst(key, afterThis);
    if(xmlAttribute != zfnull)
    {
        return xmlAttribute->xmlValue();
    }
    return zfnull;
}
const zfchar *ZFXmlItem::xmlAttributeValueLast(ZF_IN const zfchar *key,
                                               ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */) const
{
    const ZFXmlItem *xmlAttribute = this->xmlAttributeLast(key, beforeThis);
    if(xmlAttribute != zfnull)
    {
        return xmlAttribute->xmlValue();
    }
    return zfnull;
}

ZFXmlItem *ZFXmlItem::xmlAttributeNext(ZF_IN_OPT const zfchar *name /* = zfnull */)
{
    if(name == zfnull)
    {
        return d->xmlAttributeNext;
    }
    else
    {
        ZFXmlItem *xmlAttributeNext = d->xmlAttributeNext;
        while(xmlAttributeNext != zfnull && !zfscmpTheSame(xmlAttributeNext->xmlName(), name))
        {
            xmlAttributeNext = xmlAttributeNext->d->xmlAttributeNext;
        }
        return xmlAttributeNext;
    }
}
const ZFXmlItem *ZFXmlItem::xmlAttributeNext(ZF_IN_OPT const zfchar *name /* = zfnull */) const
{
    if(name == zfnull)
    {
        return d->xmlAttributeNext;
    }
    else
    {
        ZFXmlItem *xmlAttributeNext = d->xmlAttributeNext;
        while(xmlAttributeNext != zfnull && !zfscmpTheSame(xmlAttributeNext->xmlName(), name))
        {
            xmlAttributeNext = xmlAttributeNext->d->xmlAttributeNext;
        }
        return xmlAttributeNext;
    }
}
ZFXmlItem *ZFXmlItem::xmlAttributePrev(ZF_IN_OPT const zfchar *name /* = zfnull */)
{
    if(name == zfnull)
    {
        return d->xmlAttributePrev;
    }
    else
    {
        ZFXmlItem *xmlAttributePrev = d->xmlAttributePrev;
        while(xmlAttributePrev != zfnull && !zfscmpTheSame(xmlAttributePrev->xmlName(), name))
        {
            xmlAttributePrev = xmlAttributePrev->d->xmlAttributePrev;
        }
        return xmlAttributePrev;
    }
}
const ZFXmlItem *ZFXmlItem::xmlAttributePrev(ZF_IN_OPT const zfchar *name /* = zfnull */) const
{
    if(name == zfnull)
    {
        return d->xmlAttributePrev;
    }
    else
    {
        ZFXmlItem *xmlAttributePrev = d->xmlAttributePrev;
        while(xmlAttributePrev != zfnull && !zfscmpTheSame(xmlAttributePrev->xmlName(), name))
        {
            xmlAttributePrev = xmlAttributePrev->d->xmlAttributePrev;
        }
        return xmlAttributePrev;
    }
}

// ============================================================
static ZFCompareResult _ZFP_ZFXmlHasAttributeSortComparer(ZF_IN ZFXmlItem * const &v0, ZF_IN ZFXmlItem * const &v1)
{
    zfint cmpResult = zfscmp(v0->xmlName(), v1->xmlName());
    if(cmpResult < 0)
    {
        return ZFCompareSmaller;
    }
    else if(cmpResult == 0)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareGreater;
    }
}

void ZFXmlItem::xmlAttributeSort(void)
{
    if(d->xmlAttributeNeedSort)
    {
        d->xmlAttributeNeedSort = zffalse;
        ZFXmlItem *xmlAttribute = d->xmlAttributeFirst;
        if(xmlAttribute != zfnull)
        {
            ZFCoreArrayPOD<ZFXmlItem *> tmp;
            while(xmlAttribute != zfnull)
            {
                tmp.add(xmlAttribute);
                xmlAttribute = xmlAttribute->d->xmlAttributeNext;
            }
            tmp.sort(_ZFP_ZFXmlHasAttributeSortComparer);

            d->xmlAttributeFirst = tmp[0];
            d->xmlAttributeLast = tmp[tmp.count() - 1];

            ZFXmlItem *prev = zfnull;
            for(zfindex i = 0; i < tmp.count(); ++i)
            {
                xmlAttribute = tmp[i];
                xmlAttribute->d->xmlAttributePrev = prev;
                if(prev != zfnull)
                {
                    prev->d->xmlAttributeNext = xmlAttribute;
                }
                prev = xmlAttribute;
            }
            xmlAttribute->d->xmlAttributeNext = zfnull;
        }
    }
}
void ZFXmlItem::xmlAttributeSortRecursively(void)
{
    this->xmlAttributeSort();

    if(this->xmlType() == ZFXmlType::e_XmlElement || this->xmlType() == ZFXmlType::e_XmlDocument)
    {
        ZFXmlItem *xmlChild = this->xmlChildElementFirst();
        while(xmlChild != zfnull)
        {
            xmlChild->xmlAttributeSortRecursively();
            xmlChild = xmlChild->xmlSiblingNext();
        }
    }
}

// ============================================================
void ZFXmlItem::xmlChildAdd(ZF_IN ZFXmlItem *addThis,
                            ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */)
{
    _ZFP_ZFXmlAssertCanHaveChild(this);
    if(addThis == zfnull)
    {
        return ;
    }
    zfCoreAssertWithMessage(addThis->d->xmlParent == zfnull, zfTextA("adding a child that already has parent, remove it first"));
    addThis = zfnew(ZFXmlItem, *addThis);
    d->xmlChildHolder[addThis->d] = addThis;
    addThis->d->xmlParent = d;

    if(beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
    {
        if(beforeThis->d->xmlSiblingPrev != zfnull)
        {
            addThis->d->xmlSiblingPrev = beforeThis->d->xmlSiblingPrev;
            beforeThis->d->xmlSiblingPrev->d->xmlSiblingNext = addThis;
        }
        addThis->d->xmlSiblingNext = beforeThis;
        beforeThis->d->xmlSiblingPrev = addThis;
    }
    else
    {
        if(d->xmlChildLast != zfnull)
        {
            d->xmlChildLast->d->xmlSiblingNext = addThis;
        }
        addThis->d->xmlSiblingPrev = d->xmlChildLast;
    }

    if(addThis->d->xmlSiblingPrev == zfnull)
    {
        d->xmlChildFirst = addThis;
    }
    if(addThis->d->xmlSiblingNext == zfnull)
    {
        d->xmlChildLast = addThis;
    }
}
void ZFXmlItem::xmlChildRemove(ZF_IN ZFXmlItem *removeThis)
{
    if(removeThis == zfnull || removeThis->d->xmlParent != d || removeThis->xmlType() == ZFXmlType::e_XmlAttribute)
    {
        return ;
    }

    d->xmlChildHolder.erase(removeThis->d);

    removeThis->d->xmlParent = zfnull;

    if(removeThis->d->xmlSiblingPrev != zfnull)
    {
        removeThis->d->xmlSiblingPrev->d->xmlSiblingNext = removeThis->d->xmlSiblingNext;
    }
    if(removeThis->d->xmlSiblingNext != zfnull)
    {
        removeThis->d->xmlSiblingNext->d->xmlSiblingPrev = removeThis->d->xmlSiblingPrev;
    }
    if(d->xmlChildFirst == removeThis)
    {
        d->xmlChildFirst = removeThis->d->xmlSiblingNext;
    }
    if(d->xmlChildLast == removeThis)
    {
        d->xmlChildLast = removeThis->d->xmlSiblingPrev;
    }
    zfdelete(removeThis);
}
void ZFXmlItem::xmlChildRemoveAll(ZF_IN const zfchar *name /* = zfnull */)
{
    if(d->xmlChildFirst == zfnull)
    {
        return ;
    }
    if(name == zfnull)
    {
        d->xmlChildFirst = zfnull;
        d->xmlChildLast = zfnull;
        for(_ZFP_ZFXmlItemHolderType::iterator it = d->xmlChildHolder.begin(); it != d->xmlChildHolder.end(); ++it)
        {
            it->second->d->xmlParent = zfnull;
            it->second->d->xmlSiblingPrev = zfnull;
            it->second->d->xmlSiblingNext = zfnull;
            zfdelete(it->second);
        }
        d->xmlChildHolder.clear();
        return ;
    }

    ZFXmlItem *xmlChild = d->xmlChildFirst;
    while(xmlChild != zfnull)
    {
        if(name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->xmlSiblingNext();
            continue;
        }

        ZFXmlItem *tmp = xmlChild;
        xmlChild = xmlChild->xmlSiblingNext();
        this->xmlChildRemove(tmp);
    }
}

ZFXmlItem *ZFXmlItem::xmlChildFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                    ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */)
{
    ZFXmlItem *xmlChild = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? xmlChild = afterThis->xmlSiblingNext()
        : d->xmlChildFirst;

    if(name == zfnull)
    {
        return xmlChild;
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingNext;
        }
        return xmlChild;
    }
}
const ZFXmlItem *ZFXmlItem::xmlChildFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                          ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */) const
{
    ZFXmlItem *xmlChild = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? xmlChild = afterThis->xmlSiblingNext()
        : d->xmlChildFirst;

    if(name == zfnull)
    {
        return xmlChild;
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingNext;
        }
        return xmlChild;
    }
}
ZFXmlItem *ZFXmlItem::xmlChildLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                   ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */)
{
    ZFXmlItem *xmlChild = (beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? xmlChild = beforeThis->xmlSiblingPrev()
        : d->xmlChildLast;

    if(name == zfnull)
    {
        return xmlChild;
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingPrev;
        }
        return xmlChild;
    }
}
const ZFXmlItem *ZFXmlItem::xmlChildLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                         ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */) const
{
    ZFXmlItem *xmlChild = (beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? xmlChild = beforeThis->xmlSiblingPrev()
        : d->xmlChildLast;

    if(name == zfnull)
    {
        return xmlChild;
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingPrev;
        }
        return xmlChild;
    }
}
ZFXmlItem *ZFXmlItem::xmlChildElementFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                           ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */)
{
    ZFXmlItem *xmlChild = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? xmlChild = afterThis->xmlSiblingNext()
        : d->xmlChildFirst;

    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingNext;
    }
    return xmlChild;
}
const ZFXmlItem *ZFXmlItem::xmlChildElementFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                                 ZF_IN_OPT ZFXmlItem *afterThis /* = zfnull */) const
{
    ZFXmlItem *xmlChild = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? xmlChild = afterThis->xmlSiblingNext()
        : d->xmlChildFirst;

    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingNext;
    }
    return xmlChild;
}
ZFXmlItem *ZFXmlItem::xmlChildElementLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                          ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */)
{
    ZFXmlItem *xmlChild = (beforeThis != zfnull && beforeThis->d->xmlParent == d)
        ? beforeThis->d->xmlSiblingPrev
        : d->xmlChildLast;

    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingPrev;
    }
    return xmlChild;
}
const ZFXmlItem *ZFXmlItem::xmlChildElementLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                                ZF_IN_OPT ZFXmlItem *beforeThis /* = zfnull */) const
{
    ZFXmlItem *xmlChild = (beforeThis != zfnull && beforeThis->d->xmlParent == d)
        ? beforeThis->d->xmlSiblingPrev
        : d->xmlChildLast;

    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingPrev;
    }
    return xmlChild;
}

ZFXmlItem *ZFXmlItem::xmlSiblingNext(ZF_IN const zfchar *name /* = zfnull */)
{
    if(name == zfnull)
    {
        return d->xmlSiblingNext;
    }
    else
    {
        ZFXmlItem *xmlChild = d->xmlSiblingNext;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingNext;
        }
        return xmlChild;
    }
}
const ZFXmlItem *ZFXmlItem::xmlSiblingNext(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(name == zfnull)
    {
        return d->xmlSiblingNext;
    }
    else
    {
        ZFXmlItem *xmlChild = d->xmlSiblingNext;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingNext;
        }
        return xmlChild;
    }
}
ZFXmlItem *ZFXmlItem::xmlSiblingPrev(ZF_IN const zfchar *name /* = zfnull */)
{
    if(name == zfnull)
    {
        return d->xmlSiblingPrev;
    }
    else
    {
        ZFXmlItem *xmlChild = d->xmlSiblingPrev;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingPrev;
        }
        return xmlChild;
    }
}
const ZFXmlItem *ZFXmlItem::xmlSiblingPrev(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(name == zfnull)
    {
        return d->xmlSiblingPrev;
    }
    else
    {
        ZFXmlItem *xmlChild = d->xmlSiblingPrev;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))
        {
            xmlChild = xmlChild->d->xmlSiblingPrev;
        }
        return xmlChild;
    }
}
ZFXmlItem *ZFXmlItem::xmlSiblingElementNext(ZF_IN const zfchar *name /* = zfnull */)
{
    ZFXmlItem *xmlChild = d->xmlSiblingNext;
    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingNext;
    }
    return xmlChild;
}
const ZFXmlItem *ZFXmlItem::xmlSiblingElementNext(ZF_IN const zfchar *name /* = zfnull */) const
{
    ZFXmlItem *xmlChild = d->xmlSiblingNext;
    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingNext;
    }
    return xmlChild;
}
ZFXmlItem *ZFXmlItem::xmlSiblingElementPrev(ZF_IN const zfchar *name /* = zfnull */)
{
    ZFXmlItem *xmlChild = d->xmlSiblingPrev;
    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingPrev;
    }
    return xmlChild;
}
const ZFXmlItem *ZFXmlItem::xmlSiblingElementPrev(ZF_IN const zfchar *name /* = zfnull */) const
{
    ZFXmlItem *xmlChild = d->xmlSiblingPrev;
    while(xmlChild != zfnull
        && (xmlChild->xmlType() != ZFXmlType::e_XmlElement
            || (name != zfnull && !zfscmpTheSame(xmlChild->xmlName(), name))))
    {
        xmlChild = xmlChild->d->xmlSiblingPrev;
    }
    return xmlChild;
}

// ============================================================
void ZFXmlItem::xmlTextCDATASet(ZF_IN zfbool xmlTextCDATA)
{
    zfCoreAssert(this->xmlType() == ZFXmlType::e_XmlText);
    d->xmlTextCDATA = xmlTextCDATA;
}
zfbool ZFXmlItem::xmlTextCDATA(void) const
{
    return d->xmlTextCDATA;
}

// ============================================================
ZFXmlItem ZFXmlFromInput(ZF_IN const ZFInputCallback &callback,
                         ZF_IN_OPT const ZFXmlParseFlags &flags /* = ZFXmlParseFlagsDefault */)
{
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(callback, flags);
}
ZFXmlItem ZFXmlFromString(ZF_IN const zfchar *src,
                          ZF_IN_OPT zfindex size /* = zfindexMax */,
                          ZF_IN_OPT const ZFXmlParseFlags &flags /* = ZFXmlParseFlagsDefault */)
{
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(src, size, flags);
}

zfbool ZFXmlToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                     ZF_IN const ZFXmlItem *xmlItem,
                     ZF_IN_OPT const ZFXmlOutputFlags &outputFlags /* = ZFXmlOutputFlagsDefault */)
{
    if(!output.callbackIsValid() || xmlItem == zfnull)
    {
        return zffalse;
    }
    xmlItem->xmlVisit(ZFXmlVisitCallbackForOutput(output, outputFlags));
    return zftrue;
}

// ============================================================
// escape chars
void ZFXmlEscapeCharEncode(ZF_OUT zfstring &dst,
                           ZF_IN const zfchar *src,
                           ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFXmlEscapeCharEncode(ZFOutputCallbackForString(dst), src, count);
}
void ZFXmlEscapeCharEncode(ZF_OUT const ZFOutputCallback &dst,
                           ZF_IN const zfchar *src,
                           ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFPROTOCOL_ACCESS(ZFXmlEscapeChar)->xmlEscapeCharEncode(dst, src, count);
}

void ZFXmlEscapeCharDecode(ZF_OUT zfstring &dst,
                           ZF_IN const zfchar *src,
                           ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFXmlEscapeCharDecode(ZFOutputCallbackForString(dst), src, count);
}
void ZFXmlEscapeCharDecode(ZF_OUT const ZFOutputCallback &dst,
                           ZF_IN const zfchar *src,
                           ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFPROTOCOL_ACCESS(ZFXmlEscapeChar)->xmlEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END

