#include "log.h"
#include <stdio.h>
#include <stdarg.h>

namespace rt3 {

static FILE* g_log;

void LogOpen(const char* path) { g_log = fopen(path, "w"); }

void Log(const char* fmt, ...) {
    if (!g_log) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_log, fmt, ap);
    va_end(ap);
    fputc('\n', g_log);
    fflush(g_log);
}

} // namespace rt3
