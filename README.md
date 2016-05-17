# Introduction

welcome to [ZFFramework](http://ZFFramework.com),
a cross-platform, lightweight, mid-level application framework in C++

everything here starts with "ZF", which stands for "Zero Framework"
* it's a framework, you are able to write all of your code under ZFFramework to build your app quickly
* it's not a traditional framework, it's able to be loaded like a dynamic library, plug and play

    you are able to embed ZFFramework to your native framework, or embed your native code to ZFFramework

* as a mid-level framework, it's designed to be able to run at any platform that supplies C++03

    normal app, OpenGL app, or even text-based console app,
    everything's done if you are able to supply your own implementation


# Quick overview

this piece of code shows how to show a hello world on UI and log output

``` c++
#include "ZFUIKit.h" // for UI module

ZFMAIN_ENTRY(params) // app starts from here
{
    // show a hello world as a text view
    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();
    zfblockedAlloc(ZFUITextView, textView);
    window->childAdd(textView);
    textView->textContentStringSet(zfText("hello world"));

    // show a hello world to log output
    zfLogT() << zfText("hello wolrd");

    return 0;
}
```


# Online document

[click here for the full document](http://ZFFramework.com)

