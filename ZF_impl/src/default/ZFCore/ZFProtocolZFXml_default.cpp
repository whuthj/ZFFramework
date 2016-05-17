/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFXml.h"
#include "ZFCore/ZFString.h"

#include "../../_repo/pugixml/pugixml.hpp"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFXmlImpl_default, ZFXml, ZFProtocolLevelDefault)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("pugixml"))
public:
    virtual ZFXmlItem xmlParse(ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex size = zfindexMax,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
    {
        pugi::xml_document implXmlDoc;
        pugi::xml_parse_result implResult = implXmlDoc.load_buffer(src, (size == zfindexMax) ? zfslen(src) : size, pugi::parse_full);
        if(implResult.status != pugi::status_ok)
        {
            return ZFXmlItem();
        }
        ZFXmlItem doc(ZFXmlType::e_XmlDocument);
        this->translateChildren(implXmlDoc, doc);
        return doc;
    }
    virtual ZFXmlItem xmlParse(ZF_IN const ZFInputCallback &inputCallback,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault)
    {
        if(!inputCallback.callbackIsValid())
        {
            return ZFXmlItem();
        }
        ZFBuffer buf = ZFInputCallbackReadToBuffer(inputCallback);
        if(buf.buffer() == zfnull)
        {
            return ZFXmlItem();
        }

        pugi::xml_document implXmlDoc;
        pugi::xml_parse_result implResult = implXmlDoc.load_buffer_inplace(buf.buffer(), buf.bufferSize(), pugi::parse_full);
        if(implResult.status != pugi::status_ok)
        {
            return ZFXmlItem();
        }
        ZFXmlItem doc(ZFXmlType::e_XmlDocument);
        this->translateChildren(implXmlDoc, doc);
        return doc;
    }
private:
    void translateAttribute(ZF_IN const pugi::xml_node &implXmlItem,
                            ZF_OUT ZFXmlItem *zfXmlItem)
    {
        pugi::xml_attribute implXmlAttribute = implXmlItem.first_attribute();
        while(implXmlAttribute)
        {
            zfXmlItem->xmlAttributeAdd(implXmlAttribute.name(), implXmlAttribute.value());

            implXmlAttribute = implXmlAttribute.next_attribute();
        }
    }
    void translateChildren(ZF_IN const pugi::xml_node &implXmlItem,
                           ZF_OUT ZFXmlItem *zfXmlItem)
    {
        pugi::xml_node implXmlChild = implXmlItem.first_child();
        while(implXmlChild != zfnull)
        {
            switch(implXmlChild.type())
            {
                case pugi::node_document:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
                case pugi::node_element:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlElement);
                    this->translateAttribute(implXmlChild, zfXmlChild);
                    this->translateChildren(implXmlChild, zfXmlChild);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pcdata:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlText);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlChild.xmlValueSet(implXmlChild.value());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_cdata:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlText);
                    zfXmlChild.xmlTextCDATASet(zftrue);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlChild.xmlValueSet(implXmlChild.value());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_comment:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlComment);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlChild.xmlValueSet(implXmlChild.value());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_declaration:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlDeclaration);
                    this->translateAttribute(implXmlChild, zfXmlChild);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlChild.xmlValueSet(implXmlChild.value());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_doctype:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlDOCTYPE);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlChild.xmlValueSet(implXmlChild.value());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                case pugi::node_pi:
                {
                    ZFXmlItem zfXmlChild(ZFXmlType::e_XmlProcessingInstruction);

                    zfXmlChild.xmlNameSet(implXmlChild.name());
                    zfXmlChild.xmlValueSet(implXmlChild.value());
                    zfXmlItem->xmlChildAdd(zfXmlChild);
                    break;
                }
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }

            implXmlChild = implXmlChild.next_sibling();
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFXmlImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFXmlImpl_default)

ZF_NAMESPACE_GLOBAL_END

