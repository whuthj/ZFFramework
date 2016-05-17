/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreGlobalInitializerDef.h
 * @brief global initializer step
 */
#ifndef _ZFI_ZFCoreGlobalInitializerDef_h_
#define _ZFI_ZFCoreGlobalInitializerDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreLevelDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief must be called in main thread before any access of ZFFramework member
 *
 * usually, this would been done by implementation of ZFFramework,
 * if ZFFramework already initialized, this function would do nothing
 * @note this method must be called in main thread
 */
extern ZF_ENV_EXPORT void ZFFrameworkInit(void);
/**
 * @brief see #ZFFrameworkInit
 */
extern ZF_ENV_EXPORT void ZFFrameworkCleanup(void);

/**
 * @brief state for ZFFramework
 */
typedef enum {
    ZFFrameworkStateNotAvailable,
    ZFFrameworkStateInitProcessing,
    ZFFrameworkStateAvailable,
    ZFFrameworkStateCleanupProcessing,
} ZFFrameworkState;

/**
 * @brief assert fail and output log if ZFFramework not initialized
 */
extern ZF_ENV_EXPORT void ZFFrameworkAssertInit(void);
/**
 * @brief get current state of ZFFramework
 */
extern ZF_ENV_EXPORT ZFFrameworkState ZFFrameworkStateCheck(void);
/**
 * @brief get current state of ZFFramework of specified level
 */
extern ZF_ENV_EXPORT ZFFrameworkState ZFFrameworkStateCheck(ZF_IN ZFLevel level);

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFGlobalInitializerDummyBase
{
public:
    virtual ~_ZFP_ZFGlobalInitializerDummyBase(void)
    {
    }
};
typedef _ZFP_ZFGlobalInitializerDummyBase *(*_ZFP_ZFGlobalInitializerConstructor)(void);
extern ZF_ENV_EXPORT void _ZFP_ZFGlobalInitializerDataRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                                               ZF_IN const zfchar *name,
                                                               ZF_IN ZFLevel level,
                                                               ZF_IN _ZFP_ZFGlobalInitializerConstructor constructor);
extern ZF_ENV_EXPORT void _ZFP_ZFGlobalInitializerDataUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                                                 ZF_IN const zfchar *name,
                                                                 ZF_IN ZFLevel level);
extern ZF_ENV_EXPORT _ZFP_ZFGlobalInitializerDummyBase *&_ZFP_ZFGlobalInitializerInstanceHolderAccess(ZF_IN const zfchar *name,
                                                                                                      ZF_IN ZFLevel level);

zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFGlobalInitializerRegisterHolder
{
public:
    _ZFP_ZFGlobalInitializerRegisterHolder(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                           ZF_IN const zfchar *name,
                                           ZF_IN ZFLevel level,
                                           ZF_IN _ZFP_ZFGlobalInitializerConstructor constructor)
    : ZFCoreLibDestroyFlag(ZFCoreLibDestroyFlag)
    , name(name)
    , level(level)
    {
        _ZFP_ZFGlobalInitializerDataRegister(ZFCoreLibDestroyFlag, name, level, constructor);
    }
    ~_ZFP_ZFGlobalInitializerRegisterHolder(void)
    {
        _ZFP_ZFGlobalInitializerDataUnregister(ZFCoreLibDestroyFlag, name, level);
    }
private:
    zfbool *ZFCoreLibDestroyFlag;
    zfstring name;
    ZFLevel level;
};
#define _ZFP_ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_) \
    zfclassNotPOD _ZFP_ZFGlobalInitializer_##Name : zfextendsNotPOD _ZFP_ZFGlobalInitializerDummyBase \
    { \
    protected: \
        typedef _ZFP_ZFGlobalInitializer_##Name zfself; \
    public: \
        static zfbool _ZFP_ZFGlobalInitializerRegister(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static _ZFP_ZFGlobalInitializerRegisterHolder _d( \
                &ZFCoreLibDestroyFlag, \
                ZFMACRO_TOSTRING(Name), \
                ZFLevel_, \
                _ZFP_ZFGlobalInitializer_##Name::_ZFP_ZFGlobalInitializerNewInstance); \
            return zftrue; \
        } \
        static _ZFP_ZFGlobalInitializerDummyBase *_ZFP_ZFGlobalInitializerNewInstance(void) \
        { \
            return zfnew(_ZFP_ZFGlobalInitializer_##Name); \
        } \
        static _ZFP_ZFGlobalInitializer_##Name *_ZFP_ZFGlobalInitializerInstanceAccess(void) \
        { \
            static _ZFP_ZFGlobalInitializerDummyBase *&instance = \
                _ZFP_ZFGlobalInitializerInstanceHolderAccess( \
                    ZFMACRO_TOSTRING(Name), \
                    ZFLevel_); \
            if(instance == zfnull) \
            { \
                (void)_ZFP_ZFGlobalInitializerInstanceHolderAccess( \
                    ZFMACRO_TOSTRING(Name), \
                    ZFLevel_); \
            } \
            return ZFCastStatic(_ZFP_ZFGlobalInitializer_##Name *, instance); \
        } \
        _ZFP_ZFGlobalInitializer_##Name(void)
/**
 * @brief code block that will be executed when app start and stop
 *
 * usage:
 * @code
 *   // in a *.cpp file
 *   ZF_GLOBAL_INITIALIZER_INIT(SomeName)
 *   : data('c') // you can init member here
 *   {
 *       // code that will be executed when app start
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(SomeName)
 *   {
 *       // code that will be executed when app stop
 *   }
 *   private:
 *       zfcharA data; // you can add member here
 *   private:
 *       void func() {} // you can add function here
 *   ZF_GLOBAL_INITIALIZER_END(SomeName)
 * @endcode
 * \n
 * @warning if you have more than one initializer with same level,
 *   the execute order of the code block is not ensured
 * @warning init and destroy step is not one time,
 *   they'll be called each time ZFFrameworkInit/ZFFrameworkCleanup is called
 */
#define ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_) \
    _ZFP_ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_)

/**
 * @brief see #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_GLOBAL_INITIALIZER_INIT(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevelAppNormal)

#define _ZFP_ZF_GLOBAL_INITIALIZER_DESTROY(Name) \
    public: \
        virtual ~_ZFP_ZFGlobalInitializer_##Name(void)
/**
 * @brief see #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_GLOBAL_INITIALIZER_DESTROY(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_DESTROY(Name)

#define _ZFP_ZF_GLOBAL_INITIALIZER_END(Name) \
    }; \
    template<typename D> \
    class _ZFP_ZFGlobalInitializerAutoInit_##Name \
    { \
    public: \
        class exec_register \
        { \
        public: \
            exec_register(void) \
            { \
                _ZFP_ZFGlobalInitializer_##Name::_ZFP_ZFGlobalInitializerRegister(); \
            } \
        }; \
        template<exec_register &> class ref_it {}; \
        static exec_register register_object; \
        static ref_it<register_object> referrer; \
    }; \
    template<typename D> \
    typename _ZFP_ZFGlobalInitializerAutoInit_##Name<D>::exec_register _ZFP_ZFGlobalInitializerAutoInit_##Name<D>::register_object; \
    class _ZFP_ZFGlobalInitializerAutoInitDummyClass_##Name : _ZFP_ZFGlobalInitializerAutoInit_##Name<_ZFP_ZFGlobalInitializerAutoInitDummyClass_##Name> \
    {};
/**
 * @brief see #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_GLOBAL_INITIALIZER_END(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_END(Name)

#define _ZFP_ZF_GLOBAL_INITIALIZER_INSTANCE(Name) \
    (_ZFP_ZFGlobalInitializer_##Name::_ZFP_ZFGlobalInitializerInstanceAccess())
/**
 * @brief access the instance of the initializer
 */
#define ZF_GLOBAL_INITIALIZER_INSTANCE(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_INSTANCE(Name)

#define _ZFP_ZF_GLOBAL_INITIALIZER_CLASS(Name) \
    _ZFP_ZFGlobalInitializer_##Name
/**
 * @brief the class holder of the global initializer
 */
#define ZF_GLOBAL_INITIALIZER_CLASS(Name) \
    _ZFP_ZF_GLOBAL_INITIALIZER_CLASS(Name)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreGlobalInitializerDef_h_

