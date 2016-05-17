/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectSmartPointerDef.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_ZFObjectSmartPointerDef_h_
#define _ZFI_ZFObjectSmartPointerDef_h_

#include "ZFObjectDef.h"
#include "ZFObjectRetainDef.h"
#include "ZFAnyDef.h"
#include "ZFObjectSmartPointerFwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObjectWithLeakTest, {
    }, {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogReleaseVerbose(p, zfTextA("unknown"), zfTextA("ZFCorePointerOnDelete"), 0);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObjectWithoutLeakTest, {
    }, {
        zfReleaseWithoutLeakTest(p);
    })
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObjectMarkCached, {
        p->objectCachedSet(zftrue);
    }, {
        ZFCoreMutexLock();
        p->objectCachedSet(zffalse);
        zflockfree_zfReleaseWithoutLeakTest(p);
        ZFCoreMutexUnlock();
    })

/**
 * @brief deleted by zfRelease, see #ZFCorePointer
 */
#if ZF_LEAKTEST_ENABLE
    #define ZFCorePointerForZFObject ZFCorePointerForZFObjectWithLeakTest
#else
    #define ZFCorePointerForZFObject ZFCorePointerForZFObjectWithoutLeakTest
#endif

/**
 * @brief deleted by zfReleaseInternal, see #ZFCorePointer
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define ZFCorePointerForZFObjectInternal ZFCorePointerForZFObjectWithLeakTest
#else
    #define ZFCorePointerForZFObjectInternal ZFCorePointerForZFObjectWithoutLeakTest
#endif

// ============================================================
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflineReleaseWithLeakTest
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(ZF_IN T_ZFObject *obj,
                    ZF_IN const zfcharA *callerFile,
                    ZF_IN const zfcharA *callerFunction,
                    ZF_IN zfindex callerLine)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any,
                     ZF_IN const zfcharA *callerFile,
                     ZF_IN const zfcharA *callerFunction,
                     ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                    ZF_IN const zfcharA *callerFile,
                                    ZF_IN const zfcharA *callerFunction,
                                    ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
public:
    _ZFP_zflineReleaseWithLeakTest(void)
    : obj(zfnull)
    , callerFile(zfnull)
    , callerFunction(zfnull)
    , callerLine(0)
    {
    }
    ~_ZFP_zflineReleaseWithLeakTest(void)
    {
        if(this->obj != zfnull)
        {
            ZFCoreMutexLock();
            zflockfree_ZFLeakTestLogReleaseVerbose(this->obj,
                this->callerFile, this->callerFunction, this->callerLine);
            zflockfree_zfReleaseWithoutLeakTest(this->obj);
            ZFCoreMutexUnlock();
        }
    }
public:
    ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflockfree_zflineReleaseWithLeakTest
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(ZF_IN T_ZFObject *obj,
                    ZF_IN const zfcharA *callerFile,
                    ZF_IN const zfcharA *callerFunction,
                    ZF_IN zfindex callerLine)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any,
                     ZF_IN const zfcharA *callerFile,
                     ZF_IN const zfcharA *callerFunction,
                     ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                    ZF_IN const zfcharA *callerFile,
                                    ZF_IN const zfcharA *callerFunction,
                                    ZF_IN zfindex callerLine)
    {
        this->obj = any.toObject();
        this->callerFile = callerFile;
        this->callerFunction = callerFunction;
        this->callerLine = callerLine;
        return any;
    }
public:
    _ZFP_zflockfree_zflineReleaseWithLeakTest(void)
    : obj(zfnull)
    , callerFile(zfnull)
    , callerFunction(zfnull)
    , callerLine(0)
    {
    }
    ~_ZFP_zflockfree_zflineReleaseWithLeakTest(void)
    {
        if(this->obj != zfnull)
        {
            zflockfree_ZFLeakTestLogReleaseVerbose(this->obj,
                this->callerFile, this->callerFunction, this->callerLine);
            zflockfree_zfReleaseWithoutLeakTest(this->obj);
        }
    }
public:
    ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflineReleaseWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(T_ZFObject *obj)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any)
    {
        this->obj = any.toObject();
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any)
    {
        this->obj = any.toObject();
        return any;
    }
public:
    _ZFP_zflineReleaseWithoutLeakTest(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zflineReleaseWithoutLeakTest(void)
    {
        zfReleaseWithoutLeakTest(obj);
    }
public:
    ZFObject *obj;
};
zffinal zfclassNotPOD ZF_ENV_EXPORT _ZFP_zflockfree_zflineReleaseWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    T_ZFObject *set(T_ZFObject *obj)
    {
        this->obj = ZFCastZFObjectUnchecked(ZFObject *, obj);
        return obj;
    }
    const ZFAny &set(ZF_IN const ZFAny &any)
    {
        this->obj = any.toObject();
        return any;
    }
    template<typename T_ZFObject>
    const ZFAnyT<T_ZFObject *> &set(ZF_IN const ZFAnyT<T_ZFObject *> &any)
    {
        this->obj = any.toObject();
        return any;
    }
public:
    _ZFP_zflockfree_zflineReleaseWithoutLeakTest(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zflockfree_zflineReleaseWithoutLeakTest(void)
    {
        zflockfree_zfReleaseWithoutLeakTest(obj);
    }
public:
    ZFObject *obj;
};
/**
 * @brief see #zflineRelease
 */
