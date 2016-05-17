/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreStaticInitializerDef.h
 * @brief static initializer step
 */
#ifndef _ZFI_ZFCoreStaticInitializerDef_h_
#define _ZFI_ZFCoreStaticInitializerDef_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreLevelDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFStaticInitializerDummyBase
{
public:
    virtual ~_ZFP_ZFStaticInitializerDummyBase(void)
    {
    }
};
typedef _ZFP_ZFStaticInitializerDummyBase *(*_ZFP_ZFStaticInitializerConstructor)(void);
extern ZF_ENV_EXPORT _ZFP_ZFStaticInitializerDummyBase *_ZFP_ZFStaticInitializerAccessInstance(ZF_IN const zfchar *name,
                                                                                               ZF_IN ZFLevel level,
                                                                                               ZF_IN _ZFP_ZFStaticInitializerConstructor constructor);

// ============================================================
/**
 * @brief static initializer
 *
 * called before any other active code,
 * must be used in a cpp file\n
 * typical usage:
 * @code
 *   // in a cpp file only
 *   ZF_STATIC_INITIALIZER_INIT(YourInitName)
 *   : privateMember() // you may add member init here
 *   {
 *       // add your own init phase here
 *       // note: never access ZFFramework member here
 *   }
 *   ZF_STATIC_INITIALIZER_DESTROY(YourInitName)
 *   {
 *       // add your own cleanup phase here
 *       // note: never access ZFFramework member here
 *   }
 *   private:
 *       zfbool privateMember; // you may add member declaration here
 *       void func(void) {} // you may add private function here
 *   ZF_STATIC_INITIALIZER_END(YourInitName)
 * @endcode
 * \n
 * init level can be changed by #ZFLevel\n
 * \n
 * @note if more than one initializer exists,
 *   the order of the initialization is not ensured,
 *   however, if level is specified,
 *   the clean up order is ensured between different levels
 *   (the order in same levels also not ensured)
 * @note use #ZF_GLOBAL_INITIALIZER_INIT is recommended at most of case
 * @warning you should not access any of ZFFramework members
 *   in the static initializer,
 *   you may check it by #ZFFrameworkStateCheck
 */
#define ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevel_) \
    zfclassNotPOD _ZFP_ZFStaticInitializer_##Name : zfextendsNotPOD _ZFP_ZFStaticInitializerDummyBase \
    { \
    public: \
        static _ZFP_ZFStaticInitializerDummyBase *_ZFP_ZFStaticInitializerConstructor_##Name(void) \
        { \
            return zfnew(_ZFP_ZFStaticInitializer_##Name); \
        } \
        static _ZFP_ZFStaticInitializer_##Name *_ZFP_ZFStaticInitializerInstance(void) \
        { \
            static _ZFP_ZFStaticInitializer_##Name *instance = ZFCastStatic(_ZFP_ZFStaticInitializer_##Name *, \
                (_ZFP_ZFStaticInitializerAccessInstance( \
                    zfText(#Name), \
                    ZFLevel_, \
                    _ZFP_ZFStaticInitializer_##Name::_ZFP_ZFStaticInitializerConstructor_##Name)) \
                ); \
            return instance; \
        } \
    public: \
        _ZFP_ZFStaticInitializer_##Name(void)
/**
 * @brief see #ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_STATIC_INITIALIZER_INIT(Name) \
    ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(Name, ZFLevelAppNormal)
/**
 * @brief see #ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_STATIC_INITIALIZER_DESTROY(Name) \
        virtual ~_ZFP_ZFStaticInitializer_##Name(void)
/**
 * @brief see #ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL
 */
#define ZF_STATIC_INITIALIZER_END(Name) \
    }; \
    template<typename D> \
    class _ZFP_ZFStaticInitializerAutoInit_##Name \
    { \
    public: \
        class exec_register \
        { \
        public: \
            exec_register(void) \
            { \
                _ZFP_ZFStaticInitializer_##Name::_ZFP_ZFStaticInitializerInstance(); \
            } \
        }; \
        template<exec_register &> class ref_it {}; \
        static exec_register register_object; \
        static ref_it<register_object> referrer; \
    }; \
    template<typename D> \
    typename _ZFP_ZFStaticInitializerAutoInit_##Name<D>::exec_register _ZFP_ZFStaticInitializerAutoInit_##Name<D>::register_object; \
    class _ZFP_ZFStaticInitializerAutoInitDummyClass_##Name : _ZFP_ZFStaticInitializerAutoInit_##Name<_ZFP_ZFStaticInitializerAutoInitDummyClass_##Name> \
    {};

/**
 * @brief access the instance of the initializer
 */
#define ZF_STATIC_INITIALIZER_INSTANCE(Name) \
    (_ZFP_ZFStaticInitializer_##Name::_ZFP_ZFStaticInitializerInstance())

#define _ZFP_ZF_STATIC_INITIALIZER_CLASS(Name) \
    _ZFP_ZFStaticInitializer_##Name
/**
 * @brief the class holder of the static initializer
 */
#define ZF_STATIC_INITIALIZER_CLASS(Name) \
    _ZFP_ZF_STATIC_INITIALIZER_CLASS(Name)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStaticInitializerDef_h_

