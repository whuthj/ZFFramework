/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListAdapterArray.h
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */

#ifndef _ZFI_ZFUIListAdapterArray_h_
#define _ZFI_ZFUIListAdapterArray_h_

#include "ZFUIListAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */
zfclass ZF_ENV_EXPORT ZFUIListAdapterArray : zfextends ZFObject, zfimplements ZFUIListAdapter
{
    ZFOBJECT_DECLARE(ZFUIListAdapterArray, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListAdapter)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        d = zfAllocInternal(ZFArrayEditable);
        return this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfReleaseInternal(d);
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->listCellRemoveAll();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief directly access internal cell array,
     *   you may modify it directly, with caution
     */
    virtual inline ZFArrayEditable *listCellArray(void)
    {
        return d;
    }
    /**
     * @brief add cell
     */
    virtual inline void listCellAdd(ZF_IN ZFUIListCell *cell)
    {
        d->add(cell);
    }
    /**
     * @brief add cell at index
     */
    virtual inline void listCellAdd(ZF_IN zfindex index,
                                    ZF_IN ZFUIListCell *cell)
    {
        d->add(index, cell);
    }
    /**
     * @brief remove cell
     */
    virtual inline void listCellRemove(ZF_IN zfindex index,
                                       ZF_IN_OPT zfindex count = 1)
    {
        d->remove(index, count);
    }
    /**
     * @brief remove cell
     */
    virtual inline void listCellRemoveElement(ZF_IN ZFUIListCell *cell)
    {
        d->removeElement(cell);
    }
    /**
     * @brief remove all cell
     */
    virtual inline void listCellRemoveAll(void)
    {
        d->removeAll();
    }

public:
    /**
     * @brief whether to override default list cell size hint, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, listCellSizeHintOverride)
    /**
     * @brief valid only if #listCellSizeHintOverride, -1 by default, see #ZFUIListAdapter::listCellSizeAtIndex
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, listCellSizeHint, ZFPropertyInitValue(-1))

public:
    zfoverride
    virtual zfindex listCellCount(void)
    {
        return d->count();
    }
    zfoverride
    virtual zfautoObject listCellCacheOnAccess(ZF_IN zfindex index)
    {
        return zfautoObjectCreateWithLeakTest(d->get(index));
    }
    zfoverride
    virtual zfautoObject listCellAtIndex(ZF_IN zfindex index)
    {
        return zfautoObjectCreateWithLeakTest(d->get(index));
    }
    zfoverride
    virtual zfint listCellSizeAtIndex(ZF_IN zfindex index,
                                      ZF_IN ZFUIListCell *cell)
    {
        if(this->listCellSizeHintOverride())
        {
            return this->listCellSizeHint();
        }
        else
        {
            return zfsuperI(ZFUIListAdapter)::listCellSizeAtIndex(index, cell);
        }
    }

private:
    ZFArrayEditable *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapterArray_h_

