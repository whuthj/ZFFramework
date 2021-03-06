/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file zfstl_hashmap.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstl_hashmap_h_
#define _ZFI_zfstl_hashmap_h_

#include "zfstl_pair.h"

// ============================================================
/** @brief stl wrapper */
ZF_ENV_SENSITIVE
#ifndef zfstlhashmap
    #if (defined(_MSC_VER) && ((_MSC_VER < 1500) || !_HAS_TR1)) // old hash_map of MSVC
        #define zfstlhashmap _ZFP_zfstlhashmap

        #include <hash_map>
        template<typename T_Key, typename T_Hash, typename T_EqualTo>
        class _ZFP_zfstlhashmap_hashCompare
        {
            public:
                enum {bucket_size = 4, min_buckets = 8};
            public:
                size_t operator()(T_Key const &_Keyval) const
                {
                    long _Quot = _hashFunc(_Keyval) & LONG_MAX;
                    ldiv_t _Qrem = ldiv(_Quot, 127773);
                    _Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
                    if (_Qrem.rem < 0) {_Qrem.rem += LONG_MAX;}
                    return ((size_t)_Qrem.rem);
                }
                bool operator()(T_Key const &_Keyval1, T_Key const &_Keyval2) const
                {
                    return !_equalToFunc(_Keyval1, _Keyval2);
                }
            private:
                T_Hash _hashFunc;
                T_EqualTo _equalToFunc;
        };
        template<typename T_Key, typename T_Value, typename T_Hash, typename T_EqualTo>
        class _ZFP_zfstlhashmap : public stdext::hash_map<T_Key, T_Value, _ZFP_zfstlhashmap_hashCompare<T_Key, T_Hash, T_EqualTo> >
        {
        };
    #elif (defined(ANDROID) || defined(__ANDROID__)) // Android JNI stl_port
        #define zfstlhashmap std::tr1::unordered_map

        #include <unordered_map>
    #else
        #define zfstlhashmap std::unordered_map

        #include "zfstl_impl/unordered_map.hpp"
    #endif
#endif // #ifndef zfstlhashmap

#endif // #ifndef _ZFI_zfstl_hashmap_h_

