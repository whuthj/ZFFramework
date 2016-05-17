/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFMainEntry_sys_Qt_h_
#define _ZFI_ZFMainEntry_sys_Qt_h_

#include "ZFCore.h"
#include "ZFImpl_sys_Qt.h"

#if ZF_ENV_sys_Qt

#include <QWidget>
#include <QLibrary>

class ZFImpl_sys_Qt_Window : public QWidget
{
    Q_OBJECT

public:
    ZFImpl_sys_Qt_Window(void);
};

extern ZF_ENV_EXPORT int ZFMainEntry_sys_Qt(int argc, char **argv);
#define ZFImpl_sys_Qt_requireLib(libName) \
    QLibrary _lib##libName(#libName); \
    _lib##libName.load();

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFImpl_sys_Qt_Window *ZFImpl_sys_Qt_rootWindow(void);

ZF_NAMESPACE_GLOBAL_END

#endif
#endif // #ifndef _ZFI_ZFMainEntry_sys_Qt_h_

