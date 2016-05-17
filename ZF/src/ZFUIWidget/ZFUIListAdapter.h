/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListAdapter.h
 * @brief abstract list adpater to supply cells for list view
 */

#ifndef _ZFI_ZFUIListAdapter_h_
#define _ZFI_ZFUIListAdapter_h_

#include "ZFUIListCell.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIListViewPrivate;
zfclassFwd ZFUIListView;
/**
 * @brief abstract list adpater to supply cells for list view
 */
zfinterface ZF_ENV_EXPORT ZFUIListAdapter : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIListAdapter, ZFInterface)

    // ============================================================
    // events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when update list cell,
     * param0 is the #ZFUIListCell,
     * param1 is the index of the list cell (as #ZFValue::indexValue)
     */
    ZFOBSERVER_EVENT(ListCellOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when recycle list cell,
     * param0 is the list cell
     */
    ZFOBSERVER_EVENT(ListCellCacheOnRecycle)

    // ============================================================
    // owner state
private:
    ZFUIOrientationEnum _ZFP_ZFUIListAdapter_listOrientation;
public:
    /**
     * @brief list's orientation
     */
    zffinal inline ZFUIOrientationEnum listOrientation(void)
    {
        return this->_ZFP_ZFUIListAdapter_listOrientation;
    }

private:
    ZFUISize _ZFP_ZFUIListAdapter_listContainerSize;
public:
    /**
     * @brief list container's size
     */
    zffinal inline const ZFUISize &listContainerSize(void)
    {
        return this->_ZFP_ZFUIListAdapter_listContainerSize;
    }

private:
    zfint _ZFP_ZFUIListAdapter_listCellSizeHint;
public:
    /**
     * @brief list cell's size hint
     */
    zffinal inline zfint listCellSizeHint(void)
    {
        return this->_ZFP_ZFUIListAdapter_listCellSizeHint;
    }

    // ============================================================
    // basic list cell access
public:
    /**
     * @brief cell count
     */
    virtual zfindex listCellCount(void) = 0;
    /**
     * @brief access list cell at index, assert fail if return null object
     */
    virtual zfautoObject listCellAtIndex(ZF_IN zfindex index) = 0;
    /**
     * @brief check cell size at index
     *
     * for performance, this method is ensured to be called for visible cells only,
     * to check the cell's actual size\n
     * while cell is not visible,
     * a predicted hint size (or previous size) would be used
     * util it's coming to visible\n
     * return a -1 size to measure the cell automatically,
     * otherwise, the cell's size is fixed\n
     * return #listCellSizeHint by default
     */
    virtual inline zfint listCellSizeAtIndex(ZF_IN zfindex index,
                                             ZF_IN ZFUIListCell *cell)
    {
        return this->listCellSizeHint();
    }

    // ============================================================
    // list cell update callback
protected:
    /**
     * @brief see #EventListCellOnUpdate
     */
    virtual inline void listCellOnUpdate(ZF_IN zfindex atIndex,
                                         ZF_IN ZFUIListCell *cell)
    {
        this->toObject()->observerNotify(zfself::EventListCellOnUpdate(), cell, ZFValue::indexValueCreate(atIndex).toObject());
    }

    // ============================================================
    // cache logic
protected:
    /**
     * @brief try access cached list cell, return null means no cache available,
     *   by default, no cache logic support
     *
     * see #listCellCacheOnRecycle for more info
     */
    virtual zfautoObject listCellCacheOnAccess(ZF_IN zfindex index)
    {
        return zfautoObjectNull;
    }
    zffinal inline void _ZFP_ZFUIListAdapter_listCellCacheOnRecycle(ZF_IN ZFUIListCell *cell)
    {
        this->listCellCacheOnRecycle(cell);
        this->toObject()->observerNotify(zfself::EventListCellCacheOnRecycle(), cell);
    }
    /**
     * @brief recycle cell or do nothing if you don't need recycle logic
     *
     * to achieve recycle logic, you should:
     * -# override #listCellCacheOnRecycle and store the cell manually
     * -# during #listCellCacheOnAccess, return your cached cell if exist
     *
     * by default, we have #listCellCacheDefaultAccess and #listCellCacheDefaultRecycle
     * that would suit for most case
     */
    virtual inline void listCellCacheOnRecycle(ZF_IN ZFUIListCell *cell)
    {
    }
    /** @brief see #listCellCacheOnRecycle */
    zffinal zfautoObject listCellCacheDefaultAccess(ZF_IN const zfchar *key);
    /** @brief see #listCellCacheOnRecycle */
    zffinal void listCellCacheDefaultRecycle(ZF_IN const zfchar *key, ZF_IN ZFUIListCell *cell);

private:
    friend zfclassFwd _ZFP_ZFUIListViewPrivate;
    friend zfclassFwd ZFUIListView;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapter_h_

