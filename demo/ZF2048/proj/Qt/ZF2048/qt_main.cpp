/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "sys_Qt/ZFMainEntry_sys_Qt.h"

int main(int argc, char **argv)
{
    #if 1
        ZFImpl_sys_Qt_requireLib(ZFFramework)
    #endif
    #if 1
        ZFImpl_sys_Qt_requireLib(ZFFramework_impl)
    #endif
    #if 0
        ZFImpl_sys_Qt_requireLib(ZFFramework_impl_ZFUIWebKit)
    #endif
    return ZFMainEntry_sys_Qt(argc, argv);
}

