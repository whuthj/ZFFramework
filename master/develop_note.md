# Intro

this file is written as a note for developers that want to know the mechanism of ZFFramework

# Contents

## Abbreviations

there are many abbreviations in ZFFramework which keep code style and you should follow:

* `_ZFP_` : `ZF Private` contents, should not be used for public
* `_ZFP_I_` : `ZF Private Internal` contents, which won't be logged by global observers and leak tests
* `_ZFI_` : `ZF Include`, dummy macro to wrap headers
* `_ZFT_` : `ZF Type`, used to mark macro defined types
* `zfs` : `ZF String`, core string utils
* `zfm` : `ZF Math`, core math utils


## ZFCastZFObject

we use ZFCastZFObject for dynamic type cast, instead of traditional dynamic_cast

the main reason for that is the performance, ZFFramework use multi-inheritance to simulate interface logic,
which would cause deep inherit tree, which cause dynamic_cast to be very slow


## Styles

ZFFramework supply powerful serialization and style logic,
however, it cost much CPU for property comparing and copying

so, for performance, most of styleable object won't copy it's default style during init,
and most of default style are hard coded, only images are loaded from resource
