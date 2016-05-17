/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLeakTestDef.h
 * @brief leak test utility
 */

#ifndef _ZFI_ZFLeakTestDef_h_
#define _ZFI_ZFLeakTestDef_h_

#include "ZFObjectCoreRetainDef.h"
#include "ZFLeakTestObjectFilterDef.h"
#include "ZFOutputCallbackDefault.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global settings
/**
 * @brief add this to your precomipler to enable or disable builtin leak test, disable by default
 */
#ifndef ZF_LEAKTEST_ENABLE
    #define ZF_LEAKTEST_ENABLE 0
#else
    #undef ZF_LEAKTEST_ENABLE
    #define ZF_LEAKTEST_ENABLE 1
#endif

/**
 * @brief see #ZF_LEAKTEST_ENABLE, disabled by default, valid only if #ZF_LEAKTEST_ENABLE
 *
 * used only for debug ZFFramework itself
 */
#ifndef ZF_LEAKTEST_ENABLE_INTERNAL
    #define ZF_LEAKTEST_ENABLE_INTERNAL 0
#else
    #undef ZF_LEAKTEST_ENABLE_INTERNAL
    #define ZF_LEAKTEST_ENABLE_INTERNAL ZF_LEAKTEST_ENABLE
#endif

/**
 * @brief runtime configurable setting for ZFLeakTest,
 *   true by default, valid only if #ZF_LEAKTEST_ENABLE
 *
 * should be set before #ZFLeakTestBegin, and should be used only for debug use only
 */
extern ZF_ENV_EXPORT zfbool ZFLeakTestEnable;

zfclassFwd ZFClass;
zfclassFwd ZFObject;

extern ZF_ENV_EXPORT zfbool _ZFP_ZFLeakTestEnableCache;

/**
 * @brief default output callback for ZFLeakTest
 */
#define ZFLeakTestOutputCallbackDefault ZFOutputCallbackDefault

/** @brief see #ZFLeakTestBegin */
zfclassLikePOD ZF_ENV_EXPORT ZFLeakTestBeginParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFLeakTestBeginParam)

    /** @brief see #ZFLeakTestBegin, the #ZFOutputCallback to use, #ZFLeakTestOutputCallbackDefault by default */
    ZFCORE_PARAM_WITH_INIT(ZFOutputCallback, outputCallback, ZFLeakTestOutputCallbackDefault)

    /** @brief see #ZFLeakTestBegin, whether to check over-released object, zffalse by default */
    ZFCORE_PARAM_WITH_INIT(zfbool, checkOverRelease, zffalse)

    /** @brief see #ZFLeakTestBegin, whether output verbose info, zffalse by default */
    ZFCORE_PARAM_WITH_INIT(zfbool, verboseMode, zffalse)
};

extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestBegin(ZF_IN const zfcharA *callerFile,
                                               ZF_IN const zfcharA *callerFunction,
                                               ZF_IN zfindex callerLine,
                                               ZF_IN_OPT const ZFLeakTestBeginParam &param = ZFLeakTestBeginParam());
/**
 * @brief begin the leak test, remember to call ZFLeakTestEnd
 *
 * typical usage:
 * @code
 *   int main(int argc, zfcharA **argv)
 *   {
 *       ZFLeakTestBegin();
 *       int retVal = 0;
 *       { // code block so that objects would be released
 *           Application app(argc, argv); // your app routine
 *           retVal = app.exec();
 *       }
 *       ZFLeakTestPrintStatus();
 *       ZFLeakTestEnd();
 *       return retVal;
 *   }
 * @endcode
 *
 * @note if you want something pass through ZFLeakTest, use zfAllocWithoutLeakTest series, but try not to abuse it
 * @note you may call ZFLeakTestBegin multiple times,
 *   but you must make sure ZFLeakTestBegin and ZFLeakTestEnd are paired
 * @note leak test consumes much CPU and memory, you should enable only if necessary,
 *   and should not enable for too long,
 *   since object may retain or release much frequently,
 *   the actions would be accumulated and may consume much memory to store
 */
#if ZF_LEAKTEST_ENABLE
    #define ZFLeakTestBegin(...) _ZFP_ZFLeakTestBegin( \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
        ##__VA_ARGS__)
#else
    #define ZFLeakTestBegin(...) ((void)0)
#endif

extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestEnd(void);
/**
 * @brief end the leak test
 *
 * you may want to use #ZFLeakTestPrintStatus before end
 */
#if ZF_LEAKTEST_ENABLE
    #define ZFLeakTestEnd() _ZFP_ZFLeakTestEnd()
#else
    #define ZFLeakTestEnd() ((void)0)
#endif

// ============================================================
/**
 * @brief add a verbose observer to check specified class or object's life cycle,
 *   valid only if #ZFLeakTestBegin started, for debug use only,
 *   and may cause serious performance issue
 *
 * for object filter, object is compared by pointer value\n
 * for class filter, class type must fit exactly
 */
extern ZF_ENV_EXPORT void ZFLeakTestVerboseObserverAdd(ZF_IN const ZFClass *cls);
/** @brief see #ZFLeakTestVerboseObserverAdd */
extern ZF_ENV_EXPORT void ZFLeakTestVerboseObserverAdd(ZF_IN ZFObject *obj);
/** @brief see #ZFLeakTestVerboseObserverAdd */
extern ZF_ENV_EXPORT void ZFLeakTestVerboseObserverRemove(ZF_IN const ZFClass *cls);
/** @brief see #ZFLeakTestVerboseObserverAdd */
extern ZF_ENV_EXPORT void ZFLeakTestVerboseObserverRemove(ZF_IN ZFObject *obj);

// ============================================================
// global exclude list
/**
 * @brief add a global exclude to leak test
 *
 * internal use only, and usually used to exclude internal types,
 * take good care since it affect global range\n
 * after add, the leak test info would also be collected,
 * while no log would be printed when #ZFLeakTestPrintStatus\n
 * safe to be added more than one time
 * @see ZFLeakTestExcludeNameRemove, ZFLeakTestExcludeInstanceAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeNameAdd(ZF_IN const zfchar *name);
/**
 * @brief see #ZFLeakTestExcludeNameAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeNameRemove(ZF_IN const zfchar *name);
/**
 * @brief add a global exclude to leak test
 *
 * internal use only, and usually used to exclude internal types,
 * take good care since it affect global range\n
 * after add, the leak test info would also be collected,
 * while no log would be printed when #ZFLeakTestPrintStatus\n
 * safe to be added more than one time\n
 * unlike #ZFLeakTestExcludeNameRemove,
 * excluded instance would be automatically removed after the object deallocated,
 * and you may also remove it manually by #ZFLeakTestExcludeInstanceRemove
 * @see ZFLeakTestExcludeInstanceRemove, ZFLeakTestExcludeNameAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeInstanceAdd(ZF_IN ZFObject *obj);
/**
 * @brief see #ZFLeakTestExcludeInstanceAdd
 */
extern ZF_ENV_EXPORT void ZFLeakTestExcludeInstanceRemove(ZF_IN ZFObject *obj);

/**
 * @brief util macro to declare a global exclude name for ZFLeakTest
 *
 * add it to cpp file to make it active
 */
