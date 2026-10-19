#pragma once
#include <stdio.h>

extern int g_fail, g_pass;
extern FILE* g_out;

struct TestCase { const char* name; void (*fn)(); TestCase* next; };
struct TestReg { TestReg(const char* name, void (*fn)()); };

#define DIFFTEST(name) \
    static void name(); static TestReg reg_##name(#name, name); static void name()

// Reports the first few mismatches per run, then stays quiet.
#define EXPECT_EQ(a, b, ...) do { \
    auto va_ = (a); auto vb_ = (b); \
    if (!(va_ == vb_)) { if (++g_fail < 50) { fprintf(g_out, "  mismatch %s:%d: ", __FILE__, __LINE__); \
        fprintf(g_out, __VA_ARGS__); fprintf(g_out, "\n"); } } \
} while (0)
