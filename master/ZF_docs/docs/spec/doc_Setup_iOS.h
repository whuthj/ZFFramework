/**
 * @page DocTag_Setup_iOS iOS
 *
 * you must have these settings being prepared:
 * -  have XCode installed
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup "Quick setup" for quick setup your project files\n
 * \n
 * \n
 * if you really want to setup manually:
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF and ~/ZF_impl
 * -# create your own iOS project
 * -# drag and drop .a file(s) to your project:
 *   -  libZFFramework.a\n
 *     core module of ZFFramework\n
 *     supply no implementation
 *   -  libZFFramework_impl.a\n
 *     default implementation\n
 *     suit for most case,
 *     you should supply your own project and AppDelegate,
 *     and invoke all methods declared in ZFMainEntry_sys_iOS.h
 *   -  libZFFramework_loader.a\n
 *     default AppDelegate entry\n
 *     this is a convenient loader that supply a default AppDelegate entry,
 *     you should supply your own project but remove the default generated
 *     AppDelegate and RootViewController
 * -# modify your project's target settings:
 *   -  Header Search Paths:\n
 *     $(SRCROOT)/relative_path_to_ZFFramework/include
 *   -  Library Search Paths:\n
 *     $(SRCROOT)/relative_path_to_ZFFramework/lib
 *   -  Other Linker Flags:\n
 *     "-Objc -all_load"
 *   -  Symbols Hidden by Default:\n
 *     Yes
 * -# ensure these libraries seleted in "Link Binary With Libraries" of XCode's target settings:
 *   -  CoreGraphics.framework
 *   -  QuartzCore.framework
 *   -  UIKit.framework
 *   -  Foundation.framework
 *   -  libZFFramework.a, libZFFramework_impl.a, libZFFramework_loader.a
 * -# add "New Run Script Phase" in your project's target's "Build Phases"
 *   @code
 *     mkdir -p "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/res" >/dev/null 2>&1
 *     cp -rf "${SRCROOT}/relative_path_to_ZFFramework/_release/iOS/ZF/res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/res/" >/dev/null 2>&1
 *     cp -rf "${SRCROOT}/relative_path_to_ZFFramework/_release/iOS/ZF_impl/res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/res/" >/dev/null 2>&1
 *     cp -rf "${SRCROOT}/relative_path_to_ZFFramework/_release/iOS/ZF_impl_ZFUIWebKit/res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/res/" >/dev/null 2>&1
 *     cp -rf "${SRCROOT}/relative_path_to_your_project_res/res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/res/" >/dev/null 2>&1
 *   @endcode
 *   ensure all your project's resource files put under the res directory,
 *   and all of them must be copied by the script above,
 *   you should not add them to your project
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial "Tutorial" to code with ZFFramework, enjoy
 */

