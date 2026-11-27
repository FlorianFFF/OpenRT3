
namespace rt3 {
@DECLS@
}
namespace {
@FAKES@
} // namespace
namespace rt3 {
@CASEDECLS@
}

// Watched globals of the @TAG@ callbacks (pointer ones get a buffer).
static const uint32_t k@TAG@Ptr[] = { @PTRS@ };
static const uint32_t k@TAG@Int[] = { @INTS@ };

DIFFTEST(game_SmallCallbacks@TAG@) {
    UiTrace t;
@ADDS@
    t.Add(0x5193f0, (void*)&rt3::GetString, (void*)&FakeB399Str);
    static const GenCase cases[] = {
@CASES@
    };
    const int NC = (int)(sizeof cases / sizeof cases[0]);
    const int NP = (int)(sizeof k@TAG@Ptr / 4), NI = (int)(sizeof k@TAG@Int / 4);
    uint32_t savedP[sizeof k@TAG@Ptr / 4], savedI[sizeof k@TAG@Int / 4];
    for (int i = 0; i < NP; ++i) savedP[i] = *(uint32_t*)k@TAG@Ptr[i];
    for (int i = 0; i < NI; ++i) savedI[i] = *(uint32_t*)k@TAG@Int[i];
    for (int k = 0; k < 64; ++k) g_genVtbl[k] = (void*)&GenFake@TAG@_v0;
    g_genVtbl[22] = (void*)&GenFake@TAG@_v22, g_genVtbl[26] = (void*)&GenFake@TAG@_v26, g_genVtbl[32] = (void*)&GenFake@TAG@_vp;
    g_genVtbl[2] = (void*)&GenFake@TAG@_v2; // vtable +8: Collection remove (one argument)
@BEFORE@
    static const uint32_t kSmall[6] = { 0, 1, 2, 0xffffffff, 0x3c, 0x12 };
    for (int it = 0; it < 120 * NC; ++it) {
        const GenCase& c = cases[it % NC];
        std::string logs[2];
        for (int s = 0; s < 2; ++s) {
            g_genBits = (uint32_t)it * 2654435761u + 0x@TAG@;
            for (int o = 0; o < 4; ++o)
                for (int k = 0; k < 0x100; ++k) {
                    const uint32_t r = GenDraw() % 4;
                    ((uint32_t*)g_genObj[o])[k] = r < 2 ? (uint32_t)(uintptr_t)g_genObj[GenDraw() % 4] : r == 2 ? kSmall[GenDraw() % 6] : GenDraw();
                }
            for (int o = 0; o < 4; ++o) {
                ((void**)g_genObj[o])[0] = g_genVtbl;
                ((uint32_t*)g_genObj[o])[1] = (uint32_t)(uintptr_t)g_genObj[(o + 1) % 4];
                ((uint32_t*)g_genObj[o])[2] = (uint32_t)(uintptr_t)g_genObj[(o + 2) % 4];
                ((int32_t*)g_genObj[o])[3] = (int32_t)(GenDraw() % 3) - 1;
                memcpy(g_genObj[o] + 0x40, g_genObj[o], 0x10); // 'p' may point 0x40 in: the same header
            }
            for (int b = 0; b < 4; ++b) memset(g_genBuf[b], 0, 0x2000);
            *(int32_t*)(g_genBuf[0] + 0x15dd) = 8, *(uint8_t**)(g_genBuf[0] + 0x165d) = g_genBuf[1];
            for (int i = 0; i < NP; ++i) {
                uint8_t* b = g_genBuf[4 + i % 20];
                memset(b, 0, 0x2000); // the whole hashed range: both sides start alike
                ((void**)b)[0] = g_genVtbl;
                *(uint32_t*)k@TAG@Ptr[i] = GenDraw() % 5 ? (uint32_t)(uintptr_t)b : 0;
            }
@NONNULL@
            for (int i = 0; i < NI; ++i) *(uint32_t*)k@TAG@Int[i] = kSmall[GenDraw() % 6];
            g_genListHead = g_genList[0], *(uint8_t**)g_genList[0] = g_genList[1], *(uint8_t**)g_genList[1] = GenDraw() % 2 ? g_genList[2] : nullptr;
            *(uint8_t**)g_genList[2] = nullptr;
            memset(g_genBig + 0x114000, 0, 0x2000); // the hashed engine fields: both sides start alike
@SETUP@
            g_genF = (float)(GenDraw() % 100) / 4.0f;
            const char* sp = c.spec;
            const int noret = *sp == 'V';
            if (noret) ++sp;
            const int fp = *sp == 'F';
            if (fp) ++sp;
            auto value = [&](char k) -> uint32_t {
@VALUES@
                if (k == 'm') return (uint32_t)(uintptr_t)g_genBuf[0];
                if (k == 'q') return (uint32_t)(uintptr_t)g_genBuf[2];
                if (k == 'r') return (uint32_t)(uintptr_t)g_genBuf[3];
                if (k == 'c') return GenDraw() % 8;
                if (k == 'k') return GenDraw() % 2 ? 0x7fff : 3;
                if (k == 'z') return GenDraw() % 3 ? (uint32_t)(uintptr_t)g_genObj[GenDraw() % 4] : 0;
                return GenValue(k);
            };
            const uint32_t rc = value(sp[0]), rd = value(sp[1]);
            uint32_t a[8] = {};
            for (int k = 0; k < 8 && sp[2 + k]; ++k) a[k] = value(sp[2 + k]);
            logs[s] = t.Run([&] {
                double f = 0;
                const uint32_t r = GenCall(s ? c.ours : (const void*)(uintptr_t)c.exe, rc, rd, a, fp, &f);
                uint64_t fb;
                memcpy(&fb, &f, 8);
                if (!noret) Logf("ret %s fp %08x%08x", fp ? "-" : GenName(r), (uint32_t)(fb >> 32), (uint32_t)fb);
                for (int i = 0; i < NP; ++i) Logf("g %x %s", k@TAG@Ptr[i], GenName(*(uint32_t*)k@TAG@Ptr[i]));
                for (int i = 0; i < NI; ++i) Logf("g %x %08x", k@TAG@Int[i], *(uint32_t*)k@TAG@Int[i]);
                for (int o = 0; o < 4; ++o) Logf("obj %d %08x", o, Fnv(g_genObj[o], 0x400));
                for (int b = 0; b < 24; ++b) Logf("buf %d %08x", b, Fnv(g_genBuf[b], 0x2000));
                for (int b = 0; b < 3; ++b) Logf("list %d %08x", b, Fnv(g_genList[b], 0x100));
                Logf("big %08x", Fnv(g_genBig + 0x114000, 0x2000));
            });
        }
        char what[64];
        sprintf(what, "small callback %x", c.exe);
        CompareLogs(logs[0], logs[1], what, it);
    }
    for (int i = 0; i < NP; ++i) *(uint32_t*)k@TAG@Ptr[i] = savedP[i];
    for (int i = 0; i < NI; ++i) *(uint32_t*)k@TAG@Int[i] = savedI[i];
@AFTER@
}