#define zflineReleaseWithLeakTest(obj) (_ZFP_zflineReleaseWithLeakTest().set(obj, \
    ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE))
/** @brief no lock version of #zflineReleaseWithLeakTest, use with causion */
#define zflockfree_zflineReleaseWithLeakTest(obj) (_ZFP_zflockfree_zflineReleaseWithLeakTest().set(obj, \
    ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE))
/**
 * @brief see #zflineRelease
 */
#define zflineReleaseWithoutLeakTest(obj) (_ZFP_zflineReleaseWithoutLeakTest().set(obj))
/** @brief no lock version of #zflineReleaseWithLeakTest, use with causion */
#define zflockfree_zflineReleaseWithoutLeakTest(obj) (_ZFP_zflockfree_zflineReleaseWithoutLeakTest().set(obj))
/**
 * @brief convenient method to release the object after code line end
 *
 * usage:
 * @code
 *   ZFObject *allocObject(void)
 *   {
 *       return zfAlloc(ZFObject);
 *   }
 *
 *   ZFArray *array = ...;
 *   array->add( // step 2, array auto retain it, objectRetainCount 2
 *       zflineRelease(
 *               allocObject() // step 1, objectRetainCount 1
 *           )
 *       ); // step 3, zflineRelease auto release it, objectRetainCount 1
 *
 *   // equivalence:
 *   // ZFObject *obj = allocObject();
 *   // array->add(obj);
 *   // zfRelease(obj);
 * @endcode
 * usually, this method is used to save you some code,
 * or designed for chained code
 */
#if ZF_LEAKTEST_ENABLE
    #define zflineRelease(obj) zflineReleaseWithLeakTest(obj)
#else
    #define zflineRelease(obj) zflineReleaseWithoutLeakTest(obj)
#endif

/**
 * @brief see #zflineRelease
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflineReleaseInternal(obj) zflockfree_zflineReleaseWithLeakTest(obj)
#else
    #define zflineReleaseInternal(obj) zflockfree_zflineReleaseWithoutLeakTest(obj)
#endif
/** @brief no lock version of #zflineReleaseInternal, use with causion */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_zflineReleaseInternal(obj) zflineReleaseWithLeakTest(obj)
#else
    #define zflocked_zflineReleaseInternal(obj) zflineReleaseWithoutLeakTest(obj)
#endif

// ============================================================
/**
 * @brief see #zflineRelease
 */
#define zflineAllocWithLeakTest(T_ZFObject, ...) \
    zflineReleaseWithLeakTest(zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zflineAllocWithLeakTest, use with causion */
#define zflockfree_zflineAllocWithLeakTest(T_ZFObject, ...) \
    zflockfree_zflineReleaseWithLeakTest(zflockfree_zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__))
/**
 * @brief see #zflineRelease
 */
#define zflineAllocWithoutLeakTest(T_ZFObject, ...) \
    zflineReleaseWithoutLeakTest(zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zflineAllocWithoutLeakTest, use with causion */
#define zflockfree_zflineAllocWithoutLeakTest(T_ZFObject, ...) \
    zflockfree_zflineReleaseWithoutLeakTest(zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__))

/**
 * @brief same as zflineRelease(zfAlloc(...)), see #zflineRelease
 */
#if ZF_LEAKTEST_ENABLE
    #define zflineAlloc(T_ZFObject, ...) zflineAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__)
#else
    #define zflineAlloc(T_ZFObject, ...) zflineAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)
#endif

