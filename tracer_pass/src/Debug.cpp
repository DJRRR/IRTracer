#include "Debug.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_os_ostream.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#include <set>

using namespace llvm;


static const char *warningPrefix = "WARNING";
static const char *warningOncePrefix = "WARNING ONCE";
static const char *errorPrefix = "ERROR";
static const char *notePrefix = "NOTE";


static bool shouldSetColor(const char *pfx, const char *msg,
                           const char *prefixToSearchFor) {
  if (pfx && strcmp(pfx, prefixToSearchFor) == 0)
    return true;

  if (llvm::StringRef(msg).startswith(prefixToSearchFor))
    return true;

  return false;
}

static void tracer_vfmessage(FILE *fp, const char *pfx, const char *msg,
                           va_list ap) {
  if (!fp)
    return;

  llvm::raw_fd_ostream fdos(fileno(fp), /*shouldClose=*/false,
                            /*unbuffered=*/true);
  bool modifyConsoleColor = fdos.is_displayed() && (fp == stderr);

  if (modifyConsoleColor) {

    // Warnings
    if (shouldSetColor(pfx, msg, warningPrefix))
      fdos.changeColor(llvm::raw_ostream::MAGENTA,
                       /*bold=*/false,
                       /*bg=*/false);

    // Once warning
    if (shouldSetColor(pfx, msg, warningOncePrefix))
      fdos.changeColor(llvm::raw_ostream::MAGENTA,
                       /*bold=*/true,
                       /*bg=*/false);

    // Errors
    if (shouldSetColor(pfx, msg, errorPrefix))
      fdos.changeColor(llvm::raw_ostream::RED,
                       /*bold=*/true,
                       /*bg=*/false);

    // Notes
    if (shouldSetColor(pfx, msg, notePrefix))
      fdos.changeColor(llvm::raw_ostream::WHITE,
                       /*bold=*/true,
                       /*bg=*/false);
  }

  fdos << "TRACER: ";
  if (pfx)
    fdos << pfx << ": ";

  // FIXME: Can't use fdos here because we need to print
  // a variable number of arguments and do substitution
  vfprintf(fp, msg, ap);
  fflush(fp);

  fdos << "\n";

  if (modifyConsoleColor)
    fdos.resetColor();

  fdos.flush();
}


static void tracer_vmessage(const char *pfx, bool onlyToFile, const char *msg,
                          va_list ap) {
    va_list ap2;
    va_copy(ap2, ap);
    tracer_vfmessage(stderr, pfx, msg, ap2);
    va_end(ap2);

}

void tracer_message(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  tracer_vmessage(NULL, false, msg, ap);
  va_end(ap);
}


void tracer_error(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  tracer_vmessage(errorPrefix, false, msg, ap);
  va_end(ap);
  exit(1);
}

void tracer_warning(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  tracer_vmessage(warningPrefix, false, msg, ap);
  va_end(ap);
}

/* Prints a warning once per message. */
void tracer_warning_once(const void *id, const char *msg, ...) {
  static std::set<std::pair<const void *, const char *> > keys;
  std::pair<const void *, const char *> key;

  key = std::make_pair(id, msg);

  if (!keys.count(key)) {
    keys.insert(key);
    va_list ap;
    va_start(ap, msg);
    tracer_vmessage(warningOncePrefix, false, msg, ap);
    va_end(ap);
  }
}

char* getLLVMMsg(llvm::Value* v){
     std::string res("");
     llvm::raw_string_ostream ss(res);
     ss << *v;
     char* buf = new char[res.size()];
     memcpy(buf, res.c_str(), res.size());
     return buf; 
}
 

