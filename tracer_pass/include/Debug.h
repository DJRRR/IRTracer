/* Reference: klee - error handling */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <string>
#include "llvm/IR/Value.h"

/// Print "TRACER: ERROR: " followed by the msg in printf format and a
/// newline on stderr, then exit with an error.
void tracer_error(const char *msg, ...)
    __attribute__((format(printf, 1, 2), noreturn));

/// Print "TRACER: " followed by the msg in printf format and a
/// newline on stderr and to messages.txt.
void tracer_message(const char *msg, ...) __attribute__((format(printf, 1, 2)));

/// Print "TRACER: WARNING: " followed by the msg in printf format and a
/// newline on stderr.
void tracer_warning(const char *msg, ...) __attribute__((format(printf, 1, 2)));

/// Print "TRACER: WARNING: " followed by the msg in printf format and a
/// newline on stderr. However, the warning is only
/// printed once for each unique (id, msg) pair (as pointers).
void tracer_warning_once(const void *id, const char *msg, ...)
    __attribute__((format(printf, 2, 3)));

char* getLLVMMsg(llvm::Value* v);

#endif /* __DEBUG_H__ */
