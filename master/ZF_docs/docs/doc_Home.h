/**
 * @mainpage Welcome
 *
 * @section DocTag_Home_Introduction Introduction
 * welcome to ZFFramework, a cross-platform, lightweight, mid-level application framework in C++\n
 * \n
 * everything here starts with "ZF", which stands for "Zero Framework"
 * -  it's a framework, you are able to write all of your code under ZFFramework to build your app quickly\n
 * -  it's not a traditional framework, it's able to be loaded like a dynamic library, plug and play\n
 *   you are able to embed ZFFramework to your native framework, or embed your native code to ZFFramework
 * -  as a mid-level framework, it's designed to be able to run at any platform that supplies C++03\n
 *   normal app, OpenGL app, or even text-based console app,
 *   everything's done if you are able to supply your own implementation
 *
 * @section DocTag_Home_QuickOverview Quick overview
 * this piece of code shows how to show a hello world on UI and log output
 * @code
 *   #include "ZFUIKit.h" // for UI module
 *
 *   ZFMAIN_ENTRY(params) // app starts from here
 *   {
 *       // show a hello world as a text view
 *       zfblockedAlloc(ZFUIWindow, window);
 *       window->windowShow();
 *       zfblockedAlloc(ZFUITextView, textView);
 *       window->childAdd(textView);
 *       textView->textContentStringSet(zfText("hello world"));
 *
 *       // show a hello world to log output
 *       zfLogT() << zfText("hello wolrd");
 *
 *       return 0;
 *   }
 * @endcode
 * \n
 * and here are screenshot of demo 2048 game built by ZFFramework:\n
 * <div>
 *     <div style="float:left; border:20px solid transparent;">
 *         <div style="float:left; border:5px solid transparent;">
 *             <img src="http://ZFFramework.com/res/ZFFramework/ZF2048_iOS.png">
 *         </div>
 *         <div style="float:left; border:5px solid transparent;">
 *             <img src="http://ZFFramework.com/res/ZFFramework/ZF2048_Android.png">
 *         </div>
 *         <div style="float:left; border:5px solid transparent;">
 *             <img src="http://ZFFramework.com/res/ZFFramework/ZF2048_Qt_Windows.png">
 *         </div>
 *         <div style="float:left; border:5px solid transparent;">
 *             <img src="http://ZFFramework.com/res/ZFFramework/ZF2048_Qt_MacOS.png">
 *         </div>
 *         <div style="float:left; border:5px solid transparent;">
 *             <img src="http://ZFFramework.com/res/ZFFramework/ZF2048_Qt_Ubuntu.png">
 *         </div>
 *     </div>
 * </div>
 * \n
 *
 * @section DocTag_Home_Requirement Requirement
 * -  for the core modlue:
 *   -  C++03 compatible compiler (require templates, no boost/RTTI/exceptions required)
 *   -  STL containers (require: map/unordered_map/vector/deque/list), or supply custom wrapper
 * -  for the implementation module:
 *   -  depends on the actual platform implementation
 *
 * @section DocTag_Home_MainFeature Main features
 * -  minimum requirement
 * -  built-in reflection, serialzation, styleable, leak test
 * -  "core + protocol + dynamic implementation" design\n
 *   support any platform if you are able to
 *   supply a native C++ implementation,
 *   most of implementation can be replaced easily,
 *   and implementation is required only if its owner module being used
 * -  easy to communicate with native code\n
 *   even to embed UI elements and native UI elements with each other
 * -  UI module to write cross-platform UI easily
 * -  built-in auto scale logic to support multiple screen size\n
 *   you have no need to write size-dependent code
 *   in both app and implementation
 *
 * @subsection DocTag_Home_WhatWeDo What we do
 * -  aiming to be portable and can be ported easily,
 *   aiming to be lightweighted and able to be embeded easily,
 *   aiming to use 20% code to do 80% work
 * -  supply Java-like / ObjectC-like app level APIs to
 *   build up small/medium sized app easily
 *
 * @subsection DocTag_Home_WhatWeWontDo What we won't do
 * -  we won't supply functional libraries such as boost,
 *   however, you may still easily use them
 * -  we won't supply all-in-one framework to do everything,
 *   we are more likely to be a mid-level framework
 *
 * @section DocTag_Home_GettingStarted Getting started
 * -  @ref DocTag_Download "Download" necessary files
 * -  @ref DocTag_Setup "Setup" set up necessary environment for ZFFramework
 * -  @ref DocTag_Tutorial "Tutorial" quick tutorial to code with ZFFramework
 * -  @ref DocTag_FAQ "FAQ"
 *
 *
 * @section DocTag_Home_License License
 * ZFFramework is under MIT license
 * (<a href="https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt" target="_blank">see here</a>),
 * feel free to copy or modify or use it,
 * except that you must leave the license unmodified\n
 * \n
 * project home page: <a href="http://ZFFramework.com" target="_blank">http://ZFFramework.com</a>\n
 * blog: <a href="http://zsaber.com" target="_blank">http://zsaber.com</a>\n
 * \n
 * if you like my work, please consider donate:
 * -  <a href="http://paypal.com" target="_blank">paypal</a>: zerofighter@163.com
 * -  <a href="http://alipay.com" target="_blank">alipay</a>: zerofighter@163.com
 *
 * contact master@zsaber.com (Chinese or English only) before donate,
 * we would really appreciate for your help
 */

