/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectSmartPointerFwd.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_ZFObjectSmartPointerFwd_h_
#define _ZFI_ZFObjectSmartPointerFwd_h_

#include "ZFAnyDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoObject
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivateBase
{
public:
    zfindex refCount;
    ZFObject *obj;
public:
    _ZFP_zfautoObjectPrivateBase(void)
    : refCount(1)
    , obj(zfnull)
    {
    }
    _ZFP_zfautoObjectPrivateBase(ZF_IN ZFObject *obj)
    : refCount(1)
    , obj(obj)
    {
    }
    virtual ~_ZFP_zfautoObjectPrivateBase(void)
    {
    }
public:
    virtual void onRelease(void) = 0;
};
/**
 * @brief a ZFObject holder which would release content object automatically when destroyed
 *
 * must be created by #zfautoObjectCreate\n
 * useful to hold a ZFObject as temp return value:
 * @code
 *   zfautoObject yourFunc(void)
 *   {
 *       // alloc an object for return
 *       zfblockedAlloc(ZFObject, obj);
 *
 *       // use zfautoObject to wrap the returned object
 *       return zfautoObjectCreate(obj);
 *   }
 *
 *   {
 *       zfautoObject value = yourFunc();
 *       ZFObject *obj = value.toObject();
 *   } // content obj would be released when zfautoObject destroyed
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT zfautoObject
{
    /** @cond ZFPrivateDoc */
public:
    zfautoObject(void) : d(zfnull) {}
    zfautoObject(ZF_IN zfautoObject const &ref)
    {
        if(ref.d)
        {
            ++(ref.d->refCount);
        }
        d = ref.d;
    }
    zfautoObject &operator = (ZF_IN zfautoObject const &ref)
    {
        if(ref.d)
        {
            ++(ref.d->refCount);
        }
        this->releaseRef();
        d = ref.d;
        return *this;
    }
    zfbool operator == (ZF_IN zfautoObject const &ref) const
    {
        return (this->toObject() == ref.toObject());
    }
    zfbool operator != (ZF_IN zfautoObject const &ref) const
    {
        return (this->toObject() != ref.toObject());
    }
    ~zfautoObject(void)
    {
        this->releaseRef();
    }
    /** @endcond */

public:
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const
    {
        return d->refCount;
    }

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const
    {
        return (d ? d->obj : zfnull);
    }
    /**
     * @brief get the holded object
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }
    /**
     * @brief get the holded object
     */
    inline ZFAny toAny(void) const
    {
        return ZFAny(this->toObject());
    }

public:
    static inline zfautoObject _ZFP_zfautoObjectCreate(ZF_IN _ZFP_zfautoObjectPrivateBase *data)
    {
        zfautoObject ret;
        ret.d = data;
        return ret;
    }
private:
    _ZFP_zfautoObjectPrivateBase *d;
    void releaseRef(void)
    {
        if(d)
        {
            --(d->refCount);
            if(d->refCount == 0)
            {
                d->onRelease();
                zfdelete(d);
            }
            d = zfnull;
        }
    }
};
/**
 * @brief global null zfautoObject
 */
extern ZF_ENV_EXPORT const zfautoObject zfautoObjectNull;

// ============================================================
// ZFCastZFObject fix for ZFAny
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(zfautoObject const &obj);
inline void _ZFP_ZFCastZFObjectToUnknown(zfautoObject &ret,
                                         ZFObject * const &obj);

inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(zfautoObject const &obj);
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(zfautoObject &ret,
                                                  ZFObject * const &obj);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectSmartPointerFwd_h_

