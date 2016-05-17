#include "ZFCore/protocol/ZFProtocolZFLogStackTrace.h"
#include "ZFCore/ZFLog_StackTrace.h"

#if ZFLOG_STACKTRACE_SUPPORT && !ZF_ENV_sys_Android

#include "../../../thirdparty/_tmp/stack_trace/stack.cpp"

#endif

