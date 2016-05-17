/**
 * @page DocTag_Setup_Qt Qt
 *
 * you must have these settings being prepared:
 * -  have qmake and proper mingw make in your system path
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup "Quick setup" for quick setup your project files\n
 * \n
 * \n
 * if you really want to setup manually:
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF and ~/ZF_impl
 * -# create your own Qt project (*.pro)
 * -# have this line in your pro file:
 *   @code
 *     QT += core gui widgets webkitwidgets
 *
 *     CONFIG(debug, debug|release) {
 *         DEFINES += DEBUG
 *     }
 *   @endcode
 * -# specify include path and lib search path in your pro file:
 *   @code
 *     equals(ZF_BUILD_STATIC_LIB, 1) {
 *         macx {
 *             _ZF_LINKER_FLAGS = -Wl,-all_load
 *         } else {
 *             _ZF_LINKER_FLAGS = -Wl,--whole-archive
 *         }
 *     } else {
 *         _ZF_LINKER_FLAGS =
 *     }
 *
 *     INCLUDEPATH += path_to_ZFFramework_release/$$ZF_QT_TYPE/ZF/include
 *     INCLUDEPATH += path_to_ZFFramework_release/$$ZF_QT_TYPE/ZF_impl/include
 *     INCLUDEPATH += path_to_ZFFramework_release/$$ZF_QT_TYPE/ZF_impl_ZFUIWebKit/include
 *
 *     LIBS += -Lpath_to_ZFFramework_release/$$ZF_QT_TYPE/ZF/lib $$_ZF_LINKER_FLAGS -lZFFramework
 *     LIBS += -Lpath_to_ZFFramework_release/$$ZF_QT_TYPE/ZF_impl/lib $$_ZF_LINKER_FLAGS -lZFFramework_impl
 *     LIBS += -Lpath_to_ZFFramework_release/$$ZF_QT_TYPE/ZF_impl_ZFUIWebKit/lib $$_ZF_LINKER_FLAGS -lZFFramework_impl_ZFUIWebKit
 *   @endcode
 *   note: if you build ZFFramework as static library, you must specify -all_load for MacOS and --whole-archive for linux
 * -# manually add loader routine to your project:
 *   @code
 *     #include "sys_Qt/ZFMainEntry_sys_Qt.h"
 *
 *     int main(int argc, char **argv)
 *     {
 *         ZFImpl_sys_Qt_requireLib(ZFFramework)
 *         ZFImpl_sys_Qt_requireLib(ZFFramework_impl)
 *         ZFImpl_sys_Qt_requireLib(ZFFramework_impl_ZFUIWebKit)
 *         return ZFMainEntry_sys_Qt(argc, argv);
 *     }
 *   @endcode
 * -# copy all res and libs to your product output or proper location accorrding to your platform
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial "Tutorial" to code with ZFFramework, enjoy
 */