/**
 * @brief see #zflineRelease
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflineAllocInternal(T_ZFObject, ...) zflockfree_zflineAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__)
#else
    #define zflineAllocInternal(T_ZFObject, ...) zflockfree_zflineAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)
#endif
/** @brief no lock version of #zflineAllocInternal, use with causion */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_zflineAllocInternal(T_ZFObject, ...) zflineAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__)
#else
    #define zflocked_zflineAllocInternal(T_ZFObject, ...) zflineAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)
#endif

// ============================================================
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedAllocContainerWithLeakTest
{
public:
    explicit _ZFP_zfblockedAllocContainerWithLeakTest(ZF_IN T_ZFObject *obj,
                                                      ZF_IN const zfcharA *callerFile,
                                                      ZF_IN const zfcharA *callerFunction,
                                                      ZF_IN zfindex callerLine)
    : obj(obj)
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zfblockedAllocContainerWithLeakTest(void)
    {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
        ZFCoreMutexUnlock();
    }
public:
    T_ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedAllocContainerWithLeakTest
{
public:
    explicit _ZFP_zflockfree_zfblockedAllocContainerWithLeakTest(ZF_IN T_ZFObject *obj,
                                                                 ZF_IN const zfcharA *callerFile,
                                                                 ZF_IN const zfcharA *callerFunction,
                                                                 ZF_IN zfindex callerLine)
    : obj(obj)
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zflockfree_zfblockedAllocContainerWithLeakTest(void)
    {
        zflockfree_ZFLeakTestLogReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
public:
    T_ZFObject *obj;
private:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedAllocContainerWithoutLeakTest
{
public:
    explicit _ZFP_zfblockedAllocContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(obj)
    {
    }
    ~_ZFP_zfblockedAllocContainerWithoutLeakTest(void)
    {
        zfReleaseWithoutLeakTest(this->obj);
    }
public:
    T_ZFObject *obj;
};
template<typename T_ZFObject>
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest
{
public:
    explicit _ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(obj)
    {
    }
    ~_ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest(void)
    {
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
public:
    T_ZFObject *obj;
};
/**
 * @brief see #zfblockedAlloc
 */
#define zfblockedAllocWithLeakTest(T_ZFObject, name, ...) \
    _ZFP_zfblockedAllocContainerWithLeakTest<T_ZFObject> \
        ZFUniqueName(name) (zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__), \
            ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE); \
    T_ZFObject *name = (ZFUniqueName(name).obj)
/** @brief no lock version of #zfblockedAllocWithLeakTest, use with causion */
#define zflockfree_zfblockedAllocWithLeakTest(T_ZFObject, name, ...) \
    _ZFP_zflockfree_zfblockedAllocContainerWithLeakTest<T_ZFObject> \
        ZFUniqueName(name) (zflockfree_zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__), \
            ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE); \
    T_ZFObject *name = (ZFUniqueName(name).obj)
/**
 * @brief see #zfblockedAlloc
 */
#define zfblockedAllocWithoutLeakTest(T_ZFObject, name, ...) \
    _ZFP_zfblockedAllocContainerWithoutLeakTest<T_ZFObject> \
    ZFUniqueName(name) (zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)); \
    T_ZFObject *name = (ZFUniqueName(name).obj)
/** @brief no lock version of #zfblockedAllocWithoutLeakTest, use with causion */
#define zflockfree_zfblockedAllocWithoutLeakTest(T_ZFObject, name, ...) \
    _ZFP_zflockfree_zfblockedAllocContainerWithoutLeakTest<T_ZFObject> \
    ZFUniqueName(name) (zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)); \
    T_ZFObject *name = (ZFUniqueName(name).obj)

/**
 * @brief alloc a ZFObject looks like on a stack
 *
 * @code
 *   ZFString *saved = zfnull;
 *   {
 *       zfblockedAlloc(
 *           ZFString, // object's type
 *           s, // object's name
 *           zfText("init value") // params passed to object's objectOnInit
 *       );
 *       s->objectInfo(); // access like normal ZFObject
 *       saved = zfRetain(s); // you may retain it and save it
 *   } // s would be released after this block
 *   saved->objectInfo(); // s is still alive since we retained it in the block
 *   zfRelease(saved); // release it
 * @endcode
 */
#if ZF_LEAKTEST_ENABLE
    #define zfblockedAlloc(T_ZFObject, name, ...) zfblockedAllocWithLeakTest(T_ZFObject, name, ##__VA_ARGS__)
