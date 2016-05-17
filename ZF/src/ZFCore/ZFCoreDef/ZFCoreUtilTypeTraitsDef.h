/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreUtilTypeTraitsDef.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilTypeTraitsDef_h_
#define _ZFI_ZFCoreUtilTypeTraitsDef_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief type traits similar to STL's traits
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftypeTraits
{
public:
    typedef T_Type Traits; /**< @brief original type */
    typedef T_Type & Reference; /**< @brief reference type */
    typedef T_Type const &ConstReference; /**< @brief const reference type */
    typedef T_Type * Pointer; /**< @brief pointer type */
    typedef const T_Type * ConstPointer; /**< @brief const pointer type */
};
/**
 * @brief type traits similar to STL's traits
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftypeTraits<T_Type *>
{
public:
    typedef T_Type Traits; /**< @brief original type */
    typedef T_Type & Reference; /**< @brief reference type */
    typedef T_Type const &ConstReference; /**< @brief const reference type */
    typedef T_Type * Pointer; /**< @brief pointer type */
    typedef const T_Type * ConstPointer; /**< @brief const pointer type */
};
/**
 * @brief type traits similar to STL's traits
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftypeTraits<const T_Type *>
{
public:
    typedef T_Type Traits; /**< @brief original type */
    typedef T_Type & Reference; /**< @brief reference type */
    typedef T_Type const &ConstReference; /**< @brief const reference type */
    typedef T_Type * Pointer; /**< @brief pointer type */
    typedef const T_Type * ConstPointer; /**< @brief const pointer type */
};
/**
 * @brief type traits similar to STL's traits
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftypeTraits<T_Type &>
{
public:
    typedef T_Type Traits; /**< @brief original type */
    typedef T_Type & Reference; /**< @brief reference type */
    typedef T_Type const &ConstReference; /**< @brief const reference type */
    typedef T_Type * Pointer; /**< @brief pointer type */
    typedef const T_Type * ConstPointer; /**< @brief const pointer type */
};
/**
 * @brief type traits similar to STL's traits
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftypeTraits<T_Type const &>
{
public:
    typedef T_Type Traits; /**< @brief original type */
    typedef T_Type & Reference; /**< @brief reference type */
    typedef T_Type const &ConstReference; /**< @brief const reference type */
    typedef T_Type * Pointer; /**< @brief pointer type */
    typedef const T_Type * ConstPointer; /**< @brief const pointer type */
};

// ============================================================
/**
 * @brief util template to traits the template's param
 *
 * usage:
 * @code
 *   // var0's type would be "vector<int>" 's first template param's type
 *   // int in this example
 *   zftemplateTraits<vector<int> >::TemplateType0 var0;
 *
 *   // var1's type would be "map<int, float>" 's first template param's type
 *   // int in this example
 *   zftemplateTraits<map<int, float> >::TemplateType0 var1;
 *
 *   // var2's type would be "map<int, float>" 's second template param's type
 *   // float in this example
 *   zftemplateTraits<map<int, float> >::TemplateType1 var2;
 * @endcode
 */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT zftemplateTraits
{
};
/** @cond ZFPrivateDoc */
template<template<typename> class T_Template, typename T0>
zfclassNotPOD ZF_ENV_EXPORT zftemplateTraits<T_Template<T0> >
{
public:
    typedef T0 TemplateType0;
};
template<template<typename, typename> class T_Template, typename T0, typename T1>
zfclassNotPOD ZF_ENV_EXPORT zftemplateTraits<T_Template<T0, T1> >
{
public:
    typedef T0 TemplateType0;
    typedef T1 TemplateType1;
};
template<template<typename, typename, typename> class T_Template, typename T0, typename T1, typename T2>
zfclassNotPOD ZF_ENV_EXPORT zftemplateTraits<T_Template<T0, T1, T2> >
{
public:
    typedef T0 TemplateType0;
    typedef T1 TemplateType1;
    typedef T2 TemplateType2;
};
template<template<typename, typename, typename, typename> class T_Template, typename T0, typename T1, typename T2, typename T3>
zfclassNotPOD ZF_ENV_EXPORT zftemplateTraits<T_Template<T0, T1, T2, T3> >
{
public:
    typedef T0 TemplateType0;
    typedef T1 TemplateType1;
    typedef T2 TemplateType2;
    typedef T3 TemplateType3;
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilTypeTraitsDef_h_

