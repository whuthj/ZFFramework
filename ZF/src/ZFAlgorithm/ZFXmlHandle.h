/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFXmlHandle.h
 * @brief xml handler for ZFXml
 */

#ifndef _ZFI_ZFXmlHandle_h_
#define _ZFI_ZFXmlHandle_h_

#include "ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFXmlHandle
/**
 * @brief util class to make accessing ZFXml tree more easily
 *
 * usage:
 * @code
 *   // create by passing a xml item
 *   ZFXmlHandle handle(xmlItem);
 *
 *   // go through as you like, no need to worry about whether the item exists
 *   handle = handle.xmlChildFirst().xmlSiblingNext();
 *
 *   // safely to cast to desired type, null if not exist or not the type
 *   ZFXmlItem *xmlElement = handle.toXmlElement();
 *   ZFXmlItem *xmlText = handle.toXmlText();
 *
 *   // access name and value safely,
 *   // return null if not the desired type or the type have no type or name
 *   const zfchar *name = handle.xmlName();
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFXmlHandle
{
public:
    /** @cond ZFPrivateDoc */
    ZFXmlHandle(void) : d(zfnull) {}
    ZFXmlHandle(ZF_IN ZFXmlItem *xmlItem) : d(xmlItem) {}
    ZFXmlHandle(ZF_IN const ZFXmlHandle &ref) : d(ref.d) {}
    ~ZFXmlHandle(void) {}
    ZFXmlHandle &operator = (ZF_IN const ZFXmlHandle &ref) {d = ref.d; return *this;}
    ZFXmlHandle &operator = (ZF_IN ZFXmlItem *xmlItem) {d = xmlItem; return *this;}
    operator ZFXmlItem *(void) const {return d;}
    /** @endcond */

    // ============================================================
    // conversion
public:
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlItem(void) const
    {
        return d;
    }

public:
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlElement(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlElement) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlAttribute(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlAttribute) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlText(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlText) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlComment(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlComment) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlDocument(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlDocument) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlDeclaration(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlDeclaration) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlDOCTYPE(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlDOCTYPE) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFXmlItem *toXmlProcessingInstruction(void) const
    {
        return ((d && d->xmlType() == ZFXmlType::e_XmlProcessingInstruction) ? d : zfnull);
    }

    // ============================================================
    // access
public:
    /** @brief see #ZFXmlHandle */
    const zfchar *xmlName(void) const
    {
        return (d ? d->xmlName() : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    const zfchar *xmlValue(void) const
    {
        return (d ? d->xmlValue() : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlSiblingNext(ZF_IN_OPT const zfchar *name = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlSiblingNext(name) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlSiblingPrev(ZF_IN_OPT const zfchar *name = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlSiblingPrev(name) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlSiblingElementNext(ZF_IN_OPT const zfchar *name = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlSiblingElementNext(name) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlSiblingElementPrev(ZF_IN_OPT const zfchar *name = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlSiblingElementPrev(name) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlChildFirst(ZF_IN_OPT const zfchar *name = zfnull,
                              ZF_IN_OPT ZFXmlItem *afterThis = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlChildFirst(name, afterThis) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlChildLast(ZF_IN_OPT const zfchar *name = zfnull,
                             ZF_IN_OPT ZFXmlItem *beforeThis = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlChildLast(name, beforeThis) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlChildElementFirst(ZF_IN_OPT const zfchar *name = zfnull,
                                     ZF_IN_OPT ZFXmlItem *afterThis = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlChildElementFirst(name, afterThis) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlChildElementLast(ZF_IN_OPT const zfchar *name = zfnull,
                                    ZF_IN_OPT ZFXmlItem *beforeThis = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlChildElementLast(name, beforeThis) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlAttributeFirst(ZF_IN_OPT const zfchar *name = zfnull,
                                  ZF_IN_OPT ZFXmlItem *afterThis = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlAttributeFirst(name, afterThis) : zfnull);
    }
    /** @brief see #ZFXmlHandle */
    ZFXmlHandle xmlAttributeLast(ZF_IN_OPT const zfchar *name = zfnull,
                                 ZF_IN_OPT ZFXmlItem *beforeThis = zfnull) const
    {
        return ZFXmlHandle(d ? d->xmlAttributeLast(name, beforeThis) : zfnull);
    }

private:
    ZFXmlItem *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXmlHandle_h_

