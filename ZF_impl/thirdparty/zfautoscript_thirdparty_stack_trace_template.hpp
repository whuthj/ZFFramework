#ifndef _ZFI_thirdparty_stack_trace_hpp_
#define _ZFI_thirdparty_stack_trace_hpp_

#include "ZFCore/protocol/ZFProtocolZFLogStackTrace.h"
#include "ZFCore/ZFLog_StackTrace.h"

#if ZFLOG_STACKTRACE_SUPPORT && !ZF_ENV_sys_Android

#include "../../../thirdparty/_tmp/stack_trace/dbg/stack.hpp"

#endif

#endif // #ifndef _ZFI_thirdparty_stack_trace_hpp_

