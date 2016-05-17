/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFAlgorithm.hh"
#include "ZFTextTemplate.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static const zfchar *_ZFP_ZFTextTemplate_header = zfText("{ZFTT_");
static const zfindex _ZFP_ZFTextTemplate_headerSize = zfslen(_ZFP_ZFTextTemplate_header);
static void _ZFP_ZFTextTemplateApply_replace(ZF_IN const ZFTextTemplateParam &param,
                                             ZF_IN const ZFOutputCallback &output,
                                             ZF_IN const zfchar *pEnd,
                                             ZF_IN_OUT const zfchar *&data,
                                             ZF_IN_OUT const zfchar *&p,
                                             ZF_IN_OUT zfindex &size);
static void _ZFP_ZFTextTemplateApply_enableIf(ZF_IN const ZFTextTemplateParam &param,
                                              ZF_IN const ZFOutputCallback &output,
                                              ZF_IN const zfchar *pEnd,
                                              ZF_IN_OUT const zfchar *&data,
                                              ZF_IN_OUT const zfchar *&p,
                                              ZF_IN_OUT zfindex &size,
                                              ZF_IN_OUT zfindex &condCount);

zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                            ZF_IN const ZFOutputCallback &output,
                            ZF_IN const ZFInputCallback &input)
{
    ZFBuffer buffer = ZFInputCallbackReadToBuffer(input);
    if(buffer.buffer() == zfnull)
    {
        return zfindexMax;
    }
    return ZFTextTemplateApply(param, output, buffer.bufferAsString(), buffer.bufferSize());
}

zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                            ZF_IN const ZFOutputCallback &output,
                            ZF_IN const zfchar *data,
                            ZF_IN_OPT zfindex dataSize /* = zfindexMax */)
{
    if(!output.callbackIsValid() || data == zfnull)
    {
        return zfindexMax;
    }

    zfindex condCount = 0;
    zfindex size = 0;
    const zfchar *p = data;
    const zfchar *pEnd = (data + ((dataSize == zfindexMax) ? zfslen(data) : dataSize));
    do
    {
        if(p >= pEnd)
        {
            if(p > data)
            {
                size += p - data;
                output.execute(data, p - data);
                data = p;
            }
            break;
        }
        if(!(
                (zfindex)(pEnd - p) > _ZFP_ZFTextTemplate_headerSize
                && zfsncmp(p, _ZFP_ZFTextTemplate_header, _ZFP_ZFTextTemplate_headerSize) == 0
            ))
        {
            ++p;
            continue;
        }
        if(p > data)
        {
            size += p - data;
            output.execute(data, p - data);
            data = p;
        }

        switch(p[_ZFP_ZFTextTemplate_headerSize])
        {
            case 'R':
                _ZFP_ZFTextTemplateApply_replace(param, output, pEnd, data, p, size);
                break;
            case 'C':
                _ZFP_ZFTextTemplateApply_enableIf(param, output, pEnd, data, p, size, condCount);
                break;
            default:
                ++p;
                break;
        }
    } while(zftrue);
    return size;
}

// ============================================================
static zfindex _ZFP_ZFTextTemplateApply_keyLength(ZF_IN const zfchar *pEnd,
                                                  ZF_IN const zfchar *p)
{
    if(*p == ' ' || *p == '\t' || *p == '}')
    {
        return zfindexMax;
    }
    const zfchar *t = p;
    while(t < pEnd && *t != '}') {++t;}
    if(t >= pEnd)
    {
        return zfindexMax;
    }
    return (t - p);
}
// ============================================================
static void _ZFP_ZFTextTemplateApply_replace(ZF_IN const ZFTextTemplateParam &param,
                                             ZF_IN const ZFOutputCallback &output,
                                             ZF_IN const zfchar *pEnd,
                                             ZF_IN_OUT const zfchar *&data,
                                             ZF_IN_OUT const zfchar *&p,
                                             ZF_IN_OUT zfindex &size)
{ // {ZFTT_R_myKey}
    p += _ZFP_ZFTextTemplate_headerSize + 1;
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax)
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    zfstring *value = param.replaceDatas.get<zfstring *>(key);
    if(value == zfnull)
    {
        return ;
    }
    output.execute(value->cString(), value->length());
    size += value->length();
    data = p;
}
static void _ZFP_ZFTextTemplateApply_enableIf(ZF_IN const ZFTextTemplateParam &param,
                                              ZF_IN const ZFOutputCallback &output,
                                              ZF_IN const zfchar *pEnd,
                                              ZF_IN_OUT const zfchar *&data,
                                              ZF_IN_OUT const zfchar *&p,
                                              ZF_IN_OUT zfindex &size,
                                              ZF_IN_OUT zfindex &condCount)
{ // {ZFTT_C_myCond}...{ZFTT_CE}
    p += _ZFP_ZFTextTemplate_headerSize + 1;
    if(pEnd - p >= 2 && p[0] == 'E' && p[1] == '}')
    {
        p += 2;
        if(condCount > 0)
        {
            --condCount;
            data = p;
        }
        return ;
    }
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax)
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    data = p;
    zfbool *valueTmp = param.enableIfDatas.get<zfbool *>(key);
    zfbool value = (valueTmp ? *valueTmp : param.enableIfByDefault);
    if(value)
    {
        ++condCount;
        return ;
    }

    zfindex condCountOffset = 0;
    do
    {
        if(p >= pEnd)
        {
            data = p;
            break;
        }
        if(!(
                (zfindex)(pEnd - p) > _ZFP_ZFTextTemplate_headerSize
                && zfsncmp(p, _ZFP_ZFTextTemplate_header, _ZFP_ZFTextTemplate_headerSize) == 0
            ))
        {
            ++p;
            continue;
        }

        p += _ZFP_ZFTextTemplate_headerSize;
        if(*p != 'C')
        {
            continue;
        }
        ++p;
        if(pEnd - p >= 2 && p[0] == 'E' && p[1] == '}')
        {
            p += 2;
            if(condCountOffset == 0)
            {
                data = p;
                break;
            }
            else
            {
                --condCountOffset;
                continue;
            }
        }
        if(*p != '_')
        {
            continue;
        }
        ++p;
        zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
        if(keySize == zfindexMax)
        {
            continue;
        }
        ++condCountOffset;
        p += keySize + 1;
    } while(zftrue);
}

ZF_NAMESPACE_GLOBAL_END