#define ZFLEAKTEST_EXCLUDENAME_DEFINE(Name) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMACRO_CAT(ZFLeakTestGlobalExcludeName, Name), ZFLevelZFFrameworkNormal) \
    { \
        ZFLeakTestExcludeNameAdd(ZFMACRO_TOSTRING(Name)); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFMACRO_CAT(ZFLeakTestGlobalExcludeName, Name)) \
    { \
        ZFLeakTestExcludeNameRemove(ZFMACRO_TOSTRING(Name)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFMACRO_CAT(ZFLeakTestGlobalExcludeName, Name))

// ============================================================
extern ZF_ENV_EXPORT ZFLeakTestObjectFilter _ZFP_ZFLeakTestObjectFilterNull;
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestPrintStatus(ZF_IN const zfcharA *callerFile,
                                                     ZF_IN const zfcharA *callerFunction,
                                                     ZF_IN zfindex callerLine,
                                                     ZF_IN_OPT const ZFLeakTestObjectFilter &filter = _ZFP_ZFLeakTestObjectFilterNull);
/**
 * @brief print current status
 *
 * print all object with non-zero objectRetainCount, along with the information of alloc/retain/release\n
 * must be called between ZFLeakTestBegin and ZFLeakTestEnd\n
 * may take 0 or 1 param, which is ZFLeakTestObjectFilter,
 * for how to use filter, see #ZFLeakTestObjectFilter
 */
#if ZF_LEAKTEST_ENABLE
    #define ZFLeakTestPrintStatus(...) _ZFP_ZFLeakTestPrintStatus( \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, \
        ##__VA_ARGS__)
#else
    #define ZFLeakTestPrintStatus(...) ((void)0)
#endif

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAlloc(ZF_IN ZFObject *obj,
                                                  ZF_IN const zfcharA *callerFile,
                                                  ZF_IN const zfcharA *callerFunction,
                                                  ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogRetain(ZF_IN ZFObject *obj,
                                                   ZF_IN const zfcharA *callerFile,
                                                   ZF_IN const zfcharA *callerFunction,
                                                   ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogRelease(ZF_IN ZFObject *obj,
                                                    ZF_IN const zfcharA *callerFile,
                                                    ZF_IN const zfcharA *callerFunction,
                                                    ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAutoRelease(ZF_IN ZFObject *obj,
                                                        ZF_IN const zfcharA *callerFile,
                                                        ZF_IN const zfcharA *callerFunction,
                                                        ZF_IN zfindex callerLine);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZFObject *obj);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogDealloc(ZFObject *obj);

// ============================================================
/**
 * @brief manually log alloc
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAllocVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAlloc(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogAllocVerbose */
#define ZFLeakTestLogAlloc(obj) \
    ZFLeakTestLogAllocVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogAllocVerbose, use with causion */
#define zflockfree_ZFLeakTestLogAllocVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAlloc(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAlloc, use with causion */
#define zflockfree_ZFLeakTestLogAlloc(obj) \
    zflockfree_ZFLeakTestLogAllocVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief manually log retain
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogRetainVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogRetain(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogRetainVerbose */
#define ZFLeakTestLogRetain(obj) \
    ZFLeakTestLogRetainVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogRetainVerbose, use with causion */
#define zflockfree_ZFLeakTestLogRetainVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogRetain(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogRetain, use with causion */
#define zflockfree_ZFLeakTestLogRetain(obj) \
    zflockfree_ZFLeakTestLogRetainVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief manually log release
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogReleaseVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogReleaseVerbose */
#define ZFLeakTestLogRelease(obj) \
    ZFLeakTestLogReleaseVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogReleaseVerbose, use with causion */
#define zflockfree_ZFLeakTestLogReleaseVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogRelease, use with causion */
#define zflockfree_ZFLeakTestLogRelease(obj) \
    zflockfree_ZFLeakTestLogReleaseVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief manually log autorelease or add to #ZFAutoReleasePool
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAutoReleaseVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief see #ZFLeakTestLogAutoReleaseVerbose */
#define ZFLeakTestLogAutoRelease(obj) \
    ZFLeakTestLogAutoReleaseVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)
/** @brief no lock version of #ZFLeakTestLogAutoReleaseVerbose, use with causion */
#define zflockfree_ZFLeakTestLogAutoReleaseVerbose(obj, callerFile, callerFunction, callerLine) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine); \
        } \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAutoRelease, use with causion */
#define zflockfree_ZFLeakTestLogAutoRelease(obj) \
    zflockfree_ZFLeakTestLogAutoReleaseVerbose(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief log that a autoreleased object is being released
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogAutoReleaseBeforeRelease(obj) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogAutoReleaseBeforeRelease, use with causion */
#define zflockfree_ZFLeakTestLogAutoReleaseBeforeRelease(obj) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
    } while(0)

/**
 * @brief log that a object is being deallocated
 *
 * log only, not actually modify the retain count\n
 * usually used internally, try not to use this in your application
 */
#define ZFLeakTestLogDealloc(obj) \
    do \
    { \
        ZFCoreMutexLock(); \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogDealloc(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
        ZFCoreMutexUnlock(); \
    } while(0)
/** @brief no lock version of #ZFLeakTestLogDealloc, use with causion */
#define zflockfree_ZFLeakTestLogDealloc(obj) \
    do \
    { \
        if(_ZFP_ZFLeakTestEnableCache) \
        { \
            _ZFP_ZFLeakTestLogDealloc(ZFCastZFObjectUnchecked(ZFObject *, obj)); \
        } \
    } while(0)

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogCacheAdd(ZF_IN ZFObject *obj);
extern ZF_ENV_EXPORT void _ZFP_ZFLeakTestLogCacheRemove(ZF_IN ZFObject *obj);

// ============================================================
template<typename T_ZFObject>
inline T_ZFObject *_ZFP_ZFLeakTestAlloc(ZF_IN T_ZFObject *obj,
                                        ZF_IN const zfcharA *callerFile,
                                        ZF_IN const zfcharA *callerFunction,
                                        ZF_IN zfindex callerLine)
{
    if(_ZFP_ZFLeakTestEnableCache)
    {
        _ZFP_ZFLeakTestLogAlloc(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine);
    }
    return obj;
}
template<typename T_ZFObject>
inline T_ZFObject *_ZFP_ZFLeakTestRetain(ZF_IN T_ZFObject *obj,
                                         ZF_IN const zfcharA *callerFile,
                                         ZF_IN const zfcharA *callerFunction,
                                         ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestLogRetain(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine);
    return zflockfree_zfRetainWithoutLeakTest(obj);
}
inline const ZFAny &_ZFP_ZFLeakTestRetain(ZF_IN const ZFAny &any,
                                          ZF_IN const zfcharA *callerFile,
                                          ZF_IN const zfcharA *callerFunction,
                                          ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestLogRetain(any.toObject(), callerFile, callerFunction, callerLine);
    return zflockfree_zfRetainWithoutLeakTest(any);
}
template<typename T_ZFObject>
inline const ZFAnyT<T_ZFObject *> &_ZFP_ZFLeakTestRetain(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                                         ZF_IN const zfcharA *callerFile,
                                                         ZF_IN const zfcharA *callerFunction,
                                                         ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestLogRetain(any.toObject(), callerFile, callerFunction, callerLine);
    return zflockfree_zfRetainWithoutLeakTest(any);
}
template<typename T_ZFObject>
inline void _ZFP_ZFLeakTestRelease(T_ZFObject *obj,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestLogRelease(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine);
    zflockfree_zfReleaseWithoutLeakTest(obj);
}
inline void _ZFP_ZFLeakTestRelease(ZF_IN const ZFAny &any,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestRelease(any.toObject(), callerFile, callerFunction, callerLine);
}
template<typename T_ZFObject>
inline void _ZFP_ZFLeakTestRelease(ZF_IN const ZFAnyT<T_ZFObject *> &any,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    _ZFP_ZFLeakTestRelease(any.toObject(), callerFile, callerFunction, callerLine);
}

// ============================================================
/**
 * @brief ensured to be logged by ZFLeakTest
 *
 * usually used internally
 * (so that the zfAlloc macro doesn't affect when building a static library),
 * try not to abuse this in your application
 */
#define zfAllocWithLeakTest(T_ZFObject, ...) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zfAllocWithLeakTest, use with causion */
#define zflockfree_zfAllocWithLeakTest(T_ZFObject, ...) \
    _ZFP_ZFLeakTestAlloc( \
        zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__), \
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

/**
 * @brief ensured to be logged by ZFLeakTest
 *
 * usually used internally
 * (so that the zfAlloc macro doesn't affect when building a static library),
 * try not to abuse this in your application
 */
#define zfRetainWithLeakTest(obj) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfRetainWithLeakTest(obj))
/** @brief no lock version of #zfRetainWithLeakTest, use with causion */
#define zflockfree_zfRetainWithLeakTest(obj) \
    (_ZFP_ZFLeakTestEnableCache \
        ? _ZFP_ZFLeakTestRetain(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE) \
        : zflockfree_zfRetainWithoutLeakTest(obj))

/**
 * @brief ensured to be logged by ZFLeakTest
 *
 * usually used internally
 * (so that the zfAlloc macro doesn't affect when building a static library),
 * try not to abuse this in your application
 */
#define zfReleaseWithLeakTest(obj) \
    (ZFCoreMutexLockerHolder(), zflockfree_zfReleaseWithLeakTest(obj))
/** @brief no lock version of #zfRetainWithLeakTest, use with causion */
#define zflockfree_zfReleaseWithLeakTest(obj) \
    (_ZFP_ZFLeakTestEnableCache \
        ? _ZFP_ZFLeakTestRelease(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE) \
        : zflockfree_zfReleaseWithoutLeakTest(obj))

// ============================================================
/**
 * @def zfAlloc
 * @brief simlar to "new T_Zobject(...)"
 *
 * alloc a new object, the #ZFObject::objectOnInit would be called\n
 * auto log leak test info if #ZF_LEAKTEST_ENABLE is zftrue
 * @see zfAllocWithLeakTest, zfAllocWithoutLeakTest, zfRetain, zfRelease
 * @def zfRetain
 * @brief retain a ZFObject
 *
 * increase objectRetainCount by 1 and return the obj itself,
 * it's safe to pass a zfnull as param\n
 * auto log leak test info if #ZF_LEAKTEST_ENABLE is zftrue
 * @see ZFObject::objectRetainCount,
 *   zfAlloc, zfRetainWithLeakTest, zfRetainWithoutLeakTest, zfRelease, ZFPropertyChange
 * @def zfRelease
 * @brief release a ZFObject
 *
 * decrease objectRetainCount by 1, if reach 0, delete obj automatically,
 * it's safe to pass a zfnull as param\n
 * auto log leak test info if #ZF_LEAKTEST_ENABLE is zftrue
 * @see ZFObject::objectRetainCount,
 *   zfAlloc, zfRetain, zfReleaseWithLeakTest, zfReleaseWithoutLeakTest, ZFPropertyChange
 */
#if ZF_LEAKTEST_ENABLE
    #define zfAlloc(T_ZFObject, ...) zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__)
    #define zfRetain(obj) zfRetainWithLeakTest(obj)
    #define zfRelease(obj) zfReleaseWithLeakTest(obj)
#else // #if ZF_LEAKTEST_ENABLE
    #define zfAlloc(T_ZFObject, ...) zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)
    #define zfRetain(obj) zfRetainWithoutLeakTest(obj)
    #define zfRelease(obj) zfReleaseWithoutLeakTest(obj)
#endif // #if ZF_LEAKTEST_ENABLE #else

// ============================================================
/**
 * @def zfAllocInternal
 * @brief same as zfAllocWithLeakTest if ZFLeakTestEnable,
 *   or zfAllocWithoutLeakTest otherwise
 * @note no lock for internal memory management, use #zflocked_zfAllocInternal if necessary
 * @def zfRetainInternal
 * @brief same as zfRetainWithLeakTest if ZFLeakTestEnable,
 *   or zfRetainWithoutLeakTest otherwise
 * @note no lock for internal memory management, use #zflocked_zfRetainInternal if necessary
 * @def zfReleaseInternal
 * @brief same as zfReleaseWithLeakTest if ZFLeakTestEnable,
 *   or zfReleaseWithoutLeakTest otherwise
 * @note no lock for internal memory management, use #zflocked_zfReleaseInternal if necessary
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zfAllocInternal(T_ZFObject, ...) zflockfree_zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__)
    #define zfRetainInternal(obj) zflockfree_zfRetainWithLeakTest(obj)
    #define zfReleaseInternal(obj) zflockfree_zfReleaseWithLeakTest(obj)
#else
    #define zfAllocInternal(T_ZFObject, ...) zflockfree_zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)
    #define zfRetainInternal(obj) zflockfree_zfRetainWithoutLeakTest(obj)
    #define zfReleaseInternal(obj) zflockfree_zfReleaseWithoutLeakTest(obj)
#endif
/**
 * @def zflocked_zfAllocInternal
 * @brief no lock version of #zfAllocInternal, use with causion
 * @def zflocked_zfRetainInternal
 * @brief no lock version of #zfRetainInternal, use with causion
 * @def zflocked_zfReleaseInternal
 * @brief no lock version of #zfReleaseInternal, use with causion
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_zfAllocInternal(T_ZFObject, ...) zfAllocWithLeakTest(T_ZFObject, ##__VA_ARGS__)
    #define zflocked_zfRetainInternal(obj) zfRetainWithLeakTest(obj)
    #define zflocked_zfReleaseInternal(obj) zfReleaseWithLeakTest(obj)
#else
    #define zflocked_zfAllocInternal(T_ZFObject, ...) zfAllocWithoutLeakTest(T_ZFObject, ##__VA_ARGS__)
    #define zflocked_zfRetainInternal(obj) zfRetainWithoutLeakTest(obj)
    #define zflocked_zfReleaseInternal(obj) zfReleaseWithoutLeakTest(obj)
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLeakTestDef_h_

