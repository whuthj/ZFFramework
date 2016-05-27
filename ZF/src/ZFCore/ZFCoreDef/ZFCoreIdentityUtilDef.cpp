/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreIdentityUtilDef.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIdentityGeneratorPrivate
{
public:
    zfidentity cur;
    zfstlmap<zfidentity, zfbool> used;

public:
    _ZFP_ZFIdentityGeneratorPrivate(void)
    : cur(zfidentityInvalid)
    , used()
    {
        this->used[zfidentityInvalid] = zftrue;
    }
};

// ============================================================
ZFIdentityGenerator::ZFIdentityGenerator(void)
{
    d = zfnew(_ZFP_ZFIdentityGeneratorPrivate);
}
ZFIdentityGenerator::~ZFIdentityGenerator(void)
{
    zfdelete(d);
}

zfidentity ZFIdentityGenerator::current(void)
{
    return d->cur;
}
zfidentity ZFIdentityGenerator::next(void)
{
    do
    {
        ++(d->cur);
    } while(d->used.find(d->cur) != d->used.end());
    return d->cur;
}
zfidentity ZFIdentityGenerator::nextMarkUsed(void)
{
    do
    {
        ++(d->cur);
    } while(d->used.find(d->cur) != d->used.end());
    d->used[d->cur] = zftrue;
    return d->cur;
}
zfbool ZFIdentityGenerator::used(ZF_IN zfidentity identity)
{
    return (d->used.find(identity) != d->used.end());
}
void ZFIdentityGenerator::markUsed(ZF_IN zfidentity identity)
{
    d->used[identity] = zftrue;
}
void ZFIdentityGenerator::markUnused(ZF_IN zfidentity identity)
{
    d->used.erase(identity);
}

void ZFIdentityGenerator::removeAll(void)
{
    d->used.clear();
    d->used[zfidentityInvalid] = zftrue;
}

void ZFIdentityGenerator::allUsed(ZF_IN_OUT ZFCoreArray<zfidentity> &buf)
{
    buf.capacitySet(buf.capacity() + (zfindex)d->used.size());
    for(zfstlmap<zfidentity, zfbool>::iterator it = d->used.begin(); it != d->used.end(); ++it)
    {
        buf.add(it->first);
    }
}

// ============================================================
zfidentity zfidentityCalcString(ZF_IN const zfchar *start)
{
    zfidentity hash = 0;
    if(start)
    {
        while(*start)
        {
            hash = hash * 131 + (*start++);
        }
    }
    return hash;
}
zfidentity zfidentityCalcBuf(ZF_IN const void *start, ZF_IN zfindex byteSize)
{
    const zfbyte *p = (const zfbyte *)start;
    const zfbyte *pEnd = p + byteSize;
    zfidentity hash = 0;
    if(p)
    {
        while(p < pEnd)
        {
            hash = hash * 131 + (*p++);
        }
    }
    return hash;
}

zfidentity zfidentityCalcPointer(ZF_IN const void *p)
{
    return zfidentityCalcBuf(&p, sizeof(const void *));
}
zfidentity zfidentityCalcPointer(ZF_IN ZFFuncAddrType p)
{
    return zfidentityCalcBuf(&p, sizeof(ZFFuncAddrType));
}
zfidentity zfidentityCalcPointer(ZF_IN ZFMemberFuncAddrType p)
{
    return zfidentityCalcBuf(&p, sizeof(ZFMemberFuncAddrType));
}

ZF_NAMESPACE_GLOBAL_END