#else
    #define zfblockedAlloc(T_ZFObject, name, ...) zfblockedAllocWithoutLeakTest(T_ZFObject, name, ##__VA_ARGS__)
#endif

/**
 * @brief see #zfblockedAlloc
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zfblockedAllocInternal(T_ZFObject, name, ...) zflockfree_zfblockedAllocWithLeakTest(T_ZFObject, name, ##__VA_ARGS__)
#else
    #define zfblockedAllocInternal(T_ZFObject, name, ...) zflockfree_zfblockedAllocWithoutLeakTest(T_ZFObject, name, ##__VA_ARGS__)
#endif
/** @brief no lock version of #zfblockedAllocInternal, use with causion */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_zfblockedAllocInternal(T_ZFObject, name, ...) zfblockedAllocWithLeakTest(T_ZFObject, name, ##__VA_ARGS__)
#else
    #define zflocked_zfblockedAllocInternal(T_ZFObject, name, ...) zfblockedAllocWithoutLeakTest(T_ZFObject, name, ##__VA_ARGS__)
#endif

// ============================================================
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedReleaseContainerWithLeakTest
{
public:
    template<typename T_ZFObject>
    _ZFP_zfblockedReleaseContainerWithLeakTest(ZF_IN T_ZFObject *obj,
                                               ZF_IN const zfcharA *callerFile,
                                               ZF_IN const zfcharA *callerFunction,
                                               ZF_IN zfindex callerLine)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zfblockedReleaseContainerWithLeakTest(void)
    {
        ZFCoreMutexLock();
        zflockfree_ZFLeakTestLogReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
        ZFCoreMutexUnlock();
    }
private:
    ZFObject *obj;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedReleaseContainerWithLeakTest
{
public:
    template<typename T_ZFObject>
    _ZFP_zflockfree_zfblockedReleaseContainerWithLeakTest(ZF_IN T_ZFObject *obj,
                                                        ZF_IN const zfcharA *callerFile,
                                                        ZF_IN const zfcharA *callerFunction,
                                                        ZF_IN zfindex callerLine)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
    ~_ZFP_zflockfree_zfblockedReleaseContainerWithLeakTest(void)
    {
        zflockfree_ZFLeakTestLogReleaseVerbose(this->obj,
            this->callerFile, this->callerFunction, this->callerLine);
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
private:
    ZFObject *obj;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfblockedReleaseContainerWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    _ZFP_zfblockedReleaseContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    {
    }
    ~_ZFP_zfblockedReleaseContainerWithoutLeakTest(void)
    {
        zfReleaseWithoutLeakTest(this->obj);
    }
private:
    ZFObject *obj;
};
zffinal zfclassLikePOD ZF_ENV_EXPORT _ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest
{
public:
    template<typename T_ZFObject>
    _ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest(ZF_IN T_ZFObject *obj)
    : obj(ZFCastZFObjectUnchecked(ZFObject *, obj))
    {
    }
    ~_ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest(void)
    {
        zflockfree_zfReleaseWithoutLeakTest(this->obj);
    }
private:
    ZFObject *obj;
};
/**
 * @brief see #zfblockedRelease
 */
#define zfblockedReleaseWithLeakTest(obj) \
    _ZFP_zfblockedReleaseContainerWithLeakTest ZFUniqueName(zfblockedRelease) (obj, \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE); \
    ZFUNUSED(ZFUniqueName(zfblockedRelease))
/** @brief no lock version of #zfblockedReleaseWithLeakTest, use with causion */
#define zflockfree_zfblockedReleaseWithLeakTest(obj) \
    _ZFP_zflockfree_zfblockedReleaseContainerWithLeakTest ZFUniqueName(zfblockedRelease) (obj, \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE); \
    ZFUNUSED(ZFUniqueName(zfblockedRelease))
/**
 * @brief see #zfblockedRelease
 */
#define zfblockedReleaseWithoutLeakTest(obj) \
    _ZFP_zfblockedReleaseContainerWithoutLeakTest ZFUniqueName(zfblockedRelease) (obj); \
    ZFUNUSED(ZFUniqueName(zfblockedRelease))
/** @brief no lock version of #zfblockedReleaseWithoutLeakTest, use with causion */
#define zflockfree_zfblockedReleaseWithoutLeakTest(obj) \
    _ZFP_zflockfree_zfblockedReleaseContainerWithoutLeakTest ZFUniqueName(zfblockedRelease) (obj); \
    ZFUNUSED(ZFUniqueName(zfblockedRelease))

/**
 * @brief release the object after nearest block
 *
 * @code
 *   { // code block
 *       ZFObject *obj = ...;
 *       zfblockedRelease(obj); // mark release, must be single line
 *       return ; // safe to return, auto released after exiting code block
 *   } // obj would be released after here
 * @endcode
 */
#if ZF_LEAKTEST_ENABLE
    #define zfblockedRelease(obj) zfblockedReleaseWithLeakTest(obj)
#else
    #define zfblockedRelease(obj) zfblockedReleaseWithoutLeakTest(obj)
#endif

/**
 * @brief see #zfblockedRelease
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zfblockedReleaseInternal(obj) zflockfree_zfblockedReleaseWithLeakTest(obj)
#else
    #define zfblockedReleaseInternal(obj) zflockfree_zfblockedReleaseWithoutLeakTest(obj)
#endif
/** @brief no lock version of #zfblockedReleaseInternal, use with causion */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_zfblockedReleaseInternal(obj) zfblockedReleaseWithLeakTest(obj)
#else
    #define zflocked_zfblockedReleaseInternal(obj) zfblockedReleaseWithoutLeakTest(obj)
#endif

// ============================================================
// zfautoObject
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivateWithoutLeakTest : public _ZFP_zfautoObjectPrivate
{
public:
    _ZFP_zfautoObjectPrivateWithoutLeakTest(ZF_IN ZFObject *obj)
    : _ZFP_zfautoObjectPrivate(obj)
    {
    }
public:
    virtual void onRelease(void)
    {
        if(this->obj)
        {
            zfReleaseWithoutLeakTest(this->obj);
        }
    }
};
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivateWithLeakTest : public _ZFP_zfautoObjectPrivate
{
public:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
public:
    _ZFP_zfautoObjectPrivateWithLeakTest(ZF_IN ZFObject *obj,
                                         ZF_IN const zfcharA *callerFile,
                                         ZF_IN const zfcharA *callerFunction,
                                         ZF_IN zfindex callerLine)
    : _ZFP_zfautoObjectPrivate(obj)
    , callerFile(callerFile)
    , callerFunction(callerFunction)
    , callerLine(callerLine)
    {
    }
public:
    virtual void onRelease(void)
    {
        if(this->obj)
        {
            ZFCoreMutexLock();
            zflockfree_ZFLeakTestLogReleaseVerbose(this->obj, callerFile, callerFunction, callerLine);
            zflockfree_zfReleaseWithoutLeakTest(this->obj);
            ZFCoreMutexUnlock();
        }
    }
};
/** @brief see #zfautoObject */
#define zfautoObjectCreateWithoutLeakTest(obj) \
    zfautoObject::_ZFP_zfautoObjectCreate(zfnew(_ZFP_zfautoObjectPrivateWithoutLeakTest, zfRetainWithoutLeakTest(obj)))
/** @brief no lock version of #zfautoObjectCreateWithoutLeakTest, use with causion */
#define zflockfree_zfautoObjectCreateWithoutLeakTest(obj) \
    zfautoObject::_ZFP_zfautoObjectCreate(zfnew(_ZFP_zfautoObjectPrivateWithoutLeakTest, zflockfree_zfRetainWithoutLeakTest(obj)))

/** @brief see #zfautoObject */
#define zfautoObjectCreateWithLeakTestVerbose(obj, callerFile, callerFunction, callerLine) \
    zfautoObject::_ZFP_zfautoObjectCreate((ZFCoreMutexLockerHolder(), _ZFP_ZFLeakTestEnableCache \
        ? (_ZFP_zfautoObjectPrivate *)zfnew(_ZFP_zfautoObjectPrivateWithLeakTest, zfRetainWithLeakTest(obj), \
                callerFile, callerFunction, callerLine \
            ) \
        : (_ZFP_zfautoObjectPrivate *)zfnew(_ZFP_zfautoObjectPrivateWithoutLeakTest, zfRetainWithoutLeakTest(obj)) \
        ))
/** @brief see #zfautoObject */
#define zfautoObjectCreateWithLeakTest(obj) \
    zfautoObjectCreateWithLeakTestVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #zfautoObjectCreateWithLeakTestVerbose, use with causion */
#define zflockfree_zfautoObjectCreateWithLeakTestVerbose(obj, callerFile, callerFunction, callerLine) \
    zfautoObject::_ZFP_zfautoObjectCreate((ZFCoreMutexLockerHolder(), _ZFP_ZFLeakTestEnableCache \
        ? (_ZFP_zfautoObjectPrivate *)zfnew(_ZFP_zfautoObjectPrivateWithLeakTest, zflockfree_zfRetainWithLeakTest(obj), \
                callerFile, callerFunction, callerLine \
            ) \
        : (_ZFP_zfautoObjectPrivate *)zfnew(_ZFP_zfautoObjectPrivateWithoutLeakTest, zflockfree_zfRetainWithoutLeakTest(obj)) \
        ))
/** @brief no lock version of #zfautoObjectCreateWithLeakTest, use with causion */
#define zflockfree_zfautoObjectCreateWithLeakTest(obj) \
    zflockfree_zfautoObjectCreateWithLeakTestVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/** @brief see #zfautoObject */
#if ZF_LEAKTEST_ENABLE
    #define zfautoObjectCreate(obj) zfautoObjectCreateWithLeakTest(obj)
#else
    #define zfautoObjectCreate(obj) zfautoObjectCreateWithoutLeakTest(obj)
#endif

/** @brief see #zfautoObject */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zfautoObjectCreateInternal(obj) zflockfree_zfautoObjectCreateWithLeakTest(obj)
#else
    #define zfautoObjectCreateInternal(obj) zflockfree_zfautoObjectCreateWithoutLeakTest(obj)
#endif
/** @brief no lock version of #zfautoObjectCreateInternal, use with causion */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_zfautoObjectCreateInternal(obj) zfautoObjectCreateWithLeakTest(obj)
#else
    #define zflocked_zfautoObjectCreateInternal(obj) zfautoObjectCreateWithoutLeakTest(obj)
#endif

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivateMarkCached : public _ZFP_zfautoObjectPrivate
{
public:
    _ZFP_zfautoObjectPrivateMarkCached(ZF_IN ZFObject *obj)
    : _ZFP_zfautoObjectPrivate(obj)
    {
        if(this->obj)
        {
            this->obj->objectCachedSet(zftrue);
        }
    }
public:
    virtual void onRelease(void)
    {
        if(this->obj)
        {
            this->obj->objectCachedSet(zffalse);
            zfReleaseWithoutLeakTest(this->obj);
        }
    }
};
/** @brief see #zfautoObject */
#define zfautoObjectCreateMarkCached(obj) \
    zfautoObject::_ZFP_zfautoObjectCreate(zfnew(_ZFP_zfautoObjectPrivateMarkCached, zfRetainWithoutLeakTest(obj)))
/** @brief no lock version of #zfautoObjectCreateMarkCached, use with causion */
#define zflockfree_zfautoObjectCreateMarkCached(obj) \
    zfautoObject::_ZFP_zfautoObjectCreate(zfnew(_ZFP_zfautoObjectPrivateMarkCached, zflockfree_zfRetainWithoutLeakTest(obj)))

// ============================================================
// ZFCastZFObject fix for ZFAny
inline ZFObject *_ZFP_ZFCastZFObjectFromUnknown(zfautoObject const &obj)
{
    return obj.toObject();
}
inline void _ZFP_ZFCastZFObjectToUnknown(zfautoObject &ret,
                                         ZFObject * const &obj)
{
    ZFCoreMutexLock();
    zflockfree_ZFLeakTestLogRetainVerbose(obj, ZF_CALLER_FILE, zfTextA("ZFCastZFObject"), ZF_CALLER_LINE);
    ret = zflockfree_zfautoObjectCreateWithLeakTestVerbose(obj, ZF_CALLER_FILE, zfTextA("ZFCastZFObject"), ZF_CALLER_LINE);
    ZFCoreMutexUnlock();
}

inline ZFObject *_ZFP_ZFCastZFObjectFromUnknownUnchecked(zfautoObject const &obj)
{
    return obj.toObject();
}
inline void _ZFP_ZFCastZFObjectToUnknownUnchecked(zfautoObject &ret,
                                                  ZFObject * const &obj)
{
    ZFCoreMutexLock();
    zflockfree_ZFLeakTestLogRetainVerbose(obj, ZF_CALLER_FILE, zfTextA("ZFCastZFObjectUnchecked"), ZF_CALLER_LINE);
    ret = zflockfree_zfautoObjectCreateWithLeakTestVerbose(obj, ZF_CALLER_FILE, zfTextA("ZFCastZFObjectUnchecked"), ZF_CALLER_LINE);
    ZFCoreMutexUnlock();
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectSmartPointerDef_h_

