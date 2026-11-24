// Network.cpp - RT3.exe 0x468000-0x474000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "game/calendar.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/containers.h"
#include "core/ptrlist.h"

#undef near
#undef far
#undef small

struct HMENU__;

typedef HMENU__* HMENU;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all);
void* __fastcall ReturnThis473cc0(void* o);
void __fastcall Nop473cd0(void*);
void __fastcall News_Init(uint8_t* e, int, int32_t kind, const char* title, const char* text, int32_t a, int32_t b);
int32_t __fastcall RegKey_SetString(void* o, int, const char* subKey, const char* name, const char* data);
void* __fastcall Pool_Alloc(int32_t* p);
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
void* __stdcall NewListHeader(int32_t);
void __fastcall Load_Unlink(uint32_t* o);
double __fastcall Train_Float159(uint8_t* t, int, int32_t i);
int32_t __fastcall Obj_AddMessage(uint8_t* o, int, int32_t a, int32_t b, int32_t c, const char* text);
int32_t __fastcall Load_Age(uint8_t* l);
int32_t __fastcall CountMp3Files(const char* dir);
void __fastcall Music_BuildList(uint8_t* o);
int32_t Net_LockQueue();
void Net_UnlockQueue();
const char* __fastcall GetString(int32_t id);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
float __fastcall Game_VersionNumber();
float __fastcall Net_CommonVersion();
void __fastcall Playlist_Next(uint8_t* p);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall Sound_PlayMusic(uint8_t* s, int, const char* file, void* onEnd);
char* __fastcall StrCopy(char* dst, const char* src);
void __fastcall Playlist_PlayCurrent(uint8_t* p);
uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind);
void __fastcall Obj_FreeBuffer8(void* o);
void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t);
void __fastcall SetSeed(int32_t seed);
int32_t __fastcall Obj_MapInsertNew(void* o, int, uint16_t key, int32_t value);
void __fastcall Obj_Set8(void* o, int, int32_t v);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __stdcall SetGlobal6d1aa4(int32_t v);
int32_t Global6d40e4();
int32_t __fastcall Trains_AnyActive();
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Collection_Count(Collection* c);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
int32_t __stdcall Net_PlayerIdByName(const char* name);
int32_t __fastcall Engine_DeviceCall68(uint8_t* e, int, int32_t v);
uint8_t* __stdcall Textures_Find(int32_t key);
void __stdcall Net_UnlockQueueArg(int32_t);
char* __stdcall Net_LocalAddress(char* out);
int32_t Global6d4100();
void __fastcall Obj_EditNameList(uint8_t* o, int, const char* name, int32_t mode);
void __fastcall Chat_AddLine(const char* text);
void __fastcall Lobby_AddChatText(const char* text);
void __fastcall Music_Close(uint8_t* m);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
uint32_t __fastcall Sound_Get2c(uint8_t* s);
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
int32_t FirstFreeSlot6ce2e8();
const char* __fastcall Options_SaveFolder(void* opt);
int32_t __fastcall FileOpens(const char* path);
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t ClockMs();
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Track_UpdateColor(uint8_t* o);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t __fastcall Obj_Get25(void* o);
void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void* __fastcall Obj_Building(void* o);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void __fastcall Tracks_ApplyPendingKinds(Collection* c);
void __fastcall Obj_FreeList1c9(void* o);
void __fastcall Obj_SetText1c(uint8_t* o, int, const char* s, uint32_t v);
void __fastcall Train_ResetMotion(uint8_t* t);
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void* __fastcall Obj_Company21(void* o);
int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref2a8(void* o);
void __fastcall SceneObj_SetAngle2(uint8_t* o, int, uint32_t v);
void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
int32_t __fastcall Obj_CityKindIs2(void* o);
void __fastcall SceneObj_ResetAttachmentLists(uint8_t* o);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running);
void* __fastcall Identity(void* o);
uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
uint8_t __fastcall Obj_Flag3d4(void* o);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
void __fastcall Company_Dissolve(uint8_t* c);
void __fastcall Lobby_Tick(uint8_t* l, int, uint32_t now);
void __fastcall Net_HostStart(uint8_t* s);
void __fastcall Net_ProcessIncoming(uint8_t* s, int, uint32_t frame);
void __fastcall Net_DrainQueue(uint8_t* s, int, int32_t);
void __fastcall NetMessage_ShowNext(uint8_t* s);
void __fastcall NetQueue_Drain(uint8_t* s);
void* __fastcall World_Construct(uint8_t* w);
void __fastcall Obj_SumBuildingWeights(void* o);
int32_t GetSeed();
int32_t __fastcall Train_FirstCarType126(uint8_t* t);
void* __fastcall Cursor33_Init(uint8_t* c);
void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
void GameSpy_Pump();
void SetGlobal6cfeac();
uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d);
void __fastcall Obj_DecayCounters(uint8_t* o);

int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner,
                                          int32_t byDegree, int32_t exclude, int32_t skipLinked);

int32_t __fastcall Obj_RectFree(void* o, int, const int32_t* r, int32_t key);
void __fastcall NetQueue_Push(uint8_t* s, int, const uint32_t* m, int32_t tag);
float __stdcall Route_GradeFactor(float x2, float z2, float x1, float h1, float z1);
void __fastcall Obj_CopyCorners(uint8_t* o, int, uint8_t* body);
void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
void* Engine_FirstListEntry();
void __fastcall Net_DropOldMessages(uint8_t* s, int, uint8_t* e, int32_t minutes);
void __fastcall Buildings_ApplyQueuedMoves();
void __fastcall Obj_CountItems(void* o, int, uint8_t* list);
void Game_PauseTools();
uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep);
void __fastcall Net_Dispatch(uint8_t* s, int, uint8_t* msg, uint32_t arg);
int32_t __fastcall Finance_SyncMessage(uint8_t* msg, int32_t mode, void** out);
uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t GetSeed();
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE int32_t __fastcall Company_OwnerFlag1e1(void* c);
RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d);
RT3_NOINLINE int32_t __fastcall Company_TrainCount(void* c);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE void SetGlobal6cfeac();
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Obj_SumBuildingWeights(void* o);
int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void* __fastcall World_Construct(uint8_t* w);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
RT3_NOINLINE void __fastcall Tracks_ApplyPendingKinds(Collection* c);
RT3_NOINLINE void __fastcall Obj_FreeList1c9(void* o);
RT3_NOINLINE void __fastcall Track_UpdateColor(uint8_t* o);
uint8_t __stdcall Obj_Mode1Or4(int32_t);
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out);
RT3_NOINLINE int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
RT3_NOINLINE void __fastcall World_SaveGame(uint8_t* w, int, const char* name, uint32_t flags);
void Net_ApplyTrainPosition();
void Net_ApplyCash();
RT3_NOINLINE uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
void __fastcall BuildingPacket_Invest(uint32_t* pkt);
void __fastcall BuildingPacket_Release(uint32_t* pkt);
void __fastcall BuildingPacket_Take(uint32_t* pkt);
void __fastcall Chairman_Resign(int32_t* r);
RT3_NOINLINE void __fastcall CompanyCmd_IssueBond(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_RepayBond(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_Bankrupt(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_IssueShares(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_BuyBack(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_SetDividend(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_BuyTerritory(uint32_t* cmd);
void __fastcall Lobby_SetMode(int32_t kind, int, int32_t, int32_t);
void __fastcall Packet_BuyStock(uint32_t* pkt);
void __fastcall Packet_SellStock(uint32_t* pkt);
void __fastcall NetMsg_ReplaceEngines(uint8_t* m);
void __fastcall NetMsg_SellTrain(uint8_t* m);
RT3_NOINLINE int32_t __fastcall TrainCmd_Apply(uint32_t* cmd);

// Network handlers passing their packet on. Return 1.
#define RT3_NETFWD(addr, name, type, handler) \
    int32_t __fastcall name(uint8_t* m) { return NetMsg_Forward<type>(m, [](type p) { handler(p); }); } \
    RT3_REPLACE(addr, name);

void __fastcall Obj_SetEntry10e(uint8_t* o, int, const uint32_t* src, int32_t i);
void __fastcall Load_Deliver(uint32_t* load);
void Peer_LeaveStaging();
int32_t MapBitmapChecksum();
void __fastcall NameDialog_SetNameIfOpen(const char* name);

// Collection callbacks deleting record `id`: its teardown, then the slot.
#define RT3_DELETE_RECORD(addr, name, teardown, type) \
    void __fastcall name(Collection* c, int, uint32_t id) { \
        teardown((type)Collection_Get(c, 0, id)); \
        Collection_Release(c, 0, id); \
    } \
    RT3_REPLACE(addr, name);

float __fastcall Map_TotalCargoValue(uint8_t* m, int, int32_t);
void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);

// Network handlers acting on train d[1] with d[2]. Return 1.
#define RT3_TRAINMSG(addr, name, call) \
    int32_t __fastcall name(uint8_t* m) { \
        if (F<int32_t>(m, 0xc) > 0) { \
            const int32_t* d = F<const int32_t*>(m, 8); \
            if (Collection_IsValid(g_trains, 0, (uint32_t)d[1])) { \
                uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, (uint32_t)d[1]); \
                call; \
            } \
        } \
        return 1; \
    } \
    RT3_REPLACE(addr, name);

void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price);
void Profiles_IndexOf();

// Network handlers forwarding a train message (with the session's minute
// following the next queued part) for an existing train d[1]. Return 1.
#define RT3_TRAINFWD(addr, name, type) \
    int32_t __fastcall name(uint8_t* m) { \
        const uint32_t* d = F<const uint32_t*>(m, 8); \
        if (!Collection_IsValid(g_trains, 0, d[1])) return 1; \
        uint8_t* part = Net_CountMessagePart(g_session, 0, 1); \
        uint8_t* s = g_session; \
        F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0; \
        Net_Send(g_session, 0, nullptr, type, F<int32_t>(m, 0xc), (const uint8_t*)d, 0x11, 0); \
        return 1; \
    } \
    RT3_REPLACE(addr, name);

// 0x4719c0 / 0x471d50 (dialog callbacks). Marks this peer (bit +0x10, and
// the plain bit for the answer 0x3f2 "yes") in the request's mask (+8 / +6)
// and sends it to the host (0x49 / 0x4d), then frees the request.
#define RT3_PEER_ANSWER(addr, name, off, type, size) \
    void __fastcall name(uint8_t* dlg, int32_t answer) { \
        uint8_t* req = F<uint8_t*>(dlg, 0x1c); \
        const char* me = (const char*)Engine_FirstListEntry() + 8; \
        int32_t k; \
        __asm { \
            __asm push ebx \
            __asm mov ebx, me \
            __asm call Profiles_IndexOf \
            __asm pop ebx \
            __asm mov k, eax \
        } \
        F<uint32_t>(req, off) |= 1u << (k + 0x10); \
        if (answer == 0x3f2) F<uint32_t>(req, off) |= 1u << k; \
        uint8_t* s = g_session; \
        Net_Send(s, 0, (const char*)(s + 0x8f48), type, size, req, 1, 0); \
        Callback_Free(req); \
    } \
    RT3_REPLACE(addr, name);

int32_t __fastcall Company_TrainCount(void* c);
int32_t Chat_HandleCommand();
int32_t BuildingMsg_Apply();
void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);
void Companies_ForEntry();
void Companies_Clash();
void __fastcall Msg_FoundCompany(int32_t* m);
void __fastcall Obj_CopyTo6d1a08(const uint32_t* o);
void ProxyFight_Resolve();
int32_t __fastcall Models_DisplayName(const char* name, char* out);
int32_t __fastcall Scenario_SelectByName(const char* name);
void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
int32_t __fastcall Lobby_CountFreeEntries(uint8_t* l);
void __fastcall Obj_CopyTo6d10d8(const uint8_t* o);
void Merger_ResolveVote();
int32_t __fastcall Obj_NameContains(uint8_t* o, int, const char* s);
int32_t __fastcall DetailLevelFor(int32_t, int32_t v);
int32_t __fastcall NewGame_ClaimSlot(int, int32_t mode);
void __fastcall Obj_StampDate(uint8_t* o, int, int32_t v);
int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);

// 0x470ed0 / 0x470fa0 (network cheats). Every active player gets +5,000,000
// / -100,000 cash (+0x154) and every player-owned company the same stat 0xd.
// Return 1.
#define RT3_CASH_CHEAT(addr, name, cash, stat) \
    int32_t name() { \
        if (!g_world) return 1; \
        if (Collection* c = g_players) \
            for (int32_t i = 0; i < Collection_Count(c); ++i) { \
                uint8_t* e = (uint8_t*)Collection_Get(g_players, 0, Collection_IdAt(g_players, 0, (uint32_t)i, 0)); \
                if (F<int32_t>(e, 0x1e1)) Obj_AddDouble154(e, 0, cash); \
                c = g_players; \
            } \
        if (Collection* c = g_companies) \
            for (int32_t i = 0; i < Collection_Count(c); ++i) { \
                uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, Collection_IdAt(g_companies, 0, (uint32_t)i, 0)); \
                if (Company_OwnerFlag1e1(co)) Company_AddStat(co, 0, 0xd, BitsF(stat)); \
                c = g_companies; \
            } \
        return 1; \
    } \
    RT3_REPLACE(addr, name);

uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);
uint32_t __fastcall Obj_Map9050Query(void* o, int, uint32_t key, int32_t mode);

// 0x471b90 / 0x471ed0 (network handlers). Collects a peer's answer bits
// (+8 / +6) into the pending request with the same checksum (0x468cb0);
// once every active player answered it goes to everyone (0x47 / 0x4b) and
// is dropped. Return 1.
#define RT3_PEER_COLLECT(addr, name, off, size, type) \
    int32_t __fastcall name(uint8_t* m) { \
        uint8_t* req = F<uint8_t*>(m, 8); \
        const uint32_t bits = F<uint32_t>(req, off); \
        F<uint32_t>(req, off) = 0; \
        const uint32_t crc = Crc32(req, size); \
        uint8_t* e = (uint8_t*)(uintptr_t)Obj_Map9050Query(g_session, 0, crc, 0); \
        F<uint32_t>(e, off) |= bits; \
        for (int32_t i = 0; i < Collection_Count(g_players); ++i) { \
            if (F<uint32_t>(e, off) & (1u << (i + 0x10))) continue; \
            Collection* c = g_players; \
            uint8_t* p = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)); \
            if (F<int32_t>(p, 0x1e1) && F<int32_t>(p, 4)) return 1; \
        } \
        Session_StampNextPart(); \
        Net_Send(g_session, 0, nullptr, type, size, e, 0x11, 0); \
        Obj_Map9050Query(g_session, 0, crc, 1); \
        Free447(e); \
        return 1; \
    } \
    RT3_REPLACE(addr, name);

char* __stdcall Net_PlayerNameById(int32_t id);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall Company_CountIdleBuildings(void* c);
int32_t __fastcall DateToMinutes(const GameDate* d);
void __fastcall MpDialog_Refresh(int32_t reselect);
void Slots_Find();
void __fastcall Menu_Attach(HMENU* m);
void __fastcall Points_Bounds(void*, int, const uint32_t* p, int32_t* out);
int32_t TrackMsg_Apply();
int32_t __fastcall Obj_Sum987c(void* o);
float __fastcall Obj_MidAboveGround(uint8_t* o);

int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);

int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
void Player_OnLeft();
int32_t __fastcall Obj_ByteF9(uint8_t* o);
float __fastcall Car_TiltValue(uint8_t* c);
void* __fastcall Train_NthCarKind1(uint8_t* t, int, int32_t n);
uint8_t* __fastcall Net_FindFlaggedPlayer();
void __fastcall Obj_SetName8f48(uint8_t* o, int, const char* s);
void NewGame_CapAiSlots();
void __fastcall Obj_SetDateMinutes(uint8_t* o, int, const GameDate* d);
void CancelMouseAction();
int32_t __fastcall Train_CanRun(uint8_t* t);
int32_t __fastcall Train_CanRunNonElectric(uint8_t* t);
uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __stdcall Table9_Append(uint8_t* o, const uint8_t* src);
typedef void(WINAPI* CsFn)(LPCRITICAL_SECTION);
struct HashMap;
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Return1_521790(void*, int, int32_t, int32_t);
void __fastcall Net_ResetOutgoing();
void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size);
struct GameDate;
void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);
void __stdcall Net_PurgeSentMessages(int32_t);
int32_t Net_LockList2();
void __fastcall Net_BroadcastGame(uint8_t* o, int, const char* name);
void Net_UnlockList2();
void __stdcall Textures_RefreshQueued(int32_t);
void CloseFile6ce994();
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Nop5217b0(void*);
int32_t __fastcall Net_ShutdownShared(uint8_t* net);
void __fastcall Music_Shutdown(uint8_t* m);
void __fastcall Hash_DestroyThunk(HashMap* m);
void Net_DeleteLocks();
void __fastcall Music_Destroy(uint8_t* m);
int32_t __fastcall Net_Host(uint8_t* o, int, const char* game, const char* name, const char* password, int32_t ms, int32_t maxPlayers);
int32_t __fastcall Net_Connect(uint8_t* o, int, const char* host, const char* name, const char* password, int32_t ms);
int32_t __fastcall Music_Start(uint8_t* m);
uint8_t* __fastcall Net_Construct(uint8_t* o);
int32_t __fastcall Net_InitShared(uint8_t* net);
void __stdcall SetGlobal6d4108(int32_t v);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void MpSession_Reset();
void RegisterSyncChecks();
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
void __fastcall Nop552ff0();

int32_t __fastcall Net_SendTo(uint8_t* net, int, const uint8_t* data, uint16_t size, uint8_t type, uint8_t channel, const char* to,
                              uint32_t flags, uint32_t timeout, uint32_t serial, int32_t);

// Net_SendTo with every argument as the full dword the original pushes.
typedef int32_t(__fastcall* SendToFn)(void*, int, const void*, uint32_t, uint32_t, uint32_t, const char*, uint32_t, uint32_t,
                                      int32_t, int32_t);

uint8_t __stdcall Net_MsgKind(const uint8_t* msg);
int32_t __fastcall Msg_Queue(void*, int, int32_t key, uint32_t* msg, uint8_t* ctx);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);
const char* Net_WaitingPlayersText();
uint32_t __fastcall Obj_Map9050Insert(void* o, int, uint32_t key, int32_t value);
void __fastcall CmdQueue_Clear(uint8_t* q);
void File_GetSize();
int32_t __fastcall Net_SendQueueTotals(uint8_t* o, int, int32_t* msgs, int32_t* bytes);
uint8_t* __fastcall Obj_BuildStatusPacket(uint8_t* o, int, int32_t* outSize);
uint8_t* __stdcall Table9_Find(uint8_t* o, const uint8_t* e);
uint32_t __fastcall Obj_FindNode8f28(void* o, int, int32_t key);
void __stdcall Table9_Prune(uint8_t* o, uint32_t limit);
int32_t __fastcall Net_HasPlayer(void*, int, const char* name);
uint32_t* __stdcall Net_TakeChannelMessages(uint8_t id);
void __fastcall Net_FinishReceived(uint8_t* net, int, uint32_t* list);
int32_t __stdcall Profiles_IndexOfEntry(int32_t key);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
RT3_NOINLINE int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
RT3_NOINLINE uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg, int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);
RT3_NOINLINE void __fastcall Company_Dissolve(uint8_t* c);
RT3_NOINLINE void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot);
RT3_NOINLINE void Game_PauseTools();
RT3_NOINLINE void Options_Restore();
RT3_NOINLINE void __fastcall World_SaveGame(uint8_t* w, int, const char* name, uint32_t flags);
RT3_NOINLINE int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);
RT3_NOINLINE void __fastcall Load_Deliver(uint32_t* load);
RT3_NOINLINE Collection* __fastcall Collection5ce130_Construct(Collection* c);
void __fastcall Collection5ce130_Destruct(Collection* c);
void __fastcall Loads_Age(Collection* t);
RT3_NOINLINE void __fastcall Table_ReleaseKeyAny(Collection* t, int, int32_t key);
RT3_NOINLINE void __fastcall Table_ReleaseKey8(Collection* t, int, int32_t key);
RT3_NOINLINE void __fastcall Lots_UpdateLinked(Collection* c);
RT3_NOINLINE void __fastcall Lots_RebuildDemand(Collection* c);
void __fastcall Obj_Free108(void* o);
int32_t __fastcall CountMp3Files(const char* dir);
void __fastcall Music_BuildList(uint8_t* o);
void __fastcall Playlist_Next(uint8_t* p);
void ResetGlobals4686d0();
void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root);
uint32_t __fastcall HandleObj_Destruct(void* o);
uint8_t* __fastcall Music_Construct(uint8_t* o);
void __fastcall Playlist_PlayCurrent(uint8_t* p);
void __fastcall Playlist_Tick(uint8_t* p);
void* __fastcall HandleObj_Delete(void* o, int, uint32_t flags);
int32_t __fastcall RegKey_SetString(void* o, int, const char* subKey, const char* name, const char* data);
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def);
int32_t __fastcall Key_KeepSmaller(const uint32_t* a, const uint32_t* b);
int32_t __fastcall Obj_FreeBuffer4IfSet(uint8_t* o);
int32_t Toggle6cd910();
int32_t __fastcall Obj_MapInsertNew(void* o, int, uint16_t key, int32_t value);
void __fastcall Obj_Set8(void* o, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Obj_Sum987c(void* o);
void __stdcall Table9_Prune(uint8_t* o, uint32_t limit);
RT3_NOINLINE void __stdcall Table9_Append(uint8_t* o, const uint8_t* src);
uint8_t* __stdcall Table9_Find(uint8_t* o, const uint8_t* e);
uint32_t __fastcall Obj_Map9050Insert(void* o, int, uint32_t key, int32_t value);
RT3_NOINLINE uint32_t __fastcall Obj_Map9050Query(void* o, int, uint32_t key, int32_t mode);
RT3_NOINLINE int32_t __fastcall Obj_NameContains(uint8_t* o, int, const char* s);
void __fastcall Obj_EditNameList(uint8_t* o, int, const char* name, int32_t mode);
void __fastcall Lobby_Mode7Cb(void*, int32_t on, int32_t a, int32_t);
void __fastcall Set6cd974_469190(void*, int32_t v, int32_t);
void __fastcall Lobby_NameCb(void*, int32_t edit, const char* a, const char* b, int32_t);
void Slots_Find();
void __fastcall ServerList_Cb(int32_t, int32_t list, const char* name, void* server, int32_t, int32_t msg, int32_t, uint8_t* slots);
RT3_NOINLINE void __fastcall Lobby_AddServerCb(void*, int32_t ok, int32_t error, int32_t index, const char* name, uint32_t flags, int32_t);
void __fastcall Lobby_StartQueryCb(uint8_t* s, int32_t on, int32_t, int32_t, int32_t);
void Peer_Shutdown();
RT3_NOINLINE void Peer_LeaveStaging();
RT3_NOINLINE void GameSpy_Pump();
RT3_NOINLINE int32_t __fastcall Lobby_UpdateList(uint8_t* lobby);
RT3_NOINLINE int32_t __fastcall Lobby_ListGames(uint8_t* lobby);
const char* __fastcall Obj_NextBufferedLine(void* o);
RT3_NOINLINE int32_t __stdcall Peer_JoinNamed(const char* name);
RT3_NOINLINE int32_t __stdcall Peer_SendRoom(const char* msg);
RT3_NOINLINE int32_t __stdcall Peer_Think(int32_t);
RT3_NOINLINE void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
RT3_NOINLINE void* __fastcall FreeList_Take(uint8_t* q);
RT3_NOINLINE void __fastcall Obj_SetName8f48(uint8_t* o, int, const char* s);
void __fastcall Lobby_DrawQueue(uint8_t* lobby);
RT3_NOINLINE void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size);
RT3_NOINLINE void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
int32_t __fastcall Obj_AddMessage(uint8_t* o, int, int32_t a, int32_t b, int32_t c, const char* text);
uint32_t __fastcall Obj_FindNode8f28(void* o, int, int32_t key);
RT3_NOINLINE void __fastcall Obj_CountItems(void* o, int, uint8_t* list);
RT3_NOINLINE uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE void __fastcall NetMessage_ShowNext(uint8_t* s);
RT3_NOINLINE int32_t Net_AllPlayersFlag4();
uint8_t* __fastcall Obj_BuildStatusPacket(uint8_t* o, int, int32_t* outSize);
RT3_NOINLINE void __stdcall Textures_RefreshQueued(int32_t);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall Lobby_HostGame(uint8_t* lobby, int, const char* game, const char* name, const char* password, int32_t ms);
RT3_NOINLINE void __fastcall Lobby_JoinGame(uint8_t* lobby, int, const char* host, const char* name, const char* password);
RT3_NOINLINE uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby);
const char* Net_WaitingPlayersText();
RT3_NOINLINE uint8_t* __fastcall NetPacket_Copy(uint8_t* s, int, const uint8_t* m);
RT3_NOINLINE void __fastcall Obj_StampDate(uint8_t* o, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Net_MessageComplete(uint8_t* s);
RT3_NOINLINE int32_t __fastcall Net_HasPlayer(void*, int, const char* name);
RT3_NOINLINE float __fastcall Net_CommonVersion();
RT3_NOINLINE void __fastcall NetQueue_Drain(uint8_t* s);
RT3_NOINLINE void __fastcall NetQueue_Push(uint8_t* s, int, const uint32_t* m, int32_t tag);
RT3_NOINLINE int32_t __fastcall Obj_RectFree(void* o, int, const int32_t* r, int32_t key);
void __fastcall Lobby_RetryNameCb(uint8_t* s, int32_t flag, int32_t a, int32_t);
RT3_NOINLINE int32_t __fastcall Lobby_Connect(uint8_t* lobby, int, const char* nick);
RT3_NOINLINE void* __stdcall NewListHeader(int32_t);
int32_t __fastcall Msg_Queue(void*, int, int32_t key, uint32_t* msg, uint8_t* ctx);
RT3_NOINLINE void __fastcall Net_Dispatch(uint8_t* s, int, uint8_t* msg, uint32_t arg);
RT3_NOINLINE void __fastcall Net_ProcessIncoming(uint8_t* s, int, uint32_t frame);
RT3_NOINLINE void __fastcall Net_DrainQueue(uint8_t* s, int, int32_t);
RT3_NOINLINE void __fastcall Lobby_Tick(uint8_t* l, int, uint32_t now);
RT3_NOINLINE void __fastcall Net_HostStart(uint8_t* s);
void __fastcall Net_WeeklyCheck(uint8_t* s);
RT3_NOINLINE uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s);
RT3_NOINLINE void __fastcall Net_DropOldMessages(uint8_t* s, int, uint8_t* e, int32_t minutes);
RT3_NOINLINE void __fastcall Net_RunFrameMessages(uint8_t* s);
RT3_NOINLINE void __fastcall Session_Close(uint8_t* s);
RT3_NOINLINE int32_t __fastcall Session_Open(uint8_t* s, int, int32_t gamespy, const char* nick);
RT3_NOINLINE void __fastcall Session_Shutdown(uint8_t* s);
RT3_NOINLINE int32_t __fastcall DetailLevelFor(int32_t, int32_t v);
int32_t __fastcall FileOpens(const char* path);
void File_GetSize();
int32_t FirstFreeSlot6ce2e8();
void __fastcall Chat_AddLine(const char* text);
int32_t __fastcall NetMsg_SendLobby(void*, uint8_t* from);
int32_t NetMsg_GameStarted();
int32_t __fastcall NetMsg_GameOptions(uint8_t* m);
int32_t __fastcall Return1_46c760(void*);
int32_t __fastcall NetMsg_SendEngineValue(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_TextureTime(uint8_t* m, const int32_t* d);
int32_t __fastcall NetMsg_CameraBookmark(uint8_t* m);
int32_t __fastcall NetMsg_TextureFlag8(void*, const int32_t* d);
int32_t __fastcall NetMsg_TextureFlag1(uint8_t* m, const int32_t* d);
int32_t __fastcall NetMsg_QueueRecords(uint8_t* msg, uint8_t* from);
int32_t __fastcall NetMsg_SendCounter(void*, uint8_t* from);
int32_t __fastcall NetMsg_Seed(uint8_t* m);
int32_t __fastcall Return1_46c9e0(void*);
int32_t __fastcall Net_SendFiled48(const uint8_t* msg);
int32_t __fastcall Net_SendFiled4c(const uint8_t* msg);
int32_t __fastcall Return1_46ccd0(void*);
int32_t NetMsg_PauseForSync();
int32_t __fastcall NetMsg_Store6d1284(uint8_t* m);
int32_t NetMsg_SendProfile();
int32_t __fastcall NetMsg_TextureInfo(uint8_t* m, const int32_t* d);
int32_t __fastcall NetMsg_DownloadReply(uint8_t* m);
int32_t __fastcall NetMsg_TextureFlag4(void*, const int32_t* d);
int32_t __fastcall NetMsg_CopyToWorld(uint8_t* m);
int32_t NetMsg_ClearQueueFlag1();
int32_t __fastcall Return1_46cfa0(void*);
int32_t __fastcall NetMsg_SaveGame(uint8_t* m);
int32_t __fastcall NetMsg_BeginTransfer(uint8_t* m);
int32_t __fastcall NetMsg_TransferChunk(uint8_t* m);
int32_t __fastcall NetMsg_EndTransfer(uint8_t* m);
int32_t Set6ce9c8();
void CloseFile6ce994();
void __fastcall Queue6cd990_Push(uint16_t a, int32_t b, int32_t c, int32_t d, int32_t e);
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE void __fastcall Net_FeedResync(uint8_t* s);
int32_t CountUsedSlots6ce2d8();
double __fastcall Train_Total159(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_CanRun(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_CanRunNonElectric(uint8_t* t);
RT3_NOINLINE void __fastcall Obj_SetDateMinutes(uint8_t* o, int, const GameDate* d);
RT3_NOINLINE void Net_OfferResync();
void Net_ApplyTrainPosition();
RT3_NOINLINE int32_t __fastcall Scenario_SelectByName(const char* name);
void Profiles_IndexOf();
RT3_NOINLINE int32_t __stdcall Profiles_IndexOfEntry(int32_t key);
RT3_NOINLINE int32_t MapBitmapChecksum();
uint8_t* Net_BuildCargoPacket();
uint8_t* Net_BuildClockPacketA();
uint8_t* Net_BuildClockPacket9();
uint8_t* Net_BuildMapPacket();
uint8_t* Net_BuildCompaniesPacket();
uint8_t* Net_BuildPlayersPacket();
uint8_t* Net_BuildStatePacket();
uint8_t* Net_BuildBuildingsPacket();
uint8_t* Net_BuildTrackPacket();
uint8_t* Net_BuildTrainsPacket();
RT3_NOINLINE int32_t __fastcall Finance_SyncMessage(uint8_t* msg, int32_t mode, void** out);
void Player_OnLeft();
void Companies_Clash();
void NetTrain_Create();
RT3_NOINLINE int32_t __fastcall Trains_AnyActive();
int32_t __cdecl BuildingMsg_ApplyImpl(uint8_t* m, int32_t phase);
int32_t BuildingMsg_Apply();
RT3_NOINLINE int32_t __fastcall TrackMsg_ApplyImpl(uint8_t* m, int32_t phase);
int32_t TrackMsg_Apply();
RT3_NOINLINE void CancelMouseAction();
int32_t __fastcall NetMsg_SessionEvent(uint8_t* msg);
void Net_ApplyCash();
void Companies_ForEntry();
int32_t Chat_HandleCommand();
int32_t __fastcall NetMsg_Chat(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_ScenarioInfo(uint8_t* msg);
int32_t NetMsg_Resume();
int32_t __fastcall NetMsg_Forward0c(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_SetSpeed(uint8_t* m);
int32_t __fastcall NetMsg_SendQueued(uint8_t* m);
int32_t __fastcall Net_TouchEntry(uint8_t* o);
int32_t __fastcall NetMsg_Building(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_PlaceBuilding(uint8_t* m);
int32_t __fastcall NetMsg_TrackBuildRequest(uint8_t* p, uint8_t* from);
int32_t __fastcall NetMsg_TrackBuildResult(uint8_t* m);
int32_t __fastcall NetMsg_BuyTrainRequest(uint8_t* msg, uint8_t* from);
int32_t __fastcall NetMsg_BuyTrain(uint8_t* msg);
int32_t __fastcall NetMsg_SyncFinance(uint8_t* m);
int32_t __fastcall NetMsg_NewCompany(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_CompanyAnswer(uint8_t* m);
int32_t __fastcall Net_Relay1e(const uint8_t* msg);
int32_t __fastcall Net_Relay20(const uint8_t* msg);
int32_t __fastcall Net_Relay22(const uint8_t* msg);
int32_t __fastcall Net_Relay24(const uint8_t* msg);
int32_t __fastcall Net_Relay26(const uint8_t* msg);
int32_t __fastcall Net_Relay28(const uint8_t* msg);
int32_t __fastcall Net_Relay2a(const uint8_t* msg);
int32_t __fastcall Net_Relay2c(const uint8_t* msg);
int32_t __fastcall Net_Relay2e(const uint8_t* msg);
int32_t __fastcall Net_Relay30(const uint8_t* msg);
int32_t __fastcall Net_Relay32(const uint8_t* msg);
int32_t __fastcall Net_Relay34(const uint8_t* msg);
int32_t __fastcall Net_Relay36(const uint8_t* msg);
int32_t __fastcall Net_Relay38(const uint8_t* msg);
int32_t __fastcall Net_Relay3a(const uint8_t* msg);
int32_t __fastcall NetMsg_InsertOrder(uint8_t* m);
int32_t __fastcall NetMsg_TakeoverVote(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_MergerVote(uint8_t* m);
int32_t __fastcall NetMsg_MergerRequest(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_ProxyFight(uint8_t* m);
int32_t NetMsg_RequestState();
int32_t NetMsg_Copy9058();
int32_t __fastcall NetMsg_JoinRequest(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_Store9860(uint8_t* m);
int32_t __fastcall NetMsg_SetName(uint8_t* m);
int32_t __fastcall NetMsg_Set1e1(uint8_t* m);
int32_t NetMsg_ReleaseSlots();
int32_t __fastcall NetMsg_CheckScenario(uint8_t* m, uint8_t* from);
int32_t __fastcall Net_Relay5c(const uint8_t* msg);
int32_t __fastcall NetMsg_Electrify(uint8_t* m);
int32_t __fastcall Net_Relay5e(const uint8_t* msg, const char* to);
int32_t __fastcall NetMsg_RenamePlayer(uint8_t* m);
int32_t __fastcall Net_Relay60(const uint8_t* msg);
int32_t __fastcall NetMsg_PlayerColour(uint8_t* m);
int32_t __fastcall NetMsg_CompanyNameRequest(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_RenameEntry(uint8_t* m);
int32_t __fastcall NetMsg_CompanyColourRequest(uint8_t* m, uint8_t* from);
int32_t __fastcall NetMsg_CompanyColour(uint8_t* m);
int32_t __fastcall Net_Relay74(const uint8_t* msg);
int32_t __fastcall NetMsg_DeleteVehicle(uint8_t* m);
int32_t __fastcall Net_Relay72(const uint8_t* msg);
int32_t __fastcall NetMsg_PlaceBuildings(uint8_t* m);
int32_t __fastcall NetMsg_Cash(uint8_t* m);
int32_t __fastcall NetMsg_TrainPosition(uint8_t* m);
int32_t __fastcall NetMsg_PlayerStatus(uint8_t* m);
void Msg_MarkSeen();
int32_t __fastcall Net_Relay76(const uint8_t* msg);
int32_t __fastcall NetMsg_AiBuyTrain(const uint8_t* msg);
int32_t __fastcall Net_Relay78(const uint8_t* msg);
int32_t __fastcall NetMsg_Bulldoze(uint8_t* m);
int32_t __fastcall Net_Relay7a(const uint8_t* msg);
int32_t __fastcall NetMsg_AbsorbCompany(uint8_t* m);
int32_t __fastcall Net_Relay7c(const uint8_t* msg);
int32_t __fastcall NetMsg_DissolveCompany(uint8_t* m);
int32_t __fastcall Net_Relay7e(const uint8_t* msg);
RT3_NOINLINE void MpSession_Reset();
RT3_NOINLINE void RegisterSyncChecks();
void Net_SendPendingRecords();
RT3_NOINLINE void __fastcall Buildings_ApplyQueuedMoves();
void* __fastcall ReturnThis473cc0(void* o);
void __fastcall Nop473cd0(void*);
void __fastcall News_Init(uint8_t* e, int, int32_t kind, const char* title, const char* text, int32_t a, int32_t b);
RT3_NOINLINE Collection* __fastcall Collection5ce418_Construct(Collection* c);
RT3_NOINLINE void __fastcall Collection5ce418_Destruct(Collection* c);
RT3_NOINLINE void* __fastcall Table_Newest14(Collection* t);
RT3_NOINLINE int32_t __fastcall Table_AnyPending(Collection* t);
RT3_NOINLINE void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all);
RT3_NOINLINE void* __fastcall Table_FindField14Prev(Collection* t, int, int32_t v);
RT3_NOINLINE void* __fastcall Table_FindField14Next(Collection* t, int, int32_t v);
RT3_NOINLINE uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall Msg_FoundCompany(int32_t* m);
RT3_NOINLINE void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
RT3_NOINLINE int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner, int32_t byDegree, int32_t exclude, int32_t skipLinked);
RT3_NOINLINE int32_t __fastcall TrackNet_ExtendTo(Collection* net, int, float x, float y, int32_t mode, int32_t company, int32_t kind, float radius, int32_t* out);
RT3_NOINLINE int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
RT3_NOINLINE uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
RT3_NOINLINE void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
RT3_NOINLINE void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
RT3_NOINLINE void __fastcall BuildingPacket_Invest(uint32_t* pkt);
RT3_NOINLINE void __fastcall BuildingPacket_Release(uint32_t* pkt);
RT3_NOINLINE void __fastcall BuildingPacket_Take(uint32_t* pkt);
RT3_NOINLINE void __fastcall Chairman_Resign(int32_t* r);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void Merger_ResolveVote();
RT3_NOINLINE void __fastcall Lobby_AddChatText(const char* text);
RT3_NOINLINE void __fastcall MpDialog_Refresh(int32_t reselect);
void __fastcall Panel_SetDragging(uint8_t* w, int, int32_t on);
RT3_NOINLINE void __fastcall Packet_BuyStock(uint32_t* pkt);
RT3_NOINLINE void __fastcall Packet_SellStock(uint32_t* pkt);
RT3_NOINLINE void ProxyFight_Resolve();
RT3_NOINLINE void __fastcall NetMsg_ReplaceEngines(uint8_t* m);
RT3_NOINLINE void __fastcall NetMsg_SellTrain(uint8_t* m);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE uint8_t* __stdcall Textures_Find(int32_t key);
RT3_NOINLINE int32_t __fastcall Textures_QueryDesc(uint8_t* e, int, int32_t key, int32_t lock);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);

static inline void* ExeMalloc53b(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeFree53b(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline char* ExeStrstr(const char* s, const char* k) { return ((char*(__cdecl*)(const char*, const char*))0x5a1ff0)(s, k); }

// The network player list (head from Net_LockQueue): nodes {next, ?, name +8, flags +0x5c}.
static inline uint8_t* PlayerHead() { return *(uint8_t**)Net_LockQueue(); }

// Version as an integer: (v + 0x5c88a0) x 0x5c85d8 through the exe's _ftol.
static inline int32_t VersionInt(float v) {
    int32_t r;
    __asm {
        fld v
        fadd qword ptr ds:[0x5c88a0]
        fmul qword ptr ds:[0x5c85d8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// Looks up name in the save list of the given kind; on a match stores its display
// name at 0x6ce670.
static int32_t FindInList(const char* name, int32_t kind) {
    void* p = ((void*(__cdecl*)(size_t))0x53b070)(0xc);
    uint8_t* list = p ? SaveList_Init((uint8_t*)p, 0, kind) : nullptr;
    int32_t found = 0;
    for (int32_t i = 1; F<int32_t>(list, 0) > 0 && i <= F<int32_t>(list, 0); ++i) {
        if (Stricmp((const char*)F<uint8_t*>(list, 8) + (i - 1) * 0x25a, name)) continue;
        found = 1;
        g_6ce9bc = kind == 0xc;
        if (kind == 0xc) strcpy((char*)0x6ce630, name);
        strcpy((char*)0x6ce670, (const char*)F<uint8_t*>(list, 8) + (i - 1) * 0x25a + 0x12d);
        break;
    }
    Obj_FreeBuffer8(list);
    ((void(__cdecl*)(void*))0x53b080)(list);
    return found;
}

static inline uint8_t* Cargos() { return hook::As<uint8_t*>(g_cargoTypes); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

// 0x46ecd0 (player record in ESI). A player left: with none given, stops dragging
// the panel 0x6d1720; for the local player (0x6d40e4 = +0x18) it also plays sound
// 0x2f.
static void __cdecl Player_OnLeft_Impl(uint8_t* p) {
    uint8_t* panel = g_6d1720;
    if (!p) {
        if (panel && F<int32_t>(panel, 0x94) == 1) Panel_SetDragging(panel, 0, 0);
        return;
    }
    if (Global6d40e4() != F<int32_t>(p, 0x18)) return;
    panel = g_6d1720;
    if (panel && F<int32_t>(panel, 0x94) == 1) Panel_SetDragging(panel, 0, 0);
    Ui_PlaySound(0x2f, 0, 0x3f000000);
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static int32_t Exe_FileSize(const char* path) {  // 0x46c2f0 takes the path in EAX
    int32_t r;
    __asm {
        mov eax, path
        mov ecx, 0x46c2f0
        call ecx
        mov r, eax
    }
    return r;
}

static inline uint8_t* NetMgr() { return g_6d40d0; }

static inline uint8_t* GameSpyObj() { return g_session; }

// Plays one of the chat emote voices (table `voices`, pick from the game's
// random generator) when there is a 3D sound system, replacing the last one
// (0x6cd8e0), then posts message `msg` with the sender's name.
static void Chat_Emote(uint8_t* sender, uint32_t voices, int32_t choices, uint32_t msg) {
    if (uint8_t* old = g_6cd8e0) {
        Music_Close(old);
        ((void(__cdecl*)(void*))0x53b080)(old);
        g_6cd8e0 = nullptr;
    }
    if (g_sound && Sound_Is3D(g_sound)) {
        uint8_t* m = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x108);
        if (!m) {
            g_6cd8e0 = nullptr;
        } else {
            F<int32_t>(m, 0) = 0;
            F<uint32_t>(m, 0x104) = 0x3f800000;
            uint32_t seed = g_lcg * 0x19660d + 0x3c6ef35f;
            g_lcg = seed;
            uint32_t pick = choices == 2 ? (seed & 1) : (seed & 0x7fff) % 3;
            g_6cd8e0 = m;
            float vol = (float)(F<float>(g_engine, 0x114702) * D(0x5c8640));
            Music_SetVolume(m, 0, vol);
            void* snd = (void*)(uintptr_t)Sound_Get2c(g_sound);
            Music_Play(g_6cd8e0, 0, hook::Global<const char*>(voices + pick * 4), snd, nullptr);
        }
    }
    FormatText((char*)0x6ce700, GetString((int32_t)msg), (const char*)sender + 8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Chat_AddLine((const char*)0x6ce700);
}

// 0x46f960 body. Handles a chat line starting with '\' or '/' (message
// +8; normalised to '\') from player `sender` (+4 id, +0x50 host rights,
// +8 name): the host's \kick, \ban and \unban (by name or IP), \whois
// (address, CPU, version), \clear, \me, the \snore \sneeze \hurry emotes,
// \trackisfree, \loadgame (0x6ce630) and \sendgame (queues a save to send in
// the transfer table 0x6ce290). Returns 0 for commands, 1 for plain chat.
static int32_t __stdcall Chat_HandleCommandImpl(uint8_t* msg, uint8_t* sender) {
    char* text = F<char*>(msg, 8);
    if (*text != '\\' && *text != '/') return 1;
    auto has = [&](uint32_t s) { return ((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)(text, (const char*)s) != nullptr; };
    auto hasDot = [&]() { return ((const char*(__cdecl*)(const char*, int))0x5a2a00)(text, '.') != nullptr; };
    int32_t self = F<int32_t>(sender, 4);
    *text = '\\';
    if (has(0x5ce3a4)) {
        if (Global6d40e4() != self || !F<int32_t>(sender, 0x50)) return 0;
        int32_t id = Net_PlayerIdByName(text + 6);
        if (id && id != F<int32_t>(sender, 4)) Engine_DeviceCall68(NetMgr(), 0, id);
        return 0;
    }
    if (has(0x5ce39c)) {
        Lobby_AddChatText(nullptr);
        return 0;
    }
    if (has(0x5ce394)) {
        if (F<int32_t>(sender, 4) != Global6d40e4()) return 0;
        int32_t id = Net_PlayerIdByName(text + 7);
        if (!id) return 0;
        uint8_t* p = Textures_Find(id);
        char* ip = (char*)p + 0x258c;
        if (!*ip && F<int32_t>(p, 0x50)) strcpy(ip, Net_LocalAddress((char*)0x6ce700));
        auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
        const char* name = (const char*)p + 8;
        double ver = F<float>(p, 0x268c);
        if (!g_6cd8f0)
            sprintf_((char*)0x6ce700, (const char*)0x5ce368, name, ip, name, F<int32_t>(p, 0x2694), name, ver);
        else
            sprintf_((char*)0x6ce700, (const char*)0x5ce320, name, ip, name, F<int32_t>(p, 0x2694), name, ver, (const char*)0x5ce35c);
        Net_UnlockQueueArg(id);
        Chat_AddLine((const char*)0x6ce700);
        return 0;
    }
    if (has(0x5ce318)) {
        if (Global6d40e4() != F<int32_t>(sender, 4) || !F<int32_t>(sender, 0x50)) return 0;
        const char* name = text + 5;
        int32_t id = 0;
        if (hasDot()) {
            for (uint8_t* p = *(uint8_t**)(uintptr_t)Net_LockQueue(); p; p = *(uint8_t**)p)
                if (!Stricmp((const char*)p + 0x258c, name)) {
                    id = F<int32_t>(p, 4);
                    break;
                }
            Net_UnlockQueue();
            if (id != F<int32_t>(sender, 4)) Obj_EditNameList(GameSpyObj(), 0, name, 1);
        } else {
            id = Net_PlayerIdByName(name);
            if (!id) return 0;
            if (id != F<int32_t>(sender, 4)) {
                Obj_EditNameList(GameSpyObj(), 0, (const char*)Textures_Find(id) + 0x258c, 1);
                Net_UnlockQueueArg(id);
            }
        }
        if (id && id != F<int32_t>(sender, 4)) Engine_DeviceCall68(NetMgr(), 0, id);
        return 0;
    }
    if (has(0x5ce310)) {
        if (Global6d40e4() != F<int32_t>(sender, 4) || !F<int32_t>(sender, 0x50)) return 0;
        const char* name = text + 7;
        if (hasDot()) {
            Obj_EditNameList(GameSpyObj(), 0, name, 0);
            return 0;
        }
        int32_t id = Net_PlayerIdByName(name);
        if (!id || id == F<int32_t>(sender, 4)) return 0;
        Obj_EditNameList(GameSpyObj(), 0, (const char*)Textures_Find(id) + 0x258c, 0);
        Net_UnlockQueueArg(id);
        return 0;
    }
    if (has(0x5ce308)) {
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)0x6ce700, (const char*)0x5ce184, (const char*)sender + 8, text + 4);
        Chat_AddLine((const char*)0x6ce700);
        return 0;
    }
    if (has(0x5ce300)) {
        Chat_Emote(sender, 0x5f2b50, 2, 0xb79);
        return 0;
    }
    if (has(0x5ce2f8)) {
        Chat_Emote(sender, 0x5f2b58, 2, 0xb7a);
        return 0;
    }
    if (has(0x5ce2f0)) {
        Chat_Emote(sender, 0x5f2b60, 3, 0xb7b);
        return 0;
    }
    if (has(0x5ce2e0)) {
        if (Global6d40e4() == F<int32_t>(sender, 4) && F<int32_t>(sender, 0x50)) g_6ce9b4 = 1;
        return 0;
    }
    if (has(0x5ce2d4)) {
        if (!F<int32_t>(sender, 0x50)) return 0;
        strcpy((char*)0x6ce630, text + 10);
        g_6ce9bc = 1;
        return 0;
    }
    if (!has(0x5ce2c8) || !F<int32_t>(sender, 0x50) || Global6d4100() <= 1) return 0;
    const char* name = text + 10;
    int32_t slot = FirstFreeSlot6ce2e8();
    if (slot == 10) return 0;
    uint32_t* ent = (uint32_t*)(0x6ce290 + slot * 0x5c);
    memset(ent, 0, 0x5c);
    F<int32_t>(g_app, 0x6c) = 0;
    char path[0x12c];
    FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(hook::As<void*>(g_app)), name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (!FileOpens(path)) return 0;
    ent[0x4c / 4] = 1;
    ent[0x50 / 4] = (uint32_t)Exe_FileSize(path);
    ent[0x48 / 4] = Crc32String(name, 1);
    ent[0] = (uint32_t)Global6d40e4();
    ent[1] = 0;
    ent[0x58 / 4] = (uint32_t)(uintptr_t)((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c9678);
    strcpy((char*)(ent + 2), name);
    Net_Send(GameSpyObj(), 0, nullptr, 0x7f, 0x5c, (const uint8_t*)ent, 3, 0);
    return 0;
}

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

namespace {
// EAX as the exe's Obj_Mode1Or4 (0x4955b0) leaves it for a caller that
// pushed `kind` just before: AL the style (1 or 4), AH the FPU status of its
// compare of +0x1e1 with -1, the upper half `kind`'s.
int32_t ExtendStyle(uint8_t* n, int32_t kind) {
    if (kind == 1) return (int32_t)(((uint32_t)kind & 0xffffff00u) | 1);
    const float v = F<float>(n, 0x1e1);
    const uint32_t ah = v != v ? 0x45 : (double)v == D(0x5c9bf0) ? 0x40 : (double)v < D(0x5c9bf0) ? 0x01 : 0;
    const uint32_t al = (double)v != D(0x5c9bf0) && n[0x1fd] == 3 ? 1 : 4;
    return (int32_t)(((uint32_t)kind & 0xffff0000u) | ah << 8 | al);
}
}

namespace {
void Callback_Delete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int32_t Tooltip_Clear() { return Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1); }
// A network message's handler gets the packet (+8) when it has one (+0xc).
template <typename P, typename Fn> int32_t NetMsg_Forward(uint8_t* m, Fn handler) {
    if (F<int32_t>(m, 0xc) > 0) handler(F<P>(m, 8));
    return 1;
}
}

namespace {
void Callback_Free(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
uint32_t Message_Dialog(const char* text, uint32_t px) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

namespace {
void Free445(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int Stricmp445(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
int32_t Companies_ClashCall(const void* d) {
    int32_t r;
    __asm {
        push ebx
        mov ebx, d
        call Companies_Clash
        pop ebx
        mov r, eax
    }
    return r;
}
}

namespace {
void Free446(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
// Counts the records of c whose building state is 1, that pass slot 0x70
// and whose city's building class (slot 0x80, +0x8c) is `cls`.
int32_t Count_RecordsOfClass(Collection* c, uint8_t cls) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        if (Obj_Flag3d4(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0))) != 1) continue;
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)r)[0x70 / 4])(r)) continue;
        uint8_t* b = (uint8_t*)Obj_Building(Obj_Ref2a8(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0))));
        if (((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b)[0x8c] == cls) ++n;
    }
    return n;
}
// Rebuilds a vote state from packet d: two 16-bit values (+4 / +5 bytes),
// optionally a third (+6), then per player slot a "present" mark and a
// "for" mark (inactive or voting in `mask`).
void Vote_BuildState(uint8_t* st, const uint8_t* d, uint32_t mask, uint32_t present, uint32_t voted) {
    F<uint16_t>(st, 4) = d[4];
    F<uint16_t>(st, 6) = d[5];
    for (int32_t i = 0; i < Collection_Count(g_players); ++i) {
        st[present + i] = 1;
        Collection* c = g_players;
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!F<int32_t>(e, 0x1e1) || !F<int32_t>(e, 4) || (mask & (1u << i))) st[voted + i] = 1;
    }
}
}

namespace {
void Free447(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int Stricmp447(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
uint32_t Message447(const char* text, int32_t type, uint32_t buttonText, uint32_t a8) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, buttonText, a8, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// Logs the display name of key name string `key` (message `msg`).
void Log_KeyName(int32_t key, int32_t msg) {
    char name[0x190], shown[0x190], text[0x190];
    FormatText(name, GetString(key), 0, 0, 0, 0, 0, 0, 0, 0);
    if (!Models_DisplayName(name, shown)) return;
    FormatText(text, GetString(msg), shown, 0, 0, 0, 0, 0, 0, 0);
    Log_Add(g_messageLog, 0, text, 4, 0, 0);
}
// The session's minute follows the next queued part (0 without one).
void Session_StampNextPart() {
    uint8_t* part = Net_CountMessagePart(g_session, 0, 1);
    uint8_t* s = g_session;
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
}
}

namespace {
// Slots_Find (0x4691d0) takes the slot array in EAX and the server in EDX.
uint8_t* ServerList_FindSlot(uint8_t* slots, void* server) {
    uint8_t* r;
    __asm {
        mov eax, slots
        mov edx, server
        call Slots_Find
        mov r, eax
    }
    return r;
}
}

namespace {
uint32_t Message451(const char* text, uint32_t px, uint32_t py) {
    return Dialog_RunMessage(text, 1, px, py, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
int32_t MyProfileIndex451() {
    const char* me = (const char*)Engine_FirstListEntry() + 8;
    int32_t k;
    __asm {
        push ebx
        mov ebx, me
        call Profiles_IndexOf
        pop ebx
        mov k, eax
    }
    return k;
}
}

namespace {
// Player_OnLeft (0x46ecd0) takes the message data in ESI.
void PlayerOnLeft452(const void* data) {
    __asm {
        push esi
        mov esi, data
        call Player_OnLeft
        pop esi
    }
}
uint8_t* CompanyForEntry452(uint32_t pos, uint32_t key) {
    uint8_t* r;
    __asm {
        push key
        mov eax, pos
        call Companies_ForEntry
        mov r, eax
    }
    return r;
}
}

namespace {
// Chat log line 0x6ce700: into the game log 0x6acd34 (in a world with +0x4a73,
// or with 0x62be80), else as a network notice.
void LogLine460() {
    uint8_t* w = g_world;
    if ((w && F<int32_t>(w, 0x4a73)) || g_62be80)
        Log_Add(g_messageLog, 0, (const char*)0x6ce700, 4, 0, 0);
    else
        Net_QueueText(g_session, 0, (const char*)0x6ce700, 1, 0, 0, 0);
}
// The host's session change notice (0x58), stamped with the message part
// counter.
void SendSessionNotice460() {
    Obj_SetDateMinutes(g_session, 0, (const GameDate*)Net_CountMessagePart(g_session, 0, 1));
    Net_Send(g_session, 0, nullptr, 0x58, 0, nullptr, 0x11, 0);
}
}

namespace {
// Companies_ForEntry takes the company position in EAX and the key on the stack.
uint8_t* CompanyForEntry484(uint32_t pos, uint32_t key) {
    uint8_t* r;
    __asm {
        mov eax, pos
        push key
        call Companies_ForEntry
        mov r, eax
    }
    return r;
}
}

 // namespace

// 0x46edb0 body. Network train purchase data (count +0, engine +4, running
// +8, company +0x10, the 0x33-byte stops from +0x14): with `validate` clear
// it rebuilds the orders (-1 when a stop's station is gone), creates the
// train (Train_Create) and remembers its lead car's position (0x6ce6f0 /
// 0x6ce6f4 / 0x6ce988, -1 without one). Returns the train, 0 when only
// validating (QUIRK: the validate flag skips all of it).
static int32_t __cdecl NetTrain_CreateImpl(int32_t validate, uint8_t* data) {
    if (validate) return 0;
    uint8_t orders[0x28];
    Cursor33_Init(orders);
    for (int32_t i = 0; i < F<int32_t>(data, 0); ++i) {
        uint8_t* e = data + 0x14 + i * 0x33;
        if ((int8_t)e[0x28] >= 0 && !Collection_IsValid(g_stations, 0, F<uint16_t>(e, 0x29))) return -1;
        TrainOrders_Insert(orders, 0, i, e);
    }
    const uint32_t id = Train_Create(F<uint32_t>(data, 4), orders, 1, F<int32_t>(data, 0x10));
    if (!Collection_IsValid(g_trains, 0, id)) return (int32_t)id;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
    if (F<int32_t>(data, 8) >= 0) Train_SetRunning(t, 0, F<int32_t>(data, 8));
    if (Obj_City41(t)) {
        SceneObj_GetPos((uint8_t*)Obj_City41(t), 0, (uint32_t*)0x6ce6f0, (uint32_t*)0x6ce6f4, (uint32_t*)0x6ce988);
    } else {
        g_6ce6f0 = 0xbf800000;
        g_6ce6f4 = 0xbf800000;
        g_6ce988 = 0xbf800000;
    }
    return (int32_t)id;
}

namespace {
int32_t NetTrainCreate484(int32_t validate, uint8_t* data) {
    int32_t r;
    __asm {
        mov eax, validate
        push data
        call NetTrain_Create
        mov r, eax
    }
    return r;
}
}

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline void Enter(uint32_t cs, uint32_t iat) { hook::Global<CsFn>(iat)((LPCRITICAL_SECTION)cs); }

// The pool pop shared by the functions below.
static void* Pool_Take(uint8_t* pool) {
    if (!F<int32_t>(pool, 0xc)) return ExeNew(F<uint32_t>(pool, 0x10));
    uint8_t* list = F<uint8_t*>(pool, 0);
    uint8_t* node = F<uint8_t*>(list, 0);
    if (!node) return nullptr;
    uint8_t* next = F<uint8_t*>(node, 0);
    F<uint8_t*>(list, 0) = next;
    if (node == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = next;
    --F<int32_t>(list, 8);
    return node;
}

// The pool push: back onto the list when it is enabled and node is in range.
static void Pool_Give(uint8_t* pool, uint8_t* node) {
    if (F<int32_t>(pool, 0xc) && node >= F<uint8_t*>(pool, 4) && node < F<uint8_t*>(pool, 8)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        F<uint8_t*>(node, 0) = F<uint8_t*>(list, 0);
        F<uint8_t*>(list, 0) = node;
        ++F<int32_t>(list, 8);
        if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = node;
    } else ExeDelete(node);
}

// 0x46c2f0 body. The size of file name (0 when it cannot be opened).
static int32_t __cdecl File_SizeImpl(const char* name) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(name, (const char*)0x5ce2c0);
    if (!f) return 0;
    ((int(__cdecl*)(void*, long, int))0x5a261f)(f, 0, 2);
    int32_t size = ((int32_t(__cdecl*)(void*))0x5a254f)(f);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return size;
}

static inline void* Peer() { return g_6cd970; }

// 0x4691d0 body. The first of the 0x80 player slots (0x11c bytes, from base) that is
// in use (+0x104) with id key (+0x10c), or null.
static uint8_t* __cdecl Slots_FindImpl(uint8_t* base, int32_t key) {
    for (int32_t i = 0; i < 0x80; ++i, base += 0x11c)
        if (F<int32_t>(base, 0x104) && key == F<int32_t>(base, 0x10c)) return base;
    return nullptr;
}

static uint32_t* NewZeroed(size_t n, size_t zero) {
    uint32_t* p = (uint32_t*)ExeNew(n);
    if (p) memset(p, 0, zero);
    return p;
}

// 0x471070..0x4711ea body. Stamps the session (+0x8f38) with the date of its
// first pending message part (0 without one) and relays msg's payload to
// everyone as type, with the session id (flags 0x19).
static int32_t Net_RelayStamped(const uint8_t* msg, uint32_t type, uint32_t flags = 0x19) {
    const uint8_t* data = F<const uint8_t*>((void*)msg, 8);
    uint8_t* part = Net_CountMessagePart(g_session, 0, 1);
    uint8_t* s = g_session;
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
    Net_Send(g_session, 0, nullptr, type, F<int32_t>((void*)msg, 0xc), data, flags, 0);
    return 1;
}

// 0x46cbf0 / 0x46cc60 body. Clears the last four bytes (the checksum) of msg's
// n-byte payload, files a heap copy of it in the session's map +0x9050 under its
// CRC and sends it reliably to everyone as type.
static int32_t Net_SendFiled(const uint8_t* msg, uint32_t n, uint32_t clear, uint32_t type) {
    uint8_t* data = F<uint8_t*>((void*)msg, 8);
    F<uint32_t>(data, clear) = 0;
    uint8_t* copy = (uint8_t*)ExeNew(n);
    memcpy(copy, data, n);
    uint32_t crc = Crc32(copy, n);
    Obj_Map9050Insert(g_session, 0, crc, (int32_t)(uintptr_t)copy);
    Net_Send(g_session, 0, nullptr, type, F<int32_t>((void*)msg, 0xc), data, 1, 0);
    return 1;
}

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x46d780. Packet: train id, train +0xcc, car direction, node id,
// position bits. Resolve the first car twice as the original does, move it,
// refresh its pose, and only then read the packet's train value.
static int32_t __cdecl Net_ApplyTrainPosition_Impl(const uint32_t* packet) {
    Collection* trains = g_trains;
    if (!Collection_IsValid(trains, 0, packet[0])) return 0;
    uint8_t* train = (uint8_t*)Collection_Get(g_trains, 0, packet[0]);
    if (!Obj_City41(train)) return 0;
    uint8_t* car = (uint8_t*)Obj_City41(train);
    *(uint32_t*)(car + 0x35a) = packet[2];

    // These two routines deliberately remain original because of stack residue.
    hook::Orig<void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t)>(0x41c700)(car, 0, packet[3], packet[4]);
    int32_t result = hook::Orig<int32_t(__fastcall*)(uint8_t*, int, uint32_t, void*, void*, uint32_t)>(0x41ce80)(
        car, 0, 0, nullptr, nullptr, 0xffffffff);
    *(uint32_t*)(train + 0xcc) = packet[1];
    return result;
}

// 0x472ea0 body. Marks the message (+4, bit 8 of byte +0xb) as already seen when
// the player's received-hash table (other +0x2580, keyed by the session id) holds
// an entry whose checksum equals the message's first word. Always 1.
static int32_t __cdecl Msg_MarkSeenImpl(uint8_t* msg, uint8_t* other) {
    uint32_t first = *F<uint32_t*>(msg, 8);
    if (!first) return 1;
    void** list = (void**)(uintptr_t)Hash_Get(F<HashMap*>(other, 0x2580), 0, F<uint32_t>(g_session, 0x9888));
    if (!list) return 1;
    for (void* n = *list; n; n = *(void**)n)
        if (F<int32_t>(msg, 0xc) > 0 && F<uint8_t*>(msg, 8) && Crc32(F<uint8_t*>(msg, 8), F<uint32_t>(msg, 0xc)) == first) {
            F<uint8_t*>(msg, 4)[0xb] |= 8;
            return 1;
        }
    return 1;
}

// 0x46d980 body. The index of the player-profile entry (table 0x6ceb9c, name +8)
// called name, or 0xff.
static int32_t __cdecl Profiles_IndexOfImpl(const char* name) {
    for (int32_t i = 0; i < Collection_Count(g_players); ++i) {
        Collection* t = g_players;
        uint8_t* e = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (!Stricmp((const char*)e + 8, name)) return i;
    }
    return 0xff;
}

// 0x46ed30 body. Whether a new company clashes with an existing one: 1 when a
// company has its player (+0x37 == p +4), 2 when one has its name (+4 == p +0x10).
static int32_t __cdecl Companies_ClashImpl(uint8_t* p) {
    for (int32_t i = 0; i < Collection_Count(g_companies); ++i) {
        Collection* t = g_companies;
        uint8_t* c = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x37) == F<int32_t>(p, 4)) return 1;
        if (!Stricmp((const char*)c + 4, (const char*)p + 0x10)) return 2;
    }
    return 0;
}

// 0x46f870 body. Applies a received cash transfer { float amount, byte company }:
// the amount (truncated) is booked as stat 0xf and taken from cash (stat 0xd) of
// a company that accepts it. slot is the original's local in the ECX push slot.
static void __cdecl Net_ApplyCashImpl(uint8_t* m, uint32_t* slot) {
    if (!Collection_IsValid(g_companies, 0, m[4])) return;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, m[4]);
    if (!c || !Company_OwnerFlag1e1(c)) return;
    int32_t v = Ftol(*(float*)m);
    float a = (float)v;
    Company_AddStat(c, 0, 0xf, a);
    float b = (float)-v;
    *slot = *(uint32_t*)&b;
    Company_AddStat(c, 0, 0xd, b);
}

// 0x46f8f0 body. Company number i (by position in 0x62be10) when it is a human
// player's company whose profile is the one of texture entry key; else null.
static uint8_t* __cdecl Companies_ForEntryImpl(int32_t i, int32_t key) {
    Collection* t = g_companies;
    uint8_t* c = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
    if (!c || !Company_OwnerFlag1e1(c)) return nullptr;
    int32_t idx = Profiles_IndexOfEntry(key);
    if (idx == 0xff) return nullptr;
    uint32_t id = Collection_IdAt(g_players, 0, (uint32_t)idx, 0);
    if (id == 0xffffffff || id == 0) return nullptr;
    return F<uint32_t>(c, 0x3b) == id ? c : nullptr;
}

// 0x4680b0. Delivers a cargo load: moves 0.2 of its cargo type's amount
// (+0x5a) to delivered (+0x66, flag 1 of +0x12) or +0x62, then unlinks it.
RT3_NOINLINE void __fastcall Load_Deliver(uint32_t* load) {
    Collection* types = g_cargoTypes;
    if (types) {
        uint8_t* ct = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(types, 0x8e + F<uint8_t>(load, 0x10) * 4));
        const double k = *(const double*)0x5c8c20;
        F<float>(ct, 0x5a) = (float)((double)F<float>(ct, 0x5a) - k);
        if (F<uint8_t>(load, 0x12) & 1) F<float>(ct, 0x66) = (float)((double)F<float>(ct, 0x66) + k);
        else F<float>(ct, 0x62) = (float)((double)F<float>(ct, 0x62) + k);
    }
    Load_Unlink(load);
}
RT3_REPLACE(0x4680b0, Load_Deliver);

// 0x468110 / 0x468140. Collection subclass 0x5ce130 (inline 0x19-byte records, 100 + 100).
RT3_NOINLINE Collection* __fastcall Collection5ce130_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce130;
    Collection_Init(c, 0, 1, 0x19, 100, 100, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x468110, Collection5ce130_Construct);

void __fastcall Collection5ce130_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce130;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x468140, Collection5ce130_Destruct);

// 0x468160. Ages every cargo load, releasing the expired ones.
void __fastcall Loads_Age(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ) {
        if (Load_Age((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)))) {
            void** vt = *(void***)t;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(t, Collection_IdAt(t, 0, i, 0));
        } else {
            ++i;
        }
    }
}
RT3_REPLACE(0x468160, Loads_Age);

RT3_DELETE_RECORD(0x4681c0, Loads_Delete, Load_Deliver, uint32_t*)

// 0x4681f0. Releases every record whose u16 +0xa, +0xc or +0xe equals key.
RT3_NOINLINE void __fastcall Table_ReleaseKeyAny(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (key == F<uint16_t>(r, 0xa) || key == F<uint16_t>(r, 0xc) || key == F<uint16_t>(r, 0xe)) {
            void** vt = *(void***)t;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(t, Collection_IdAt(t, 0, i, 0));
        } else {
            ++i;
        }
    }
}
RT3_REPLACE(0x4681f0, Table_ReleaseKeyAny);

// 0x468260. Releases every record whose u16 +8 equals key.
RT3_NOINLINE void __fastcall Table_ReleaseKey8(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ) {
        if (key == F<uint16_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 8)) {
            void** vt = *(void***)t;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(t, Collection_IdAt(t, 0, i, 0));
        } else {
            ++i;
        }
    }
}
RT3_REPLACE(0x468260, Table_ReleaseKey8);

// 0x4682c0. Refreshes every lot's link flag.
RT3_NOINLINE void __fastcall Lots_UpdateLinked(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Lot_UpdateLinked((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, 1);
}
RT3_REPLACE(0x4682c0, Lots_UpdateLinked);

// 0x468300. Clears the region demand for the three lot cargo classes (cargo
// collection +0x88/+0x8a/+0x8c) and rebuilds it from the lots' link flags.
RT3_NOINLINE void __fastcall Lots_RebuildDemand(Collection* c) {
    uint32_t a = F<uint16_t>(Cargos(), 0x88), b = F<uint16_t>(Cargos(), 0x8a), k = F<uint16_t>(Cargos(), 0x8c);
    uint8_t* m = g_map;
    uint8_t* cell = F<uint8_t*>(m, 0x2129);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(g_map, 0x2145); ++x, cell += 0x1d7) {
            F<int32_t>(cell, 0x103 + a * 4) = 0;
            F<int32_t>(cell, 0x103 + b * 4) = 0;
            F<int32_t>(cell, 0x103 + k * 4) = 0;
            m = g_map;
        }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* lot = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        lot[0x12] &= 0xfd;
        Lot_UpdateLinked(lot, 0, 1);
    }
}
RT3_REPLACE(0x468300, Lots_RebuildDemand);

void __fastcall Obj_Free108(void* o) {
    if (!F<void*>(o, 0x108)) return;
    Free(F<void*>(o, 0x108));
    F<void*>(o, 0x108) = nullptr;
}
RT3_REPLACE(0x4683f0, Obj_Free108);

// 0x468420. Number of files matching "<dir>*.mp3".
RT3_NOINLINE int32_t __fastcall CountMp3Files(const char* dir) {
    char pattern[0x100];
    uint8_t findData[0x140];
    FormatText(pattern, (const char*)0x5ce13c, dir, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    int32_t n = 0;
    void* h = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190)(pattern, findData);
    if (h == (void*)-1) return 0;
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    do ++n;
    while (findNext(h, findData) == 1);
    Import<int32_t(__stdcall*)(void*)>(0x5c819c)(h);
    return n;
}
RT3_REPLACE(0x468420, CountMp3Files);

// 0x468490. Lists the music files "<dir>*.mp3" (dir at +0, count +0x100
// from 0x468420) into +0x108: 0x104-byte entries {path, CRC of the name}.
RT3_NOINLINE void __fastcall Music_BuildList(uint8_t* o) {
    if (F<void*>(o, 0x108)) {
        Free(F<void*>(o, 0x108));
        F<void*>(o, 0x108) = nullptr;
    }
    if (F<int32_t>(o, 0x100) <= 0) return;
    uint8_t* e = (uint8_t*)Malloc(F<int32_t>(o, 0x100) * 0x104);
    char pattern[0x100];
    uint8_t find[0x140];
    FormatText(pattern, (const char*)0x5ce13c, (const char*)o, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    F<uint8_t*>(o, 0x108) = e;
    void* h = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190)(pattern, find);
    if (h == (void*)-1) return;
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    const char* fileName = (const char*)find + 0x2c;
    do {
        F<uint32_t>(e, 0x100) = ((uint32_t(__fastcall*)(const char*, int32_t))0x53d810)(fileName, 1);
        FormatText((char*)e, (const char*)0x5c8730, (const char*)o, fileName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        e += 0x104;
    } while (findNext(h, find) == 1);
    Import<int32_t(__stdcall*)(void*)>(0x5c819c)(h);
}
RT3_REPLACE(0x468490, Music_BuildList);

// 0x468580. Picks the next playlist track (+0x10c of +0x100 entries at +0x108,
// 0x104 bytes, name hash +0x100). The first pick prefers the hashed default
// track; shuffle (+0x104) avoids the last few picks (history 0x6ada9c).
RT3_NOINLINE void __fastcall Playlist_Next(uint8_t* p) {
    uint8_t* entries = F<uint8_t*>(p, 0x108);
    uint32_t h = Crc32String((const char*)0x5ce144, 1);
    if (!F<uint8_t*>(p, 0x108)) return;
    if (!F<int32_t>(p, 0x114)) {
        for (int32_t i = 0; i < F<int32_t>(p, 0x100); ++i)
            if (h == F<uint32_t>(entries, 0x100 + i * 0x104)) {
                F<int32_t>(p, 0x10c) = i;
                return;
            }
    }
    int32_t* hist = (int32_t*)0x6ada9c;
    for (;;) {
        if (!F<int32_t>(p, 0x104) || F<int32_t>(p, 0x100) <= 2) {
            F<int32_t>(p, 0x10c) = (F<int32_t>(p, 0x10c) + 1) % F<int32_t>(p, 0x100);
            return;
        }
        int32_t r = Rand(0, F<int32_t>(p, 0x100) - 1);
        int32_t n = F<int32_t>(p, 0x100);
        int32_t k = n > 6 ? 4 : n - 2;
        int32_t i = 0;
        if (k > 0) {
            uint32_t e = F<uint32_t>(F<uint8_t*>(p, 0x108), r * 0x104 + 0x100);
            for (; i < k; ++i)
                if ((uint32_t)hist[i] == e || e == h) break;
        }
        if (i != k) continue;
        int32_t c = k - 2;
        if (c >= 0) do {
            --c;
            hist[c + 2] = hist[c + 1];
        } while (c >= 0);
        hist[0] = F<int32_t>(F<uint8_t*>(p, 0x108), r * 0x104 + 0x100);
        F<int32_t>(p, 0x10c) = r;
        if (F<int32_t>(p, 0x114)) F<int32_t>(p, 0x114)++;
        return;
    }
}
RT3_REPLACE(0x468580, Playlist_Next);

void ResetGlobals4686d0() {
    g_5f2b24 = 1;
    g_6ada98 = 0;
}
RT3_REPLACE(0x4686d0, ResetGlobals4686d0);

// 0x4686f0. Registry key wrapper (vtbl 0x5ce15c): creates/opens `subKey`
// under `root` with full access; +8 is set when that fails.
void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root) {
    o[0] = 0x5ce15c;
    o[2] = 0;
    uint32_t disposition;
    int32_t r = ((int32_t(__stdcall*)(void*, const char*, uint32_t, void*, uint32_t, uint32_t, void*, void*, void*))
                     g_5c8008)(root, subKey, 0, nullptr, 0, 0xf003f, nullptr, &o[1], &disposition);
    if (r) o[2] = 1;
    return o;
}
RT3_REPLACE(0x4686f0, RegKey_Construct);

// 0x468740 / 0x468970. Handle wrapper (vtbl 0x5ce15c): closes handle +4
// through the import 0x5c800c; the deleting form frees itself on flag 1.
uint32_t __fastcall HandleObj_Destruct(void* o) {
    *(uint32_t*)o = 0x5ce15c;
    return ((uint32_t(__stdcall*)(void*))g_5c800c)(F<void*>(o, 4));
}
RT3_REPLACE(0x468740, HandleObj_Destruct);

// 0x468760. Music player constructor: directory "data\music\" and its file list.
uint8_t* __fastcall Music_Construct(uint8_t* o) {
    F<int32_t>(o, 0x104) = 1;
    F<int32_t>(o, 0x108) = 0;
    F<int32_t>(o, 0x110) = 0;
    F<int32_t>(o, 0x10c) = -1;
    F<int32_t>(o, 0x114) = 0;
    char dir[12];
    for (int k = 0; k < 12; ++k) dir[k] = ((const char*)0x5ce160)[k];
    char* d = (char*)o;
    const char* s = dir;
    while ((*d++ = *s++) != 0) {}
    int32_t n = CountMp3Files((const char*)o);
    F<int32_t>(o, 0x100) = n;
    if (n > 0) Music_BuildList(o);
    return o;
}
RT3_REPLACE(0x468760, Music_Construct);

// 0x4687f0. Starts the playlist's current track (+0x108 names of 0x104, index
// +0x10c) when nothing plays (0x6ada98) and it has tracks (+0x100); on a pending
// advance (0x5f2b24) moves to the next and, with the log on, announces its title.
// Quirk: without a path the extension is cut off the playlist's own name.
RT3_NOINLINE void __fastcall Playlist_PlayCurrent(uint8_t* p) {
    if (g_6ada98 || F<int32_t>(p, 0x100) <= 0) return;
    if (g_5f2b24 == 1) {
        g_5f2b24 = 0;
        Playlist_Next(p);
        if (hook::As<int32_t>(g_windowManager)) {
            char msg[0x100], path[0x100];
            char* cur = F<char*>(p, 0x108) + F<int32_t>(p, 0x10c) * 0x104;
            StrCopy(path, cur);
            char* title = ((char*(__cdecl*)(const char*, int))0x5a1df0)(path, '\\');
            title = title ? title + 1 : F<char*>(p, 0x108) + F<int32_t>(p, 0x10c) * 0x104;
            char* dot = ((char*(__cdecl*)(const char*, int))0x5a2a00)(title, '.');
            if (dot) *dot = 0;
            if (F<int32_t>(g_engine, 0x114712) > 0) {
                char* prev = (char*)0x6adab8;
                if (!*prev) FormatText(msg, GetString(0x1c0), title, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                else FormatText(msg, GetString(0x1c1), title, prev, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                if (g_messageLog) Log_Add(g_messageLog, 0, msg, 3, 1, 0);
            }
            StrCopy((char*)0x6adab8, title);
        }
    }
    g_6ada98 = Sound_PlayMusic(g_sound, 0,
                                                      F<char*>(p, 0x108) + F<int32_t>(p, 0x10c) * 0x104,
                                                      (void*)0x4686d0);
}
RT3_REPLACE(0x4687f0, Playlist_PlayCurrent);

// 0x468960. Plays the current track when the playlist is active (+0x110 == 1).
void __fastcall Playlist_Tick(uint8_t* p) {
    if (F<int32_t>(p, 0x110) == 1) Playlist_PlayCurrent(p);
}
RT3_REPLACE(0x468960, Playlist_Tick);

void* __fastcall HandleObj_Delete(void* o, int, uint32_t flags) {
    *(uint32_t*)o = 0x5ce15c;
    ((uint32_t(__stdcall*)(void*))g_5c800c)(F<void*>(o, 4));
    if (flags & 1) ExeFree53b(o);
    return o;
}
RT3_REPLACE(0x468970, HandleObj_Delete);

// 0x4689a0. Writes the string value `name` = data under subKey of this
// registry key (+4). 1 on success. QUIRK: if creating the key fails, the
// subKey pointer is used as the handle.
RT3_NOINLINE int32_t __fastcall RegKey_SetString(void* o, int, const char* subKey, const char* name, const char* data) {
    void* hk = (void*)subKey;
    uint32_t disposition;
    Import<int32_t(__stdcall*)(void*, const char*, uint32_t, void*, uint32_t, uint32_t, void*, void*, void*)>(0x5c8008)(
        F<void*>(o, 4), subKey, 0, nullptr, 0, 0xf003f, nullptr, &hk, &disposition);
    uint32_t len = 0;
    while (data[len]) ++len;
    int32_t ok = 0;
    if (Import<int32_t(__stdcall*)(void*, const char*, uint32_t, uint32_t, const void*, uint32_t)>(0x5c8014)(
            hk, name, 0, 1, data, len + 1) == 0)
        ok = 1;
    Import<int32_t(__stdcall*)(void*)>(0x5c800c)(hk);
    return ok;
}
RT3_REPLACE(0x4689a0, RegKey_SetString);

// 0x468a10. Reads the string value `name` under subKey into 0x6adbb8; returns
// it, or def (which is then written when the value is missing).
// QUIRK: if creating the key fails, def is used as the handle.
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def) {
    void* hk = (void*)def;
    uint32_t disposition, type, size = 0x400;
    Import<int32_t(__stdcall*)(void*, const char*, uint32_t, void*, uint32_t, uint32_t, void*, void*, void*)>(0x5c8008)(
        F<void*>(o, 4), subKey, 0, nullptr, 0, 0xf003f, nullptr, &hk, &disposition);
    const char* result = def;
    int32_t r = Import<int32_t(__stdcall*)(void*, const char*, void*, void*, void*, void*)>(0x5c8000)(
        hk, name, nullptr, &type, (void*)0x6adbb8, &size);
    if (r == 0 && type == 1) result = (const char*)0x6adbb8;
    Import<int32_t(__stdcall*)(void*)>(0x5c800c)(hk);
    if (r == 2 && def) RegKey_SetString(o, 0, subKey, name, def);
    return result;
}
RT3_REPLACE(0x468a10, RegKey_GetString);

// 0x468ac0 (sort callback). Keeps the smaller non-zero key in 0x6cd968:
// 1 when a < b or b is 0.
int32_t __fastcall Key_KeepSmaller(const uint32_t* a, const uint32_t* b) {
    if (*a < *b || !*b) {
        g_6cd968 = *a;
        return 1;
    }
    return 0;
}
RT3_REPLACE(0x468ac0, Key_KeepSmaller);

int32_t __fastcall Obj_FreeBuffer4IfSet(uint8_t* o) {
    if (void* p = F<void*>(o, 4)) Callback_Delete(p);
    return 0;
}
RT3_REPLACE(0x468ae0, Obj_FreeBuffer4IfSet);

// 0x468b00 (callback). Toggles 0x6cd910; returns 0.
int32_t Toggle6cd910() {
    g_6cd910 ^= 1;
    return 0;
}
RT3_REPLACE(0x468b00, Toggle6cd910);

// 0x468b10. Inserts key -> value in the map +4 unless present; 1 if inserted.
RT3_NOINLINE int32_t __fastcall Obj_MapInsertNew(void* o, int, uint16_t key, int32_t value) {
    void* m = F<void*>(o, 4);
    if (((int32_t(__thiscall*)(void*, uint32_t))0x53daa0)(m, key)) return 0;
    ((void(__thiscall*)(void*, uint32_t, int32_t))0x53d960)(F<void*>(o, 4), key, value);
    return 1;
}
RT3_REPLACE(0x468b10, Obj_MapInsertNew);

RT3_NOINLINE void __fastcall Obj_Set8(void* o, int, int32_t v) { F<int32_t>(o, 8) = v; }
RT3_REPLACE(0x468b50, Obj_Set8);

RT3_NOINLINE int32_t __fastcall Obj_Sum987c(void* o) { return F<int32_t>(o, 0x987c) + F<int32_t>(o, 0x9894); }
RT3_REPLACE(0x468b60, Obj_Sum987c);

// 0x468b70. Keeps the entries of the current row (see 0x468c10) whose dword
// +5 exceeds `limit`, compacting them into the other row, which becomes current.
RT3_NOINLINE void __stdcall Table9_Prune(uint8_t* o, uint32_t limit) {
    int32_t n = F<int32_t>(o, 0x178);
    if (n <= 0) return;
    int32_t row = F<int32_t>(o, 0x17c);
    uint8_t* src = o + row * 9 * 512 + 0x180;
    uint8_t* dst = o + (row ^ 1) * 9 * 512 + 0x180;
    int32_t kept = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 0x178); ++i, src += 9) {
        if (limit >= F<uint32_t>(src, 5)) continue;
        F<uint32_t>(dst, 0) = F<uint32_t>(src, 0);
        F<uint32_t>(dst, 4) = F<uint32_t>(src, 4);
        dst[8] = src[8];
        dst += 9;
        ++kept;
    }
    F<int32_t>(o, 0x17c) ^= 1;
    F<int32_t>(o, 0x178) = kept;
}
RT3_REPLACE(0x468b70, Table9_Prune);

// 0x468c10. Appends a 9-byte entry to row +0x17c of the table +0x180 (512 per row), count +0x178.
RT3_NOINLINE void __stdcall Table9_Append(uint8_t* o, const uint8_t* src) {
    int32_t i = F<int32_t>(o, 0x178) + F<int32_t>(o, 0x17c) * 512;
    F<int32_t>(o, 0x178)++;
    uint8_t* d = o + i * 9 + 0x180;
    F<uint32_t>(d, 0) = F<uint32_t>((void*)src, 0);
    F<uint32_t>(d, 4) = F<uint32_t>((void*)src, 4);
    d[8] = src[8];
}
RT3_REPLACE(0x468c10, Table9_Append);

// 0x468c50. Entry of row +0x17c of the 9-byte table (see 0x468c10) matching byte 0 and dword +5.
RT3_NOINLINE uint8_t* __stdcall Table9_Find(uint8_t* o, const uint8_t* e) {
    uint8_t* p = o + F<int32_t>(o, 0x17c) * 9 * 512 + 0x180;
    for (int32_t i = 0; i < F<int32_t>(o, 0x178); ++i, p += 9)
        if (p[0] == e[0] && F<int32_t>(p, 5) == F<int32_t>((void*)e, 5)) return p;
    return nullptr;
}
RT3_REPLACE(0x468c50, Table9_Find);

// 0x468ca0 / 0x468cb0. Forwarders to the map +0x9050.
RT3_NOINLINE uint32_t __fastcall Obj_Map9050Insert(void* o, int, uint32_t key, int32_t value) {
    return ((uint32_t(__thiscall*)(void*, uint32_t, int32_t))0x53d960)(F<void*>(o, 0x9050), key, value);
}
RT3_REPLACE(0x468ca0, Obj_Map9050Insert);

RT3_NOINLINE uint32_t __fastcall Obj_Map9050Query(void* o, int, uint32_t key, int32_t mode) {
    void* m = F<void*>(o, 0x9050);
    if (mode == 1) return ((uint32_t(__thiscall*)(void*, uint32_t))0x53d9e0)(m, key);
    return ((uint32_t(__thiscall*)(void*, uint32_t))0x53dae0)(m, key);
}
RT3_REPLACE(0x468cb0, Obj_Map9050Query);

// 0x468ce0. Whether name +0x905c contains s (exe CRT strstr 0x5a1ff0).
RT3_NOINLINE int32_t __fastcall Obj_NameContains(uint8_t* o, int, const char* s) {
    return ((int(__cdecl*)(const char*, const char*))0x5a1ff0)((const char*)o + 0x905c, s) != 0 ? 1 : 0;
}
RT3_REPLACE(0x468ce0, Obj_NameContains);

// 0x468d00. Adds (mode 1, ';'-separated, if absent) or removes a name in the list +0x905c.
RT3_NOINLINE void __fastcall Obj_EditNameList(uint8_t* o, int, const char* name, int32_t mode) {
    char* list = (char*)o + 0x905c;
    auto append = [](char* dst, const char* src) {
        while (*dst) ++dst;
        while ((*dst++ = *src++) != 0) {}
    };
    if (mode == 1) {
        if (ExeStrstr(list, name)) return;
        append(list, (const char*)0x5cbc48);
        append(list, name);
        return;
    }
    char* p = ExeStrstr(list, name);
    if (!p) return;
    if (p == list) *p = 0;
    else p[-1] = 0;
    size_t len = 0;
    while (name[len]) ++len;
    char* rest = p + len + 1;
    if (!*rest) return;
    if (*rest == ';') ++rest;
    append(list, rest);
}
RT3_REPLACE(0x468d00, Obj_EditNameList);

// 0x469030 (lobby callback). When on (EDX), the lobby switches to mode 7.
void __fastcall Lobby_Mode7Cb(void*, int32_t on, int32_t a, int32_t) {
    if (on) Lobby_SetMode(7, on, 0, a);
}
RT3_REPLACE(0x469030, Lobby_Mode7Cb);

// 0x469190 (callback). Stores its second register argument in 0x6cd974.
void __fastcall Set6cd974_469190(void*, int32_t v, int32_t) { g_6cd974 = v; }
RT3_REPLACE(0x469190, Set6cd974_469190);

// 0x4691a0 (lobby callback). Names the open name dialog with the second
// argument when EDX is set, else with the first and switches the lobby to
// mode 8 for it.
void __fastcall Lobby_NameCb(void*, int32_t edit, const char* a, const char* b, int32_t) {
    if (edit) {
        NameDialog_SetNameIfOpen(b);
        return;
    }
    NameDialog_SetNameIfOpen(a);
    Lobby_SetMode(8, (int32_t)(uintptr_t)a, 0, 0);
}
RT3_REPLACE(0x4691a0, Lobby_NameCb);

// 0x4691d0. Takes the slot array in EAX and the key in EDX (kept).
__declspec(naked) void Slots_Find() {
    __asm {
        push edx
        push edx
        push eax
        call Slots_FindImpl
        add esp, 8
        pop edx
        ret
    }
}
RT3_REPLACE(0x4691d0, Slots_Find);

 // namespace

// 0x469200 (GameSpy server list callback). For a list (EDX) message on the
// server table `slots` (0x80 slots of 0x11c bytes, keyed +0x10c by server):
// 0 adds an unknown server named `name` to the first free slot, 1 refreshes
// a known one, 2 frees it (+0x104). A filled slot holds the name (+4), the
// server, a 9000 ms lifetime, the ping (+0x118), numplayers (+0x110),
// maxplayers (+0x114) and its address text (+0x84).
void __fastcall ServerList_Cb(int32_t, int32_t list, const char* name, void* server, int32_t, int32_t msg, int32_t, uint8_t* slots) {
    if (!list) return;
    auto find = [&]() { return ServerList_FindSlot(slots, server); };
    auto fill = [&](uint8_t* s) {
        strcpy((char*)s + 4, name);
        F<void*>(s, 0x10c) = server;
        F<int32_t>(s, 0x104) = 0x2328;
        F<int32_t>(s, 0x118) = ((int32_t(__fastcall*)(void*))0x58d5a0)(server);
        F<int32_t>(s, 0x110) = ((int32_t(__fastcall*)(void*, const char*, int32_t))0x58d6d0)(server, (const char*)0x5ce1a0, 0);
        F<int32_t>(s, 0x114) = ((int32_t(__fastcall*)(void*, const char*, int32_t))0x58d6d0)(server, (const char*)0x5ce194, 0);
        strcpy((char*)s + 0x84, ((const char*(__fastcall*)(void*))0x58d1e0)(server));
    };
    if (msg == 0) {
        if (find() || !name) return;
        uint8_t* s = slots;
        int32_t i = 0;
        for (; i < 0x80 && F<int32_t>(s, 0x104); ++i) s += 0x11c;
        if (i == 0x80) return;
        fill(s);
    } else if (msg == 1) {
        uint8_t* s = find();
        if (!s || !name) return;
        fill(s);
    } else if (msg == 2) {
        uint8_t* s = find();
        if (!s || !name) return;
        F<int32_t>(s, 0x104) = 0;
    }
}
RT3_REPLACE(0x469200, ServerList_Cb);

// 0x4693b0 (GameSpy server list callback). Keeps server name `name` (up to
// 500 of 256 bytes at 0x6ae4d8) for a successful entry without flags 0x14.
RT3_NOINLINE void __fastcall Lobby_AddServerCb(void*, int32_t ok, int32_t error, int32_t index, const char* name, uint32_t flags, int32_t) {
    if (!ok || error || !name || index == -1) return;
    const int32_t n = g_6cd97c;
    if (n >= 0x1f4) return;
    if (flags && (flags & 0x14)) return;
    g_6cd97c = n + 1;
    strcpy((char*)(0x6ae4d8 + (n << 8)), name);
}
RT3_REPLACE(0x4693b0, Lobby_AddServerCb);

// 0x469410 (GameSpy callback). Remembers EDX (0x6cd978); when set, clears
// the 0x1f400-byte table 0x6ae4d8 and starts the query 0x58da60 with the
// callback 0x4693b0.
void __fastcall Lobby_StartQueryCb(uint8_t* s, int32_t on, int32_t, int32_t, int32_t) {
    g_6cd978 = on;
    if (!on) return;
    memset((void*)0x6ae4d8, 0, 0x7d00 * 4);
    g_6cd97c = 0;
    ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))0x58da60)(s, 0, 0x4693b0, 0);
}
RT3_REPLACE(0x469410, Lobby_StartQueryCb);

// 0x469450. Shuts the peer down: leaves the room and stops listing when connected,
// then disconnects and frees it.
void Peer_Shutdown() {
    if (!Peer()) return;
    if (g_6cd978) {
        if (g_6cd920) ((void(__fastcall*)(void*, int32_t))0x58d960)(Peer(), 0);
        if (g_6cd978 && g_6cd920) ((void(__fastcall*)(void*))0x58db20)(Peer());
    }
    ((void(__fastcall*)(void*))0x58d830)(Peer());
    ((void(__fastcall*)(void*))0x58de90)(Peer());
    g_6cd970 = nullptr;
}
RT3_REPLACE(0x469450, Peer_Shutdown);

// 0x4694c0. Leaves the staging room when connected.
RT3_NOINLINE void Peer_LeaveStaging() {
    if (!Peer() || !g_6cd978 || !g_6cd920) return;
    ((void(__fastcall*)(void*, int32_t))0x58d960)(Peer(), 1);
    ((void(__fastcall*)(void*, const char*, int32_t))0x58e100)(Peer(), (const char*)0x5c87a8, 1);
}
RT3_REPLACE(0x4694c0, Peer_LeaveStaging);

// 0x469500. GameSpy: pumps the master-server object (0x6cd970) if present.
RT3_NOINLINE void GameSpy_Pump() {
    void* o = g_6cd970;
    if (!o) return;
    ((void(__fastcall*)(void*))0x58db60)(o);
    ((void(__fastcall*)(void*))0x58def0)(g_6cd970);
}
RT3_REPLACE(0x469500, GameSpy_Pump);

// 0x469520. Starts the server list update with the 8 standard keys (callback 0x469200).
RT3_NOINLINE int32_t __fastcall Lobby_UpdateList(uint8_t* lobby) {
    if (g_6cd920) return g_6cd920;
    F<int32_t>(lobby, 0x8f18) = 1;
    uint8_t keys[8] = { 3, 1, 4, 8, 10, 11, 0x13, 5 };
    return ((int32_t(__fastcall*)(void*, uint8_t*, int32_t, int32_t, uint32_t, uint8_t*))0x58d970)(Peer(), keys, 8, 0, 0x469200, lobby + 0x10);
}
RT3_REPLACE(0x469520, Lobby_UpdateList);

// 0x469580. Starts listing the game servers: clears the 32000-dword list at 0x6ae4d8
// and its count, then asks the peer (callback 0x4693b0). Returns the peer's result,
// or the busy flag 0x6cd920 when already busy.
RT3_NOINLINE int32_t __fastcall Lobby_ListGames(uint8_t* lobby) {
    if (g_6cd920) return g_6cd920;
    F<int32_t>(lobby, 0x9874) = 1;
    memset((void*)0x6ae4d8, 0, 0x7d00 * 4);
    g_6cd97c = 0;
    return ((int32_t(__fastcall*)(void*, int32_t, uint32_t, int32_t))0x58da60)(Peer(), 0, 0x4693b0, 0);
}
RT3_REPLACE(0x469580, Lobby_ListGames);

// 0x4695d0. Next non-empty 256-byte line of the buffer 0x6ae4d8 (cursor
// 0x6cd97c, reset when +0x9874 is set); 0 at the end or while 0x6cd920 is set.
const char* __fastcall Obj_NextBufferedLine(void* o) {
    if (g_6cd920) return nullptr;
    if (F<int32_t>(o, 0x9874) == 1) {
        g_6cd97c = 0;
        F<int32_t>(o, 0x9874) = 0;
    }
    const char* p = (const char*)0x6ae4d8 + g_6cd97c * 256;
    if (!*p) return nullptr;
    g_6cd97c++;
    return p;
}
RT3_REPLACE(0x4695d0, Obj_NextBufferedLine);

// 0x469620. Builds the room key from name and asks the peer to join it.
RT3_NOINLINE int32_t __stdcall Peer_JoinNamed(const char* name) {
    char key[0x100];
    ((void(__fastcall*)(char*, const char*))0x58e650)(key, name);
    return ((int32_t(__fastcall*)(void*, const char*, uint32_t, int32_t, int32_t))0x58df20)(Peer(), key, 0x4691a0, 0, 1);
}
RT3_REPLACE(0x469620, Peer_JoinNamed);

// 0x469660. Sends a message to the peer's room (no-op for null).
RT3_NOINLINE int32_t __stdcall Peer_SendRoom(const char* msg) {
    if (!msg) return 0;
    return ((int32_t(__fastcall*)(void*, int32_t, const char*, int32_t))0x58d9e0)(Peer(), 0, msg, 0);
}
RT3_REPLACE(0x469660, Peer_SendRoom);

// 0x469680. Polls the peer object (the argument is unused).
RT3_NOINLINE int32_t __stdcall Peer_Think(int32_t) { return ((int32_t(__fastcall*)(void*))0x58db00)(Peer()); }
RT3_REPLACE(0x469680, Peer_Think);

// 0x469690. Singly linked list {head, tail, count}: insert after `after` (NULL = at head).
RT3_NOINLINE void __fastcall SList_InsertAfter(void** l, int, void** after, void** node) {
    if (after) {
        *node = *after;
        *after = node;
        ((int32_t*)l)[2]++;
        if (after != (void**)l[1]) return;
    } else {
        *node = l[0];
        l[0] = node;
        ((int32_t*)l)[2]++;
        if (l[1]) return;
    }
    l[1] = node;
}
RT3_REPLACE(0x469690, SList_InsertAfter);

// 0x4696d0. Takes a block from the free list (+0 list, +0xc enabled) or, when the
// list is disabled, allocates one of size +0x10. Null when the list is empty.
RT3_NOINLINE void* __fastcall FreeList_Take(uint8_t* q) {
    if (!F<int32_t>(q, 0xc)) return ExeNew(F<uint32_t>(q, 0x10));
    uint8_t* list = F<uint8_t*>(q, 0);
    uint8_t* node = F<uint8_t*>(list, 0);
    if (!node) return nullptr;
    uint8_t* next = F<uint8_t*>(node, 0);
    F<uint8_t*>(list, 0) = next;
    if (node == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = next;
    --F<int32_t>(list, 8);
    return node;
}
RT3_REPLACE(0x4696d0, FreeList_Take);

RT3_NOINLINE void __fastcall Obj_SetName8f48(uint8_t* o, int, const char* s) {
    char* d = (char*)o + 0x8f48;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x469700, Obj_SetName8f48);

// 0x469720. Multiplayer ping overlay: one "<name> = <n>ms" line (0x5ce1ac) per
// player in the network list (node name +8, ping +0x54), in yellow in the
// lobby's font (+0x9884), while the switch 0x6cd910 is on.
void __fastcall Lobby_DrawQueue(uint8_t* lobby) {
    if (!g_6cd910 || !F<uint8_t*>(lobby, 0x9884)) return;
    char line[0x100], text[0x400];
    text[0] = 0;
    if (uint8_t** q = (uint8_t**)(uintptr_t)Net_LockQueue()) {
        for (uint8_t* node = q[0]; node; node = F<uint8_t*>(node, 0)) {
            ExeSprintf()(line, (const char*)0x5ce1ac, (const char*)(node + 8), F<int32_t>(node, 0x54));
            strcat(text, line);
        }
        Net_UnlockQueue();
    }
    Font_DrawTextBox(F<uint8_t*>(lobby, 0x9884), 0, text, BitsF(0x3d4ccccd), 0.0f, BitsF(0x3f666666), 1.0f, 0xffffff00, 3, 1, 0, 1.0f);
}
RT3_REPLACE(0x469720, Lobby_DrawQueue);

// 0x469800. A network packet buffer: up to 0x300 bytes from the session's pool
// (+0x8f3c, under the lock 0x6cd940, spinning while its first word is set),
// larger ones (or when the pool gives none) from the heap.
RT3_NOINLINE void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size) {
    if (size <= 0x300) {
        while (*(volatile int32_t*)0x6cd940) {
        }
        Enter(0x6cd940, 0x5c8070);
        void* p = Pool_Take(F<uint8_t*>(s, 0x8f3c));
        Enter(0x6cd940, 0x5c8074);
        if (p) return p;
    }
    return ExeNew((size_t)size);
}
RT3_REPLACE(0x469800, NetPacket_Alloc);

// 0x469890. Frees a packet buffer: those inside the session's 0x48000-byte block
// (+0x8f40) go back to its pool, the rest to the heap.
RT3_NOINLINE void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p) {
    uint8_t* base = F<uint8_t*>(s, 0x8f40);
    if (p < base || p >= base + 0x48000) {
        ExeDelete(p);
        return;
    }
    while (*(volatile int32_t*)0x6cd940) {
    }
    Enter(0x6cd940, 0x5c8070);
    Pool_Give(F<uint8_t*>(s, 0x8f3c), p);
    Enter(0x6cd940, 0x5c8074);
}
RT3_REPLACE(0x469890, NetPacket_Free);

// 0x469930. Appends a new 0xa4-byte message node (pool +0x8f20) {a, b, c, text}
// to the list +0x8f28. QUIRK: an exhausted pool writes through null.
RT3_NOINLINE int32_t __fastcall Obj_AddMessage(uint8_t* o, int, int32_t a, int32_t b, int32_t c, const char* text) {
    uint32_t* n = (uint32_t*)Pool_Alloc(F<int32_t*>(o, 0x8f20));
    for (int k = 0; k < 0x29; ++k) n[k] = 0;
    n[1] = c;
    n[2] = b;
    n[5] = a;
    char* d = (char*)n + 0x18;
    while ((*d++ = *text++) != 0) {}
    void** l = F<void**>(o, 0x8f28);
    if (void** tail = (void**)l[1]) {
        n[0] = (uint32_t)tail[0];
        tail[0] = n;
        ((int32_t*)l)[2]++;
        l[1] = n;
    } else {
        n[0] = (uint32_t)l[0];
        l[0] = n;
        ((int32_t*)l)[2]++;
        if (!l[1]) l[1] = n;
    }
    return 1;
}
RT3_REPLACE(0x469930, Obj_AddMessage);

// 0x4699e0. Value (+4) of the node with key +0x14 in the list *(+0x8f28); 0 if none.
RT3_NOINLINE uint32_t __fastcall Obj_FindNode8f28(void* o, int, int32_t key) {
    for (uint8_t* n = *F<uint8_t**>(o, 0x8f28); n; n = *(uint8_t**)n)
        if (F<int32_t>(n, 0x14) == key) return F<uint32_t>(n, 4);
    return 0;
}
RT3_REPLACE(0x4699e0, Obj_FindNode8f28);

// 0x469a10. Counts the list's entries (from +8) whose item is not kind 'g' and lacks flag 4 at +0xb.
RT3_NOINLINE void __fastcall Obj_CountItems(void* o, int, uint8_t* list) {
    F<int32_t>(o, 0x9894) = 0;
    if (!list) return;
    for (uint8_t** n = F<uint8_t**>(list, 8); n; n = (uint8_t**)*n) {
        uint8_t* e = (uint8_t*)n[1];
        if (e[9] != 0x67 && !(e[0xb] & 4)) F<int32_t>(o, 0x9894)++;
    }
}
RT3_REPLACE(0x469a10, Obj_CountItems);

// 0x469a50. Queues a text message (fields a..d, time stamp, text) at the end of
// the session's chat queue (+0x8f2c).
RT3_NOINLINE uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d) {
    uint8_t* p = (uint8_t*)NetPacket_Alloc(s, 0, (int32_t)strlen(text) + 0x21);
    F<int32_t>(p, 0x14) = a;
    F<int32_t>(p, 4) = c;
    F<int32_t>(p, 0x18) = d;
    F<int32_t>(p, 0x10) = b;
    F<int32_t>(p, 0x1c) = 0;
    F<int32_t>(p, 8) = ClockMs();
    strcpy((char*)p + 0x20, text);
    uint8_t* q = F<uint8_t*>(s, 0x8f2c);
    uint8_t* tail = F<uint8_t*>(q, 4);
    if (tail) {
        F<uint8_t*>(p, 0) = F<uint8_t*>(tail, 0);
        F<uint8_t*>(tail, 0) = p;
    } else {
        F<uint8_t*>(p, 0) = F<uint8_t*>(q, 0);
        F<uint8_t*>(q, 0) = p;
    }
    ++F<int32_t>(q, 8);
    if (tail || !F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = p;
    return p;
}
RT3_REPLACE(0x469a50, Net_QueueText);

// 0x469b00. Shows the next queued network message of session s (+0x8f2c
// queue; not re-entrant, 0x6cd980): unlinks it, runs its message box (text
// +0x20, type +0x14, timeout +0x10), hands the result to its callback +0x18
// and frees it.
RT3_NOINLINE void __fastcall NetMessage_ShowNext(uint8_t* s) {
    if (g_6cd980) return;
    uint8_t** q = F<uint8_t**>(s, 0x8f2c);
    uint8_t* node = q[0];
    g_6cd980 = 1;
    if (node) {
        uint8_t* next = F<uint8_t*>(node, 0);
        q[0] = next;
        if (node == q[1]) q[1] = next;
        --*(int32_t*)&q[2];
        uint32_t r = Message_Show((const char*)(node + 0x20), F<int32_t>(node, 0x14), F<int32_t>(node, 0x10));
        if (void(__fastcall * cb)(uint8_t*, uint32_t) = F<void(__fastcall*)(uint8_t*, uint32_t)>(node, 0x18)) cb(node, r);
        NetPacket_Free(s, 0, node);
    }
    g_6cd980 = 0;
}
RT3_REPLACE(0x469b00, NetMessage_ShowNext);

// 0x469b90. Whether every network player has flag 4 (1 when not networked).
RT3_NOINLINE int32_t Net_AllPlayersFlag4() {
    if (!g_6d40dc) return 1;
    int32_t ok = 1;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e) {
        if (!(e[0x5c] & 4)) {
            ok = 0;
            break;
        }
    }
    Net_UnlockQueue();
    return ok;
}
RT3_REPLACE(0x469b90, Net_AllPlayersFlag4);

// 0x469be0. Builds the message status packet {0xff, count, date minutes,
// count x value} from the message list +0x8f28 (each node's callback +4);
// stamps the nodes and +0x8f44 with the date.
RT3_NOINLINE uint8_t* __fastcall Obj_BuildStatusPacket(uint8_t* o, int, int32_t* outSize) {
    int32_t count = F<int32_t>(F<uint8_t*>(o, 0x8f28), 8);
    int32_t size = count * 4 + 6;
    *outSize = size;
    uint8_t* buf = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(size);
    uint8_t* w = g_world;
    int32_t minutes = DateToMinutes((const GameDate*)(w + 0xd));
    buf[0] = 0xff;
    buf[1] = F<uint8_t>(F<uint8_t*>(o, 0x8f28), 8);
    F<int32_t>(buf, 2) = minutes;
    uint8_t* out = buf + 6;
    for (uint8_t* n = *F<uint8_t**>(o, 0x8f28); n; n = *(uint8_t**)n, out += 4) {
        F<int32_t>(n, 0xc) = 0;
        uint8_t* r = ((uint8_t*(__cdecl*)())F<void*>(n, 4))();
        if (F<int32_t>(r, 1)) g_6cd908 |= 1u << (r[0] & 31);
        w = g_world;
        F<uint32_t>(n, 0x9c) = F<uint32_t>(w, 0xd);
        F<uint32_t>(n, 0xa0) = F<uint32_t>(w, 0x11);
        F<uint32_t>(out, 0) = F<uint32_t>(r, 1);
    }
    F<int32_t>(o, 0x8f44) = minutes;
    return buf;
}
RT3_REPLACE(0x469be0, Obj_BuildStatusPacket);

// 0x469ca0. Refreshes the surface description (+0x54 from +0x269c) of every queued
// texture entry except the current frame's (network play with a world only).
RT3_NOINLINE void __stdcall Textures_RefreshQueued(int32_t) {
    if (hook::As<void*>(g_world) && !g_6cd910) return;
    if (!Engine_FirstListEntry()) return;
    if (Global6d4100() <= 1) return;
    uint8_t** l = (uint8_t**)(uintptr_t)Net_LockQueue();
    if (!l) return;
    for (uint8_t* n = *l; n; n = F<uint8_t*>(n, 0)) {
        int32_t id = F<int32_t>(n, 4);
        if (id == Global6d40e4()) continue;
        F<int32_t>(n, 0x54) = 0;
        if (Textures_QueryDesc(g_6d40d0, 0, id, 0)) F<int32_t>(n, 0x54) = F<int32_t>(n, 0x269c);
    }
    Net_UnlockQueue();
}
RT3_REPLACE(0x469ca0, Textures_RefreshQueued);

// 0x469d30. Sends message type (size bytes at data) to player to (null: everyone)
// under the send lock 0x6cd960. Chat (type 1) goes to the GameSpy room instead
// while connected there (+0x985c) and 0x6d4100 is clear. Flags: 0x10 prefixes the
// session id (+0x8f38, send flag 0x40000000); 4 also queues a copy (0x28-byte
// header) on the first engine list's queue and sends an empty type 0xe instead,
// reliable unless 0x6d40dc is set; 1 is reliable (8, no timeout instead of 2000);
// 2 adds 0x20; 0x20 adds 0x20000000; 8, and anything of 0x30 bytes or more, is
// LZSS-packed when that succeeds (0x80000000). A serial also stamps the time.
// Totals of raw / sent bytes go to 0x6cd930 / 0x6cd934.
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                         int32_t serial) {
    typedef LONG(WINAPI * LockFn)(LONG volatile*);
    uint32_t sendFlags = 0, idFlag = 0, timeout = 2000;
    if ((uint16_t)type == 1) {
        if (!data || !size) return;
        if (F<int32_t>(s, 0x985c) == 1 && !Global6d4100()) {
            Peer_SendRoom((const char*)data);
            return;
        }
    }
    while (*(volatile LONG*)0x6cd960) {
    }
    hook::As<LockFn>(g_5c8070)((LONG volatile*)0x6cd960);
    uint8_t local[0x1000];
    if (flags) {
        if (flags & 0x10) {
            *(uint32_t*)local = F<uint32_t>(s, 0x8f38);
            idFlag = 0x40000000;
            if (size > 0) memcpy(local + 4, data, (uint32_t)size);
            data = local;
            size += 4;
        }
        if (flags & 4) {
            flags = (flags & ~4u) | 1;
            if (g_6d40dc) flags &= ~2u;
            else flags |= 2;
            uint8_t* pkt = (uint8_t*)NetPacket_Alloc(s, 0, size + 0x28);
            memset(pkt, 0, (uint32_t)(size + 0x28));
            F<uint32_t>(pkt, 0x10) = flags;
            if (flags & 8) flags &= ~8u;
            F<uint32_t>(pkt, 0x14) = (uint16_t)type;
            F<int32_t>(pkt, 0xc) = size;
            if (size > 0) {
                F<uint8_t*>(pkt, 8) = pkt + 0x28;
                memcpy(pkt + 0x28, data, (uint32_t)size);
            }
            void** q = F<void**>(Engine_FirstListEntry(), 0x64);
            SList_InsertAfter(q, 0, (void**)q[1], (void**)pkt);
            data = nullptr;
            size = 0;
            type = 0xe;
        }
        if (flags & 1) {
            timeout = 0;
            sendFlags = 8;
        }
        if (flags & 2) sendFlags |= 0x20;
        if (flags & 0x20) sendFlags |= 0x20000000;
    }
    int32_t now = 0;
    if (serial) now = ClockMs();
    SendToFn send = (SendToFn)&Net_SendTo;
    uint8_t* net = g_6d40d0;
    g_6cd930 += size;
    if (!(flags & 8) && size < 0x30) {
        g_6cd934 += size;
        send(net, 0, data, (uint32_t)size, type, 0xff, to, sendFlags | idFlag, timeout, serial, now);
        hook::As<LockFn>(g_5c8074)((LONG volatile*)0x6cd960);
        return;
    }
    uint8_t* packed = nullptr;
    int32_t packedSize = 0;
    int32_t packer[4] = { 0x91, 0, 0, (int32_t)(uintptr_t)s };
    if (Pack_Compress(packer, 0, data, size, &packed, &packedSize) == 1) {
        g_6cd934 += packedSize;
        send(net, 0, packed, (uint32_t)packedSize, type, 0xff, to, sendFlags | idFlag | 0x80000000, timeout, serial, now);
        if (packed) ((void(__cdecl*)(void*))0x5a1145)(packed);
    } else {
        g_6cd934 += size;
        send(net, 0, data, (uint32_t)size, type, 0xff, to, sendFlags | idFlag, timeout, serial, now);
    }
    ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
    hook::As<LockFn>(g_5c8074)((LONG volatile*)0x6cd960);
}
RT3_REPLACE(0x469d30, Net_Send);

// 0x46a030. Hosts game as name (password optional: 0x6cd928 says whether one is
// set) unless a connection is up (0x6d40dc): a running server-list request is
// stopped first; the names are kept at +0x8e10 / +0x8f48. The GameSpy peer then
// leaves its room and stops listing.
RT3_NOINLINE void __fastcall Lobby_HostGame(uint8_t* lobby, int, const char* game, const char* name, const char* password, int32_t ms) {
    if (g_6d40dc) return;
    if (F<int32_t>(lobby, 0x8f18) == 1) {
        if (!F<int32_t>(lobby, 0x985c)) Nop5217b0(hook::As<void*>(g_6d40d0));
        F<int32_t>(lobby, 0x8f18) = 0;
    }
    F<int32_t>(lobby, 0x8f10) = 0;
    strcpy((char*)lobby + 0x8e10, game);
    strcpy((char*)lobby + 0x8f48, name);
    const char* pw = nullptr;
    if (!password) g_6cd928 = 0;
    else {
        g_6cd928 = *password != 0;
        pw = *password ? password : nullptr;
    }
    Net_Host(g_6d40d0, 0, game, name, pw, ms, 0);
    if (void* peer = Peer()) {
        ((void(__fastcall*)(void*))0x58d9d0)(peer);
        ((void(__fastcall*)(void*))0x58e1f0)(Peer());
    }
}
RT3_REPLACE(0x46a030, Lobby_HostGame);

// 0x46a110. Joins the game at address host as name (password optional): the
// game's title (from the server list, matched on its address +0x80) goes to
// +0x8e10; +0xc becomes 1 while connecting, 2 when the connect fails at once.
RT3_NOINLINE void __fastcall Lobby_JoinGame(uint8_t* lobby, int, const char* host, const char* name, const char* password) {
    uint8_t* g = lobby + 0x14;
    for (int32_t k = 0; k < 0x80; ++k, g += 0x11c)
        if (!((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)g + 0x80, host)) strcpy((char*)lobby + 0x8e10, (const char*)g);
    F<int32_t>(lobby, 0xc) = 1;
    const char* pw = password && *password ? password : nullptr;
    if (Net_Connect(g_6d40d0, 0, host, name, pw, 0) < 0) F<int32_t>(lobby, 0xc) = 2;
}
RT3_REPLACE(0x46a110, Lobby_JoinGame);

// 0x46a1a0. Starts a fresh server list (cleared 0x2380 dwords at +0x10) unless one
// is in progress, updating it from the peer when connected (+0x985c) and otherwise
// resetting the outgoing traffic; returns the list.
RT3_NOINLINE uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby) {
    if (!F<int32_t>(lobby, 0x8f18)) {
        F<int32_t>(lobby, 0x8f10) = 0;
        F<int32_t>(lobby, 0x8f18) = 1;
        memset(lobby + 0x10, 0, 0x2380 * 4);
        if (!F<int32_t>(lobby, 0x985c)) {
            Net_ResetOutgoing();
            return lobby + 0x10;
        }
        Lobby_UpdateList(lobby);
    }
    return lobby + 0x10;
}
RT3_REPLACE(0x46a1a0, Lobby_FreshList);

// 0x46a200. Message (0x6adfc0) naming the players still without flag 8
// (string 0xb71 with the ", "-joined names) or string 0xb72 if none.
RT3_NOINLINE const char* Net_WaitingPlayersText() {
    char names[0x100];
    names[0] = 0;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e) {
        if (e[0x5c] & 8) continue;
        char* d = names;
        while (*d) ++d;
        for (const char* s = (const char*)e + 8; (*d++ = *s++) != 0;) {}
        d = names;
        while (*d) ++d;
        d[0] = ((const char*)0x5c9f38)[0];
        d[1] = ((const char*)0x5c9f38)[1];
        d[2] = ((const char*)0x5c9f38)[2];
    }
    if (names[0]) {
        size_t len = 0;
        while (names[len]) ++len;
        names[len - 2] = 0;
        FormatText((char*)0x6adfc0, GetString(0xb71), names, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    } else {
        const char* s = GetString(0xb72);
        char* d = (char*)0x6adfc0;
        while ((*d++ = *s++) != 0) {}
    }
    Net_UnlockQueue();
    return (const char*)0x6adfc0;
}
RT3_REPLACE(0x46a200, Net_WaitingPlayersText);

// 0x46a310. A packet copy of message m (0x28-byte header, its 12-byte address block
// at +0x28 and the payload after its first word at +0x34, linked from +8 when
// present; size +0xc less 4), unlinked.
RT3_NOINLINE uint8_t* __fastcall NetPacket_Copy(uint8_t* s, int, const uint8_t* m) {
    uint8_t* p = (uint8_t*)NetPacket_Alloc(s, 0, F<int32_t>((void*)m, 0xc) + 0x30);
    memcpy(p, m, 0x28);
    F<int32_t>(p, 0) = 0;
    F<int32_t>(p, 0x14) = 0;
    F<int32_t>(p, 0x1c) = 0;
    memcpy(p + 0x28, F<void*>((void*)m, 4), 12);
    F<uint8_t*>(p, 4) = p + 0x28;
    int32_t n = F<int32_t>((void*)m, 0xc);
    uint8_t* payload = nullptr;
    if (n > 4) {
        payload = p + 0x34;
        memcpy(payload, F<uint8_t*>((void*)m, 8) + 4, (size_t)(n - 4));
    }
    F<uint8_t*>(p, 8) = payload;
    F<int32_t>(p, 0xc) = F<int32_t>(p, 0xc) - 4;
    return p;
}
RT3_REPLACE(0x46a310, NetPacket_Copy);

// 0x46a390. Sets +0x8f30 and stamps the current date (+0x9048) and its minutes (+0x8f38).
RT3_NOINLINE void __fastcall Obj_StampDate(uint8_t* o, int, int32_t v) {
    F<int32_t>(o, 0x8f30) = v;
    uint8_t* w = g_world;
    F<uint32_t>(o, 0x9048) = F<uint32_t>(w, 0xd);
    F<uint32_t>(o, 0x904c) = F<uint32_t>(w, 0x11);
    F<int32_t>(o, 0x8f38) = DateToMinutes((const GameDate*)(o + 0x9048));
}
RT3_REPLACE(0x46a390, Obj_StampDate);

// 0x46a3d0. Whether the session's current message (stamped at the date +0x9048) is
// complete: true unless the frame's entry (0x6d40d0 list) has it with fewer parts
// received (+0x10) than expected (+0). Releases the entry's lock afterwards.
RT3_NOINLINE int32_t __fastcall Net_MessageComplete(uint8_t* s) {
    int32_t key = DateToMinutes((const GameDate*)(s + 0x9048));
    int32_t ok = 1;
    uint8_t* e = Textures_Find(Global6d40e4());
    if (e) {
        uint8_t* h = (uint8_t*)(uintptr_t)Hash_Get(F<HashMap*>(e, 0x2580), 0, (uint32_t)key);
        if (h) ok = F<int32_t>(h, 0x10) >= F<int32_t>(h, 0);
        Net_UnlockQueueArg(Global6d40e4());
    }
    return ok;
}
RT3_REPLACE(0x46a3d0, Net_MessageComplete);

// 0x46a440. Whether a network player has this name.
RT3_NOINLINE int32_t __fastcall Net_HasPlayer(void*, int, const char* name) {
    int32_t found = 0;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e) {
        if (Stricmp((const char*)e + 8, name) == 0) {
            found = 1;
            break;
        }
    }
    Net_UnlockQueue();
    return found;
}
RT3_REPLACE(0x46a440, Net_HasPlayer);

// 0x46a4b0. The common game version (lowest non-zero version among the queued
// session players, else our own), as an integer x 0x5c85d8; cached in 0x6cd96c.
RT3_NOINLINE float __fastcall Net_CommonVersion() {
    if (int32_t v = g_6cd96c) return (float)v;
    float best = 99999.0f;
    for (uint8_t* n = *(uint8_t**)Net_LockQueue(); n; n = *(uint8_t**)n) {
        float v = F<float>(n, 0x268c);
        if (v < best && !(*(const float*)0x5c8598 == v)) best = v;
    }
    Net_UnlockQueue();
    if (best == *(const float*)0x5c8598 || *(uint32_t*)&best == 0x47c34f80) best = Game_VersionNumber();
    int32_t r;
    __asm {
        fld best
        fadd qword ptr ds:[0x5c88a0]
        fmul qword ptr ds:[0x5c85d8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    g_6cd96c = r;
    return (float)r;
}
RT3_REPLACE(0x46a4b0, Net_CommonVersion);

// 0x46a590. Frees every packet queued on the session (+0x9878 queue).
RT3_NOINLINE void __fastcall NetQueue_Drain(uint8_t* s) {
    for (;;) {
        uint8_t* q = F<uint8_t*>(s, 0x9878);
        if (!q) return;
        uint8_t* n = F<uint8_t*>(q, 0);
        if (!n) return;
        F<uint8_t*>(q, 0) = F<uint8_t*>(n, 0);
        if (n == F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = F<uint8_t*>(n, 0);
        --F<int32_t>(q, 8);
        NetPacket_Free(s, 0, n);
    }
}
RT3_REPLACE(0x46a590, NetQueue_Drain);

// 0x46a5f0. Queues a copy of the 16-byte message m with tag (front of +0x9878);
// messages with id 999999 are dropped.
RT3_NOINLINE void __fastcall NetQueue_Push(uint8_t* s, int, const uint32_t* m, int32_t tag) {
    uint8_t* q = F<uint8_t*>(s, 0x9878);
    if (!q || m[0] == 0xf423f) return;
    uint8_t* p = (uint8_t*)NetPacket_Alloc(s, 0, 0x18);
    F<int32_t>(p, 0x14) = tag;
    memcpy(p + 4, m, 16);
    q = F<uint8_t*>(s, 0x9878);
    F<uint8_t*>(p, 0) = F<uint8_t*>(q, 0);
    F<uint8_t*>(q, 0) = p;
    ++F<int32_t>(q, 8);
    if (!F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = p;
}
RT3_REPLACE(0x46a5f0, NetQueue_Push);

// 0x46a660. Whether rect {x0, x1, y0, y1} overlaps none of the list +0x9878's
// rects other than `key`'s (always 1 when x0 is 999999).
RT3_NOINLINE int32_t __fastcall Obj_RectFree(void* o, int, const int32_t* r, int32_t key) {
    uint8_t** list = F<uint8_t**>(o, 0x9878);
    if (!list || r[0] == 999999) return 1;
    for (uint8_t* e = *list; e; e = *(uint8_t**)e) {
        if (F<int32_t>(e, 0x14) == key) continue;
        if (r[3] < F<int32_t>(e, 0xc) || r[2] > F<int32_t>(e, 0x10)) continue;
        if (r[1] < F<int32_t>(e, 4) || r[0] > F<int32_t>(e, 8)) continue;
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x46a660, Obj_RectFree);

// 0x46a830 (GameSpy name callback). After 25 tries (0x6cd984) the lobby
// gives up (0x58e2e0); the first try switches the lobby to mode 5 (6 with
// EDX). Each try names this peer "<0x5ce1e4><random 0..999>" (0x6ae0c0).
void __fastcall Lobby_RetryNameCb(uint8_t* s, int32_t flag, int32_t a, int32_t) {
    const int32_t n = ++g_6cd984;
    if (n >= 0x19) {
        g_6cd984 = 0;
        g_6cd974 = 0;
        ((void(__fastcall*)(uint8_t*))0x58e2e0)(s);
        return;
    }
    if (n == 1) Lobby_SetMode((flag != 0) + 5, a, 0, 0);
    g_lcg = g_lcg * 0x19660d + 0x3c6ef35f;
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)0x6ae0c0, (const char*)0x5ce1e4, (int32_t)(g_lcg & 0x7fff) % 1000);
    char name[0x100];
    ((void(__fastcall*)(char*, const char*))0x58e650)(name, (const char*)0x6ae0c0);
    NameDialog_SetNameIfOpen(name);
    ((void(__fastcall*)(uint8_t*, const char*))0x58d7a0)(s, name);
}
RT3_REPLACE(0x46a830, Lobby_RetryNameCb);

// 0x46a900. Logs lobby onto GameSpy: creates the peer (0x6cd970) with the lobby
// callbacks, sets the RT3 title (rooms pinged: title + group, cross-pinged:
// staging), connects as nick (or "RT3Player<n>", n from the shared LCG 0x624324)
// and joins the title room, then asks for the game list. When already connected
// (0x6cd920) it only marks the connect done (0x6cd974). 1 on success.
RT3_NOINLINE int32_t __fastcall Lobby_Connect(uint8_t* lobby, int, const char* nick) {
    uint32_t cb[0x1c];
    memset(cb, 0, sizeof cb);
    g_6cd978 = 0;
    F<int32_t>(lobby, 0x9874) = 1;
    cb[0] = 0x468de0;
    cb[1] = 0x468e00;
    cb[9] = 0x468ec0;
    cb[10] = 0x468f50;
    cb[13] = 0x469010;
    cb[16] = 0x469030;
    cb[17] = 0x469060;
    cb[20] = 0x46a6c0;
    cb[21] = 0x469070;
    cb[22] = 0x4690d0;
    cb[23] = 0x4690f0;
    cb[24] = 0x469160;
    cb[25] = 0x469180;
    cb[27] = 0;
    g_6cd974 = 0;
    void* peer = ((void*(__fastcall*)(uint32_t*))0x58dc50)(cb);
    g_6cd970 = peer;
    if (!peer) return 0;
    int32_t cross[3] = { 0, 0, 1 }, ping[3] = { 1, 1, 0 };
    const char* title = g_5f2b44;
    const char* secret = g_5f2b48;
    int32_t ok = ((int32_t(__fastcall*)(void*, const char*, const char*, const char*, const char*, int32_t, int32_t, int32_t, int32_t*, int32_t*))0x58dce0)(
        peer, title, secret, title, secret, 0, 0x1e, 0, ping, cross);
    if (g_6cd920) {
        g_6cd974 = 1;
        return 1;
    }
    if (!ok) return 0;
    char name[0x100];
    if (nick && *nick) strcpy(name, nick);
    else {
        uint32_t r = g_lcg * 0x19660d + 0x3c6ef35f;
        g_lcg = r;
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(name, (const char*)0x5ce1e4, (int32_t)(r & 0x7fff) % 1000);
    }
    char key[0x100];
    ((void(__fastcall*)(char*, const char*))0x58e650)(key, name);
    ((void(__fastcall*)(void*, const char*, int32_t, uint32_t, uint32_t, int32_t, int32_t))0x58e200)(g_6cd970, key, 0, 0x46a830,
                                                                                                    0x469190, 0, 1);
    if (!g_6cd974) return 0;
    g_6cd978 = 0;
    ((void(__fastcall*)(void*, int32_t, uint32_t, int32_t, int32_t))0x58dfb0)(g_6cd970, 0, 0x469410, 0, 1);
    if (!g_6cd978) return 0;
    Lobby_UpdateList(lobby);
    return 1;
}
RT3_REPLACE(0x46a900, Lobby_Connect);

// 0x46ab10. New empty 20-byte list header. QUIRK: writes through null on failure.
RT3_NOINLINE void* __stdcall NewListHeader(int32_t) {
    uint32_t* p = (uint32_t*)ExeMalloc53b(0x14);
    if (p) {
        p[2] = 0;
        p[3] = 0;
        p[4] = 0;
    }
    p[0] = 0;
    return p;
}
RT3_REPLACE(0x46ab10, NewListHeader);

// 0x46ab40. Queues message `msg` for `key` in ctx's map +0x2580 (new list
// headers via 0x46ab10); tracks the highest 'g' sequence in +0x2584.
// 1 if queued (or no world), 0 for 'n' messages and key 0.
RT3_NOINLINE int32_t __fastcall Msg_Queue(void*, int, int32_t key, uint32_t* msg, uint8_t* ctx) {
    if (!hook::As<void*>(g_world)) return 1;
    if (F<uint8_t>((void*)msg[1], 9) == 0x6e) return 0;
    if (!key) return 0;
    void** h = ((void**(__thiscall*)(void*, int32_t))0x53dae0)(F<void*>(ctx, 0x2580), key);
    if (F<uint8_t>((void*)msg[1], 9) == 0x67) {
        uint32_t seq = F<uint32_t>((void*)msg[2], 4);
        if (seq > F<uint32_t>(ctx, 0x2584)) F<uint32_t>(ctx, 0x2584) = seq;
    }
    if (h) {
        SList_InsertAfter(h + 2, 0, (void**)h[3], (void**)msg);
        return 1;
    }
    uint32_t* l = (uint32_t*)NewListHeader(key);
    msg[0] = l[2];
    l[2] = (uint32_t)msg;
    l[4]++;
    if (!l[3]) l[3] = (uint32_t)msg;
    ((void(__thiscall*)(void*, int32_t, void*))0x53d960)(F<void*>(ctx, 0x2580), key, l);
    return 1;
}
RT3_REPLACE(0x46ab40, Msg_Queue);

// 0x46ac00. Hands message msg to the handler registered for its kind (hash +4;
// none: dropped) with arg. A packed message (header +0xb bit 1) is unpacked
// first and its payload / size restored afterwards (dropped when unpacking
// fails). A multi-part one (bit 2) goes as an unpacked copy through Msg_Queue,
// keyed by its first payload word, and is handled (key kept in +0x9888) and freed
// only when that does not queue it.
RT3_NOINLINE void __fastcall Net_Dispatch(uint8_t* s, int, uint8_t* msg, uint32_t arg) {
    typedef void(__fastcall * Handler)(uint8_t*, uint32_t);
    Handler h = (Handler)(uintptr_t)Hash_Get(F<HashMap*>(s, 4), 0, Net_MsgKind(msg));
    if (!h) return;
    uint8_t flags = F<uint8_t*>(msg, 4)[0xb];
    if (flags & 1) {
        int32_t savedSize = F<int32_t>(msg, 0xc);
        uint8_t* out = nullptr;
        int32_t outSize = 0;
        int32_t packer[3] = { 0, 0, 0 };
        if (Pack_Decompress(packer, 0, F<const int32_t*>(msg, 8), 0, &out, &outSize) == 1) {
            F<uint8_t*>(msg, 8) = out;
            F<int32_t>(msg, 0xc) = outSize;
            if (F<uint8_t*>(msg, 4)[0xb] & 2) {
                int32_t key = *(int32_t*)out;
                uint8_t* copy = NetPacket_Copy(s, 0, msg);
                F<uint8_t*>(copy, 4)[0xb] &= ~1;
                F<uint8_t*>(copy, 4)[0xb] &= ~2;
                if (!Msg_Queue(s, 0, key, (uint32_t*)copy, (uint8_t*)Engine_FirstListEntry())) {
                    F<int32_t>(s, 0x9888) = key;
                    h(copy, arg);
                    NetPacket_Free(s, 0, copy);
                }
            } else {
                h(msg, arg);
            }
            uint8_t* hdr = F<uint8_t*>(msg, 4);
            F<uint8_t*>(msg, 8) = hdr + hdr[0xa];
            F<int32_t>(msg, 0xc) = savedSize;
            ((void(__cdecl*)(void*))0x5a1145)(out);
        }
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
        return;
    }
    if (flags & 2) {
        int32_t key = *F<int32_t*>(msg, 8);
        uint8_t* copy = NetPacket_Copy(s, 0, msg);
        F<uint8_t*>(copy, 4)[0xb] &= ~2;
        if (!Msg_Queue(s, 0, key, (uint32_t*)copy, (uint8_t*)Engine_FirstListEntry())) {
            F<int32_t>(s, 0x9888) = key;
            h(copy, arg);
            NetPacket_Free(s, 0, copy);
        }
        return;
    }
    h(msg, arg);
}
RT3_REPLACE(0x46ac00, Net_Dispatch);

// 0x46ad80. Handles the messages received on every channel (counted in +0x988c):
// ones with their own handler (+0x10) go to the session's raw handler (+8);
// sequenced ones (header +4) are copied into a packet buffer, marked (+0x20 bit
// 0, frame +0x24) and queued in the engine heap (+0x6c) by sequence; the rest
// are dispatched with their sender's record (looked up once per sender, +0x18).
// Messages without a kind (header +9) are skipped. Then the resync transfers are
// fed.
RT3_NOINLINE void __fastcall Net_ProcessIncoming(uint8_t* s, int, uint32_t frame) {
    F<int32_t>(s, 0x988c) = 0;
    uint32_t* list = Net_TakeChannelMessages(0xff);
    if (list) {
        uint8_t* from = nullptr;
        uint8_t* e = (uint8_t*)Engine_FirstListEntry();
        for (uint8_t* m = (uint8_t*)(uintptr_t)list[0]; m; m = *(uint8_t**)m) {
            ++F<int32_t>(s, 0x988c);
            if (F<int32_t>(m, 0x10)) {
                ((void(__fastcall*)(uint8_t*, int))F<void*>(s, 8))(m, 0);
                continue;
            }
            if (!from || F<uint32_t>(from, 4) != F<uint32_t>(m, 0x18)) from = Textures_Lookup(m, 0);
            uint8_t* hdr = F<uint8_t*>(m, 4);
            if (!hdr[9]) continue;
            if (!F<int32_t>(hdr, 4)) {
                Net_Dispatch(s, 0, m, (uint32_t)(uintptr_t)from);
                continue;
            }
            uint32_t len = (uint32_t)hdr[0xa] + F<uint32_t>(m, 0xc);
            uint8_t* copy = (uint8_t*)NetPacket_Alloc(s, 0, (int32_t)len);
            memcpy(copy, F<uint8_t*>(m, 4), len);
            F<uint8_t*>(m, 4) = copy;
            if (len != copy[0xa]) F<uint8_t*>(m, 8) = copy + copy[0xa];
            F<uint32_t>(m, 0x20) |= 1;
            F<uint32_t>(m, 0x24) = frame;
            Heap_Push(F<MaxHeap*>(e, 0x6c), 0, F<int32_t>(copy, 4), (int32_t)(uintptr_t)m);
        }
        Net_FinishReceived(g_6d40d0, 0, list);
    }
    Net_FeedResync((uint8_t*)(uintptr_t)frame);
}
RT3_REPLACE(0x46ad80, Net_ProcessIncoming);

// 0x46aeb0. Dispatches the first engine list entry's queued messages (heap +0x6c,
// count +0x70) while the heap's top key equals the count: each is popped, handed
// to Net_Dispatch with its sender's record, its packet freed and the message kept
// on a local list that is cleared at the end.
RT3_NOINLINE void __fastcall Net_DrainQueue(uint8_t* s, int, int32_t) {
    void* list[3] = { nullptr, nullptr, nullptr };
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    if (e) {
        for (int32_t top = Heap_TopKey(F<MaxHeap*>(e, 0x6c)); top; top = Heap_TopKey(F<MaxHeap*>(e, 0x6c))) {
            if (top != F<int32_t>(e, 0x70)) break;
            uint8_t* m = (uint8_t*)(uintptr_t)Heap_PopMax(F<MaxHeap*>(e, 0x6c));
            --F<int32_t>(e, 0x70);
            uint8_t* from = Textures_Lookup(m, 0);
            Net_Dispatch(s, 0, m, (uint32_t)(uintptr_t)from);
            NetPacket_Free(s, 0, F<uint8_t*>(m, 4));
            *(void**)m = list[0];
            list[0] = m;
            list[2] = (void*)((uintptr_t)list[2] + 1);
            if (!list[1]) list[1] = m;
        }
    }
    if (list[0]) CmdQueue_Clear((uint8_t*)list);
}
RT3_REPLACE(0x46aeb0, Net_DrainQueue);

// 0x46af70. The lobby's periodic work at time now (ms): expires the 1 s hold
// +0x9054, refreshes the queued textures every 5 s (in the lobby or when +0x8f30),
// and every second either advertises the hosted game (host, +0x8e10) or, while a
// server list is being shown (+0x8f18), copies the list entries seen in the last
// 10 s (0x11c bytes each, up to 0x80) into it and purges old messages.
RT3_NOINLINE void __fastcall Lobby_Tick(uint8_t* l, int, uint32_t now) {
    if (F<uint32_t>(l, 0x9054) && now - F<uint32_t>(l, 0x9054) > 1000) F<uint32_t>(l, 0x9054) = 0;
    if (!hook::As<void*>(g_world) || F<int32_t>(l, 0x8f30) == 1) {
        if (now - F<uint32_t>(l, 0x8f14) > 5000) {
            F<uint32_t>(l, 0x8f14) = now;
            Textures_RefreshQueued((int32_t)now);
        }
    }
    void* w = hook::As<void*>(g_world);
    if (g_6d40dc == 1) {
        if (!w && now - F<uint32_t>(l, 0x8f10) > 1000) {
            F<uint32_t>(l, 0x8f10) = now;
            Net_BroadcastGame(g_6d40d0, 0, (const char*)l + 0x8e10);
        }
        return;
    }
    if (F<int32_t>(l, 0x8f18) != 1 || w || F<int32_t>(l, 0x985c)) return;
    if (now - F<uint32_t>(l, 0x8f10) <= 1000) return;
    F<uint32_t>(l, 0x8f10) = now;
    memset(l + 0x10, 0, 0x2380 * 4);
    uint8_t** lst = (uint8_t**)(uintptr_t)Net_LockList2();
    if (lst) {
        uint8_t* dst = l + 0x10;
        int32_t k = 0;
        for (uint8_t* n = *lst; n && k < 0x80; n = F<uint8_t*>(n, 0), ++k, dst += 0x11c) {
            if (now - F<uint32_t>(n, 0x118) < 10000) memcpy(dst, n, 0x47 * 4);
            else F<int32_t>(n, 0x118) = -1;
        }
    }
    Net_UnlockList2();
    Net_PurgeSentMessages(0);
}
RT3_REPLACE(0x46af70, Lobby_Tick);

// 0x46b0c0. The host starts the game once (+0x8f30) a world exists and every
// connected player is ready (queue nodes flagged 8 in +0x5c have their +0x58
// cleared and don't count): each player record (0x6ceb9c, with a slot +4 and
// +0x1e1 set; the first takes the engine's own name) goes out as a 0x88-byte
// type 0x56 message {slot index +4, name +8}, packed when that works, then the
// empty start message 0xa. Until then the tooltip lists who is still missing.
RT3_NOINLINE void __fastcall Net_HostStart(uint8_t* s) {
    typedef LONG(WINAPI * LockFn)(LONG volatile*);
    if (F<int32_t>(s, 0x8f30) || !hook::As<void*>(g_world)) return;
    bool ready = false;
    if (g_6d40dc) {
        int32_t waiting = Global6d4100();
        uint8_t* q = (uint8_t*)(uintptr_t)Net_LockQueue();
        for (uint8_t* n = *(uint8_t**)q; n; n = *(uint8_t**)n)
            if (n[0x5c] & 8) {
                F<int32_t>(n, 0x58) = 0;
                --waiting;
            }
        Net_UnlockQueue();
        ready = waiting == 0;
    }
    if (!ready) {
        if (uint8_t* ui = g_windowManager) Ui_ShowTooltip(ui, 0, Net_WaitingPlayersText(), 3, 0, -1);
        return;
    }
    SendToFn send = (SendToFn)&Net_SendTo;
    Collection* players = g_players;
    for (int32_t i = 0; i < Collection_Count(players); ++i) {
        uint8_t* p = (uint8_t*)Collection_Get(players, 0, Collection_IdAt(players, 0, (uint32_t)i, 0));
        if (!F<int32_t>(p, 4) || !F<int32_t>(p, 0x1e1)) continue;
        if (i == 0) strcpy((char*)p + 8, (const char*)Engine_FirstListEntry() + 8);
        uint8_t msg[0x88];
        memset(msg, 0, sizeof msg);
        F<int32_t>(msg, 4) = i;
        strcpy((char*)msg + 8, (const char*)p + 8);
        while (*(volatile LONG*)0x6cd960) {
        }
        hook::As<LockFn>(g_5c8070)((LONG volatile*)0x6cd960);
        g_6cd930 += 0x88;
        uint8_t* out = nullptr;
        int32_t outSize = 0;
        int32_t packer[3] = { 0x91, 0, 0 };
        if (Pack_Compress(packer, 0, msg, 0x88, &out, &outSize) == 1) {
            g_6cd934 += outSize;
            send(g_6d40d0, 0, out, (uint32_t)outSize, 0x56, 0xff, nullptr, 0x80000028, 0, 0, 0);
            if (out) ((void(__cdecl*)(void*))0x5a1145)(out);
        } else {
            g_6cd934 += 0x88;
            send(g_6d40d0, 0, msg, 0x88, 0x56, 0xff, nullptr, 0x28, 0, 0, 0);
        }
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
        hook::As<LockFn>(g_5c8074)((LONG volatile*)0x6cd960);
    }
    F<int32_t>(s, 0x8f30) = 1;
    while (*(volatile LONG*)0x6cd960) {
    }
    hook::As<LockFn>(g_5c8070)((LONG volatile*)0x6cd960);
    send(g_6d40d0, 0, nullptr, 0, 0xa, 0xff, nullptr, 8, 0, 0, 0);
    hook::As<LockFn>(g_5c8074)((LONG volatile*)0x6cd960);
}
RT3_REPLACE(0x46b0c0, Net_HostStart);

// 0x46b360. The weekly (world day byte +0x11 % 7 == 1) state check of a started
// game (+0x8f30, 0x6cd8ec): the host packs its status (Obj_BuildStatusPacket,
// timed into 0x6ae4cc when 0x6cd900) and sends it as message 0xd. A client
// collects its own 9-byte entries from the status sources (+0x8f28, cleared +0xc)
// and compares them with the host's (the flagged player's table, +0x178 entries
// from +0x180 in block +0x17c): each slot with both checksums set is marked in
// 0x6cd908; a mismatch flags the slot's source (+0x98) and, unless reporting is
// muted (0x6cd91c without 0x6cd918), goes straight back as message 0x4f. Both
// tables are then pruned to the date.
// QUIRK: a slot without a matching source clears +0x98 of a null pointer.
void __fastcall Net_WeeklyCheck(uint8_t* s) {
    typedef LONG(WINAPI * LockFn)(LONG volatile*);
    g_6cd908 = 0;
    uint8_t* world = g_world;
    if (!world || F<int32_t>(s, 0x8f30) != 1 || g_6cd8ec != 1) return;
    if ((int32_t)world[0x11] % 7 != 1) return;
    uint32_t date = (uint32_t)DateToMinutes((const GameDate*)(world + 0xd));
    bool timed = g_6cd900 == 1;
    int32_t t0 = 0;
    if (g_6d40dc) {
        if (Global6d4100() <= 1) return;
        if (timed) t0 = ClockMs();
        int32_t size = 0;
        uint8_t* pkt = Obj_BuildStatusPacket(s, 0, &size);
        if (g_6cd900 == 1) g_6ae4cc = ClockMs() - t0;
        Net_Send(s, 0, nullptr, 0xd, size, pkt, 2, 0);
        if (pkt) ExeDelete(pkt);
        g_6cd918 = 0;
        return;
    }
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    if (!e) return;
    if (timed) t0 = ClockMs();
    for (uint8_t* n = *F<uint8_t**>(s, 0x8f28); n; n = *(uint8_t**)n) {
        F<int32_t>(n, 0xc) = 0;
        if (const uint8_t* r = ((const uint8_t*(*)())F<void*>(n, 4))()) Table9_Append(e, r);
    }
    if (g_6cd900 == 1) g_6ae4cc = ClockMs() - t0;
    uint8_t* q = Net_FindFlaggedPlayer();
    const uint8_t* rec = q + F<int32_t>(q, 0x17c) * 0x1200 + 0x180;
    uint8_t* own = (uint8_t*)Engine_FirstListEntry();
    for (int32_t i = 0; i < F<int32_t>(q, 0x178); ++i, rec += 9) {
        uint8_t* p = Table9_Find(own, rec);
        if (!p) continue;
        F<uint32_t>(s, 0x8f44) = F<uint32_t>((void*)rec, 5);
        volatile uint8_t* src = nullptr;
        if (void** list = F<void**>(s, 0x8f28)) {
            for (uint8_t* n = (uint8_t*)*list; n; n = *(uint8_t**)n)
                if (F<uint32_t>(n, 4) == Obj_FindNode8f28(s, 0, p[0])) {
                    src = n;
                    break;
                }
        }
        *(volatile int32_t*)(src + 0x98) = 0;
        if (!F<int32_t>(p, 1) || !F<int32_t>((void*)rec, 1)) continue;
        g_6cd908 |= 1u << (p[0] & 0x1f);
        if (F<int32_t>(p, 1) == F<int32_t>((void*)rec, 1)) continue;
        *(volatile int32_t*)(src + 0x98) = 1;
        if (g_6cd91c && g_6cd918 != 1) continue;
        while (*(volatile LONG*)0x6cd960) {
        }
        hook::As<LockFn>(g_5c8070)((LONG volatile*)0x6cd960);
        g_6cd930 += 9;
        g_6cd934 += 9;
        ((SendToFn)&Net_SendTo)(g_6d40d0, 0, p, 9, 0x4f, 0xff, (const char*)q + 8, 8, 0, 0, 0);
        hook::As<LockFn>(g_5c8074)((LONG volatile*)0x6cd960);
        if (g_5f2b3c) g_5f2b3c = 0;
        if (g_6cd918 == 1) g_6cd91c = 1;
    }
    if (g_6cd908) g_6cd918 = 0;
    Table9_Prune(own, date);
    Table9_Prune(Textures_Find(F<int32_t>(q, 4)), date);
    Net_UnlockQueueArg(F<int32_t>(q, 4));
}
RT3_REPLACE(0x46b360, Net_WeeklyCheck);

// 0x46b6d0. The session's message date (+0x9048); with count set it also counts
// one more part for that date in the current frame entry's hash (creating its list
// header), under the entry's lock. Null without a running world.
RT3_NOINLINE uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count) {
    uint8_t* w = g_world;
    if (!w || !F<int32_t>(w, 0x19)) return nullptr;
    if (count) {
        int32_t key = DateToMinutes((const GameDate*)(s + 0x9048));
        uint8_t* e = Textures_Find(Global6d40e4());
        if (e) {
            int32_t* h = (int32_t*)(uintptr_t)Hash_Get(F<HashMap*>(e, 0x2580), 0, (uint32_t)key);
            if (!h) {
                h = (int32_t*)NewListHeader(key);
                Hash_Insert(F<HashMap*>(e, 0x2580), 0, (uint32_t)key, (int32_t)(uintptr_t)h);
            }
            ++*h;
            Net_UnlockQueueArg(Global6d40e4());
        }
    }
    return s + 0x9048;
}
RT3_REPLACE(0x46b6d0, Net_CountMessagePart);

// 0x46b780. Network session s, once per update: shows a pending status line
// (0xf27, 3 s) and, while connected (0x6cd8dc), either drops a lost game - a
// world whose peer link 0x6d40d0 reports nothing (unless 0x6d4114): the view
// returns to the start (0x6d0818) and input event 0xcc goes out; 0 then - or
// runs the lobby / host / incoming / outgoing queues stamped with the
// clock (+0x9058, the step time into 0x6ae4d0 when timing, 0x6cd900 == 1),
// shows messages and, in a running game, drains the queue; a world awaited
// by 0x6ce9c4 is built here and loaded from 0x6ce630. Finally the voice chat
// (0x6cd970 / 0x6cd974) ticks. 1 otherwise.
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s) {
    if (g_6cd91c) StatusLine_Set(g_windowManager, 0, GetString(0xf27), 0xbb8);
    if (g_6cd8dc) {
        if (g_world && !(g_6d40d0 && Global6d4100())) {
            if (!g_6d4114) {
                if (uint8_t* v = g_gameWindow) ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ddbd0)(v, 0, 0, -1);
                Game_PostInputEvent(0xcc, 0, 2, 0, 0);
            }
            return 0;
        }
        if (!g_6ce9b8) {
            F<uint32_t>(s, 0x9058) = (uint32_t)ClockMs();
            Lobby_Tick(s, 0, F<uint32_t>(s, 0x9058));
            Net_HostStart(s);
            Net_ProcessIncoming(s, 0, F<uint32_t>(s, 0x9058));
            Net_DrainQueue(s, 0, F<int32_t>(s, 0x9058));
            if (g_6cd900 == 1) g_6ae4d0 = ClockMs() - F<int32_t>(s, 0x9058);
            NetMessage_ShowNext(s);
            if (uint8_t* w = g_world)
                if (!F<int32_t>(w, 0x19)) NetQueue_Drain(s);
            if (g_6ce9c4) {
                g_6ce9c4 = 0;
                uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x46c40);
                uint8_t* w = mem ? (uint8_t*)World_Construct(mem) : nullptr;
                g_world = w;
                ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x438890)(w, 0, 0, 0);
                F<int32_t>(g_app, 0x6c) = 0;
                ((void(__fastcall*)(uint8_t*, int, const char*, int32_t, int32_t))0x445ac0)(g_world, 0, (const char*)0x6ce630,
                                                                                        g_6ce9c0, 0);
            }
        }
    }
    if (hook::As<uint8_t*>(g_6cd970) && g_6cd974 && g_6cd8dc)
        ((void(__fastcall*)(uint8_t*))0x58def0)(hook::As<uint8_t*>(g_6cd970));
    return 1;
}
RT3_REPLACE(0x46b780, Session_Update);

// 0x46b910. Drops entry e's stored message lists older than the given minute
// stamp (the current date when 0): for each key the visitor 0x468ac0 selects
// (into 0x6cd968) the list is unhooked and its packets freed.
RT3_NOINLINE void __fastcall Net_DropOldMessages(uint8_t* s, int, uint8_t* e, int32_t minutes) {
    typedef int32_t(__fastcall * ForEachFn)(HashMap*, int, uint32_t, void*, void*);
    int32_t stamp = 0;
    if (!minutes && g_world) stamp = DateToMinutes((const GameDate*)(g_world + 0xd));
    ForEachFn forEach = (ForEachFn)0x53db20;
    while (forEach(F<HashMap*>(e, 0x2580), 0, 0, (void*)0x468ac0, &stamp)) {
        uint8_t* n = (uint8_t*)(uintptr_t)Hash_Remove(F<HashMap*>(e, 0x2580), 0, g_6cd968);
        if (!n) continue;
        for (;;) {
            uint8_t* p = F<uint8_t*>(n, 8);
            if (!p) break;
            F<uint8_t*>(n, 8) = F<uint8_t*>(p, 0);
            if (p == F<uint8_t*>(n, 0xc)) F<uint8_t*>(n, 0xc) = F<uint8_t*>(p, 0);
            --F<int32_t>(n, 0x10);
            NetPacket_Free(s, 0, p);
        }
        ExeDelete(n);
    }
}
RT3_REPLACE(0x46b910, Net_DropOldMessages);

// 0x46b9f0. Network session s runs this frame's messages: the session's
// clock (+0x9048) becomes tomorrow's date at the world's time, old
// messages are dropped and queues drained, and the batch due now (by
// minute, from the engine entry's hash +0x2580) is dispatched in order
// with the tools paused (+0x9890 counts them, non-'g' unflagged ones count
// as changes). The host then sends message 0x67: {+0x9880, minute, seed,
// changes + +0x987c}. With timing on (0x6cd900) the time goes to 0x6ae4d4.
RT3_NOINLINE void __fastcall Net_RunFrameMessages(uint8_t* s) {
    uint8_t changes = 0;
    int32_t started = 0;
    if (g_6cd900 == 1) started = ClockMs();
    F<int32_t>(s, 0x9890) = 0;
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    uint8_t* w = g_world;
    if (e && w) {
        F<uint32_t>(s, 0x9048) = F<uint32_t>(w, 0xd);
        F<uint32_t>(s, 0x904c) = F<uint32_t>(w, 0x11);
        GameDate d = F<GameDate>(w, 0xd);
        F<uint64_t>(s, 0x9048) = DateAddDays(1, d);
        s[0x904d] = 0, s[0x904e] = 0, s[0x904f] = 0;
        const int32_t minute = DateToMinutes((const GameDate*)(g_world + 0xd));
        Net_DropOldMessages(s, 0, e, 0);
        NetQueue_Drain(s);
        Buildings_ApplyQueuedMoves();
        F<int32_t>(s, 0x987c) = 0;
        F<int32_t>(s, 0x9880) = 0;
        F<int32_t>(e, 0x2588) = 0;
        if (Hash_Contains(F<HashMap*>(e, 0x2580), 0, (uint32_t)minute)) {
            if (uint8_t* q = (uint8_t*)(uintptr_t)Hash_Remove(F<HashMap*>(e, 0x2580), 0, (uint32_t)minute)) {
                if (g_6d40dc) Obj_CountItems(s, 0, q);
                Game_PauseTools();
                uint32_t count = 0;
                auto pop = [q]() -> uint8_t* {
                    uint8_t* node = F<uint8_t*>(q, 8);
                    if (!node) return nullptr;
                    if (node == F<uint8_t*>(q, 0xc)) F<uint8_t*>(q, 0xc) = F<uint8_t*>(node, 0);
                    F<uint8_t*>(q, 8) = F<uint8_t*>(node, 0);
                    --F<int32_t>(q, 0x10);
                    return node;
                };
                for (uint8_t* node = pop(); node; node = pop()) {
                    ++F<int32_t>(s, 0x9890);
                    uint8_t* rec = F<uint8_t*>(node, 4);
                    if (rec[9] != 'g' && !(rec[0xb] & 4)) ++count;
                    uint8_t* r = Textures_Lookup(node, 0);
                    Net_Dispatch(s, 0, node, (uint32_t)(uintptr_t)r);
                    NetPacket_Free(s, 0, node);
                }
                changes = (uint8_t)((uint8_t)F<int32_t>(s, 0x987c) + (uint8_t)count);
                ((void(__cdecl*)(void*))0x53b080)(q);
                Options_Restore();
            }
        }
        if (g_6d40dc) {
            uint32_t msg[4];
            msg[0] = F<uint32_t>(s, 0x9880);
            F<int32_t>(s, 0x8f38) = minute;
            msg[1] = (uint32_t)minute;
            msg[3] = changes;
            msg[2] = (uint32_t)GetSeed();
            hook::As<uint32_t>(g_5f2b6c) = msg[2];
            void* out = nullptr;
            const int32_t size = Finance_SyncMessage((uint8_t*)msg, 1, &out);
            Net_Send(g_session, 0, nullptr, 0x67, size, (const uint8_t*)out, 0x13, 0);
            if (out != msg) ((void(__cdecl*)(void*))0x5a1145)(out);
        }
    }
    if (g_6cd900 == 1) g_6ae4d4 = ClockMs() - started;
}
RT3_REPLACE(0x46b9f0, Net_RunFrameMessages);

// 0x46bc40. Closes the network session s (when open, +0): drops its pack object
// (+0x9884), stops hosting (+0x8f18), cleans up the first player's messages,
// shuts the peer, the shared state and the music stream down, returns every
// queued packet (+0x8f2c) to the pool and frees the session's tables, pools and
// buffers, then the global stream 0x6cd8e0.
RT3_NOINLINE void __fastcall Session_Close(uint8_t* s) {
    g_6cd91c = 0;
    if (!F<int32_t>(s, 0)) return;
    CloseFile6ce994();
    if (void* o = F<void*>(s, 0x9884)) {
        Pack_Release(g_pack, 0, o);
        F<void*>(s, 0x9884) = nullptr;
    }
    if (F<int32_t>(s, 0x8f18) == 1) Nop5217b0(hook::As<void*>(g_6d40d0));
    if (hook::As<void*>(g_6d40d0) && Net_LockQueue()) {
        if (Engine_FirstListEntry()) Net_DropOldMessages(s, 0, (uint8_t*)Engine_FirstListEntry(), 1);
        Net_UnlockQueue();
    }
    Peer_Shutdown();
    Net_ShutdownShared(g_6d40d0);
    Music_Shutdown(F<uint8_t*>(s, 0));
    if (uint8_t* q = F<uint8_t*>(s, 0x8f2c)) {
        for (;;) {
            uint8_t* n = F<uint8_t*>(q, 0);
            if (!n) break;
            F<uint8_t*>(q, 0) = F<uint8_t*>(n, 0);
            if (n == F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = F<uint8_t*>(n, 0);
            --F<int32_t>(q, 8);
            NetPacket_Free(s, 0, n);
            q = F<uint8_t*>(s, 0x8f2c);
        }
    }
    if (void* p = F<void*>(s, 0x9878)) {
        ExeDelete(p);
        F<void*>(s, 0x9878) = nullptr;
    }
    NodePool* pool = F<NodePool*>(s, 0x8f20);
    F<int32_t>(s, 0x9054) = 0;
    F<int32_t>(s, 0) = 0;
    F<int32_t>(s, 0xc) = 0;
    F<int32_t>(s, 0x8f18) = 0;
    F<int32_t>(s, 0x8f30) = 0;
    if (pool) {
        Pool_Free(pool);
        ExeDelete(pool);
        F<void*>(s, 0x8f20) = nullptr;
    }
    if (void* p = F<void*>(s, 0x8f24)) {
        ((void(__cdecl*)(void*))0x5a1145)(p);
        F<void*>(s, 0x8f24) = nullptr;
    }
    if (HashMap* m = F<HashMap*>(s, 0x9050)) Hash_ForEach(m, 0, 0, (HashVisit)0x468ae0, nullptr);
    if (HashMap* m = F<HashMap*>(s, 0x9050)) {
        Hash_DestroyThunk(m);
        ExeDelete(m);
        F<void*>(s, 0x9050) = nullptr;
    }
    if (NodePool* p = F<NodePool*>(s, 0x8f3c)) {
        Pool_Free(p);
        ExeDelete(p);
        F<void*>(s, 0x8f3c) = nullptr;
    }
    static const uint32_t bufs[] = { 0x8f40, 0x8f28, 0x8f2c };
    for (uint32_t off : bufs)
        if (void* p = F<void*>(s, off)) {
            ExeDelete(p);
            F<void*>(s, off) = nullptr;
        }
    if (HashMap* m = F<HashMap*>(s, 4)) {
        Hash_DestroyThunk(m);
        ExeDelete(m);
        F<void*>(s, 4) = nullptr;
    }
    if (uint8_t* m = g_6cd8e0) {
        Music_Close(m);
        ExeDelete(m);
        g_6cd8e0 = nullptr;
    }
}
RT3_REPLACE(0x46bc40, Session_Close);

// 0x46be80. Opens network session s (closing any open one): resets the session
// globals, creates the music stream 0xd973b4 (kept at +0) and the shared network
// object 0x6d40d0 (game GUID 0x6257e8 from 0x5f2b28), stores the game version as
// 0x6d4108, and allocates the session's tables, pools and packet buffers. With
// gamespy == 1 it also logs onto the GameSpy lobby as nick (result at +0x985c).
// 1 when open, 0 when a step fails.
RT3_NOINLINE int32_t __fastcall Session_Open(uint8_t* s, int, int32_t gamespy, const char* nick) {
    Session_Close(s);
    if (F<int32_t>(s, 0)) return 0;
    g_6ae4cc = 0;
    g_6ae4d0 = 0;
    g_6ae4d4 = 0;
    g_6cd96c = 0;
    g_6d10a0 = 0;
    g_622090 = -1;
    g_622094 = -1;
    static const uint32_t zeros[] = { 0x6cd92c, 0x6cd928, 0x6cd924, 0x6cd91c, 0x6cd8e4, 0x6cd8e8 };
    for (uint32_t a : zeros) hook::Global<int32_t>(a) = 0;
    g_5f2b38 = 0x3c;
    static const uint32_t zeros2[] = { 0x6cd8f4, 0x6cd8f8, 0x6cd8fc, 0x6cd908, 0x6cd90c, 0x6cd918, 0x6cd8ec, 0x6cd8f0, 0x6cd900, 0x5f2b3c, 0x6cd904 };
    for (uint32_t a : zeros2) hook::Global<int32_t>(a) = 0;
    g_5f2b40 = 1;
    F<int32_t>(s, 0x988c) = 0;
    F<int32_t>(s, 0x9890) = 0;
    uint32_t* m = g_d973b4;
    if (!m) {
        m = (uint32_t*)ExeNew(0x18);
        if (m) {
            m[0] = 0;
            m[1] = 1;
            m[2] = m[3] = m[4] = m[5] = 0;
        }
        g_d973b4 = m;
    }
    F<uint32_t*>(s, 0) = m;
    for (int k = 0; k < 4; ++k) ((uint32_t*)0x6257e8)[k] = ((const uint32_t*)0x5f2b28)[k];
    if (Music_Start(F<uint8_t*>(s, 0)) != 1) return 0;
    uint8_t* net = g_6d40d0;
    if (!net) {
        net = (uint8_t*)ExeNew(0x25a);
        net = net ? Net_Construct(net) : nullptr;
        g_6d40d0 = net;
    }
    if (Net_InitShared(net) != 1) return 0;
    float ver = Game_VersionNumber();
    SetGlobal6d4108(*(int32_t*)&ver);
    uint32_t* h = NewZeroed(0x14, 0x14);
    F<uint32_t*>(s, 4) = h;
    Hash_Init((HashMap*)h, 0, 0x65, 0x100);
    memset(s + 0x10, 0, 0x2380 * 4);
    F<uint32_t*>(s, 0x8f2c) = NewZeroed(0xc, 0xc);
    F<void*>(s, 0x8f24) = ((void*(__cdecl*)(size_t))0x5a125d)(0x2900);
    uint32_t* pool = NewZeroed(0x1c, 0x1c);
    F<uint32_t*>(s, 0x8f20) = pool;
    Pool_Init((NodePool*)pool, 0, F<void*>(s, 0x8f24), 0xa4, 0x40, 0);
    F<uint32_t*>(s, 0x8f28) = NewZeroed(0xc, 0xc);
    F<void*>(s, 0x8f40) = ExeNew(0x48000);
    pool = NewZeroed(0x1c, 0x1c);
    F<uint32_t*>(s, 0x8f3c) = pool;
    Pool_Init((NodePool*)pool, 0, F<void*>(s, 0x8f40), 0x300, 0x180, 0);
    h = NewZeroed(0x14, 0x14);
    F<uint32_t*>(s, 0x9050) = h;
    Hash_Init((HashMap*)h, 0, 0x11, 0x64);
    memset(s + 0x8f48, 0, 0x100);
    for (int k = 0; k < 4; ++k) F<int32_t>(s, 0x9864 + k * 4) = -1;
    F<int32_t>(s, 0x9860) = -1;
    F<int32_t>(s, 0x8f34) = 0;
    F<int32_t>(s, 0x8f14) = 0;
    F<int32_t>(s, 0x8f30) = 0;
    F<int32_t>(s, 0x8f44) = 0;
    F<int32_t>(s, 0x9054) = 0;
    memset(s + 0x905c, 0, 0x800);
    F<uint32_t*>(s, 0x9878) = NewZeroed(0xc, 0xc);
    F<uint8_t*>(s, 0x9884) = Pack_AcquireFont(g_pack, 0, 1);
    MpSession_Reset();
    RegisterSyncChecks();
    if (gamespy != 1) return 1;
    F<int32_t>(s, 0x985c) = Lobby_Connect(s, 0, nick);
    return F<int32_t>(s, 0x985c) ? 1 : 0;
}
RT3_REPLACE(0x46be80, Session_Open);

// 0x46c230. Closes session s, then frees the shared network object 0x6d40d0 and
// the music player 0xd973b4.
RT3_NOINLINE void __fastcall Session_Shutdown(uint8_t* s) {
    Session_Close(s);
    if (uint8_t* n = g_6d40d0) {
        Net_DeleteLocks();
        ExeDelete(n);
        g_6d40d0 = nullptr;
    }
    if (uint8_t* m = hook::As<uint8_t*>(g_d973b4)) {
        Music_Destroy(m);
        ExeDelete(m);
        hook::As<uint8_t*>(g_d973b4) = nullptr;
    }
}
RT3_REPLACE(0x46c230, Session_Shutdown);

// 0x46c280. Detail level for value v (EDX) under setting 0x5f2b70.
RT3_NOINLINE int32_t __fastcall DetailLevelFor(int32_t, int32_t v) {
    int32_t s = hook::As<int32_t>(g_5f2b70);
    if (s < 1000) return v <= 1000 ? 0xff : 9;
    if (s < 2000) {
        if (v <= 1000) return 8;
        return v <= 3000 ? 0xff : 9;
    }
    return 0xff;
}
RT3_REPLACE(0x46c280, DetailLevelFor);

// 0x46c2c0. Whether the file can be opened (mode 0x5ce2c0).
RT3_NOINLINE int32_t __fastcall FileOpens(const char* path) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ce2c0);
    if (!f) return 0;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return 1;
}
RT3_REPLACE(0x46c2c0, FileOpens);

// 0x46c2f0. Takes the name in EAX.
__declspec(naked) void File_GetSize() {
    __asm {
        push eax
        call File_SizeImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46c2f0, File_GetSize);

// 0x46c330. First free slot of the table 0x6ce2e8 (10 x 0x5c bytes), or 10.
RT3_NOINLINE int32_t FirstFreeSlot6ce2e8() {
    int32_t i = 0;
    for (const uint8_t* p = (const uint8_t*)0x6ce2e8; p < (const uint8_t*)0x6ce680; p += 0x5c, ++i)
        if (*(const int32_t*)p == 0) return i;
    return 10;
}
RT3_REPLACE(0x46c330, FirstFreeSlot6ce2e8);

// 0x46c360. Adds a chat line: to the game log (kind 5) during a game, else to
// the lobby chat.
RT3_NOINLINE void __fastcall Chat_AddLine(const char* text) {
    if (!g_world) {
        Lobby_AddChatText(text);
        return;
    }
    Log_Add(g_messageLog, 0, text, 5, 1, 0);
}
RT3_REPLACE(0x46c360, Chat_AddLine);

// 0x46c390 (network handler). Sends the lobby (0x109 bytes) back to the
// sender (EDX) as message 2. Returns 1.
int32_t __fastcall NetMsg_SendLobby(void*, uint8_t* from) {
    if (uint8_t* lobby = g_newGame) Net_Send(g_session, 0, (const char*)(from + 8), 2, 0x109, lobby, 1, 0);
    return 1;
}
RT3_REPLACE(0x46c390, NetMsg_SendLobby);

// 0x46c3c0 (network handler). The game starts: staging is left (0x6cd90c,
// 0x6cd8e8 set, 0x6cd96c cleared); a pending restart (0x6ce9bc) clears the
// lobby's +0x97 and sets 0x6ce9c4. Returns 1.
int32_t NetMsg_GameStarted() {
    SetGlobal6cfeac();
    g_6cd90c = 1;
    g_6cd8e8 = 1;
    Peer_LeaveStaging();
    g_6cd96c = 0;
    if (g_6ce9bc) {
        g_newGame[0x97] = 0;
        g_6ce9c4 = 1;
    }
    return 1;
}
RT3_REPLACE(0x46c3c0, NetMsg_GameStarted);

// 0x46c420 (net handler). Game options from the host (msg +8): copies the
// shared option fields into the app (0x6cec74, at +0x70 from their packet
// offset), saves the options and refreshes the menu (0x6ada8c). Always 1.
int32_t __fastcall NetMsg_GameOptions(uint8_t* m) {
    static const uint16_t kFields[] = { 0x1c, 0x20, 0x24, 0x28, 0x8c, 0xa0, 0xb4, 0xb8, 0xc0, 0xc4, 0xc8, 0xcc, 0xd0, 0xd4, 0xd8, 0xdc, 0xf8,
                                        0xfc, 0x100, 0x104, 0x108, 0x10c, 0x110, 0x118, 0x143, 0x153, 0x157, 0x15b, 0x15f, 0x163, 0x16f, 0x173,
                                        0x17b, 0x1df, 0x1ef, 0x223, 0x227, 0x22b, 0x22f, 0x233, 0x237, 0x23b, 0x24f, 0x258 };
    uint8_t* d = F<uint8_t*>(m, 8);
    for (uint16_t off : kFields) F<uint32_t>(g_app, off + 0x70) = F<uint32_t>(d, off);
    Options_Save(g_app, 0, 0);
    if (hook::As<HMENU*>(g_6ada8c)) Menu_Attach(hook::As<HMENU*>(g_6ada8c));
    return 1;
}
RT3_REPLACE(0x46c420, NetMsg_GameOptions);

// 0x46c760 (vtable stub).
int32_t __fastcall Return1_46c760(void*) { return 0x1; }
RT3_REPLACE(0x46c760, Return1_46c760);

// 0x46c770 (network handler). Sends the first engine entry's +0x48 with the
// packet's d[1] to the sender (message 7). Returns 1.
int32_t __fastcall NetMsg_SendEngineValue(uint8_t* m, uint8_t* from) {
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    int32_t buf[2];
    buf[0] = F<int32_t>(e, 0x48);
    buf[1] = F<int32_t>(F<uint8_t*>(m, 8), 4);
    Net_Send(g_session, 0, (const char*)(from + 8), 7, 8, (const uint8_t*)buf, 0, 0);
    return 1;
}
RT3_REPLACE(0x46c770, NetMsg_SendEngineValue);

// 0x46c7c0 / 0x46c870 (network handlers). Texture d[1] takes the packet's
// time (+0x58, age +0x54) / its flag 1 (+0x5c) from the first packet byte;
// the queue entry is released. Return 1.
int32_t __fastcall NetMsg_TextureTime(uint8_t* m, const int32_t* d) {
    uint8_t* rec = F<uint8_t*>(m, 8);
    if (uint8_t* t = Textures_Find(d[1])) {
        F<int32_t>(t, 0x58) = F<int32_t>(rec, 4);
        F<int32_t>(t, 0x54) = ClockMs() - F<int32_t>(rec, 4);
        Net_UnlockQueueArg(d[1]);
    }
    return 1;
}
RT3_REPLACE(0x46c7c0, NetMsg_TextureTime);

// 0x46c810 (network handler). Stores the packet as camera bookmark 0 and
// moves there. Returns 1.
int32_t __fastcall NetMsg_CameraBookmark(uint8_t* m) {
    if (g_62be68) {
        Obj_SetEntry10e(g_62be68, 0, F<const uint32_t*>(m, 8), 0);
        Camera_RecallBookmark(g_62be68, 0, 0);
    }
    return 1;
}
RT3_REPLACE(0x46c810, NetMsg_CameraBookmark);

// 0x46c840 / 0x46cf10 (network handlers). Texture d[1] gets flag 8 / 4
// (+0x5c) and its queue entry is released. Return 1.
int32_t __fastcall NetMsg_TextureFlag8(void*, const int32_t* d) {
    if (uint8_t* t = Textures_Find(d[1])) {
        F<uint32_t>(t, 0x5c) |= 8;
        Net_UnlockQueueArg(d[1]);
    }
    return 1;
}
RT3_REPLACE(0x46c840, NetMsg_TextureFlag8);

int32_t __fastcall NetMsg_TextureFlag1(uint8_t* m, const int32_t* d) {
    if (uint8_t* t = Textures_Find(d[1])) {
        if (*F<uint8_t*>(m, 8)) F<uint32_t>(t, 0x5c) |= 1;
        else F<uint32_t>(t, 0x5c) &= ~1u;
        Net_UnlockQueueArg(d[1]);
    }
    return 1;
}
RT3_REPLACE(0x46c870, NetMsg_TextureFlag1);

// 0x46c8c0 (network handler). On the host (0x6cd8ec 1) or with 0x6cd918 set,
// queues the packet msg +8 into the sender's entry (0x521d60 by from +4; the
// entry stays locked until the unlock): a 0xff packet {0xff, n, id, n
// values} becomes n 9-byte records {1..n, value, id}, any other packet is
// queued as it is. Returns 1.
int32_t __fastcall NetMsg_QueueRecords(uint8_t* msg, uint8_t* from) {
    if (g_6cd8ec != 1 && !g_6cd918) return 1;
    const uint8_t* p = F<const uint8_t*>(msg, 8);
    if (p[0] == 0xff) {
        uint8_t rec[9];
        memcpy(rec + 5, p + 2, 4);
        uint8_t* q = Textures_Find(F<int32_t>(from, 4));
        const uint8_t* v = p + 6;
        for (uint8_t k = 0; k < p[1]; v += 4) {
            rec[0] = ++k;
            memcpy(rec + 1, v, 4);
            Table9_Append(q, rec);
        }
    } else {
        Table9_Append(Textures_Find(F<int32_t>(from, 4)), p);
    }
    Net_UnlockQueueArg(F<int32_t>(from, 4));
    return 1;
}
RT3_REPLACE(0x46c8c0, NetMsg_QueueRecords);

// 0x46c990 (network handler). Sends the first engine entry's counter +0x74
// to the sender (message 0xf) and counts it down. Returns 1.
int32_t __fastcall NetMsg_SendCounter(void*, uint8_t* from) {
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    int32_t v = F<int32_t>(e, 0x74);
    F<int32_t>(e, 0x74) = F<int32_t>(e, 0x74) - 1;
    Net_Send(g_session, 0, (const char*)(from + 8), 0xf, 4, (const uint8_t*)&v, 1, 0);
    return 1;
}
RT3_REPLACE(0x46c990, NetMsg_SendCounter);

// 0x46c9d0 (network handler). Takes the random seed from the packet.
int32_t __fastcall NetMsg_Seed(uint8_t* m) {
    SetSeed(*F<int32_t*>(m, 8));
    return 1;
}
RT3_REPLACE(0x46c9d0, NetMsg_Seed);

// 0x46c9e0 (vtable stub).
int32_t __fastcall Return1_46c9e0(void*) { return 0x1; }
RT3_REPLACE(0x46c9e0, Return1_46c9e0);

RT3_NETFWD(0x46c9f0, NetMsg_BuyStock, uint32_t*, Packet_BuyStock)

RT3_NETFWD(0x46ca10, NetMsg_SellStock, uint32_t*, Packet_SellStock)

RT3_NETFWD(0x46ca30, NetMsg_ChairmanResign, int32_t*, Chairman_Resign)

RT3_NETFWD(0x46ca50, NetMsg_IssueBond, uint32_t*, CompanyCmd_IssueBond)

RT3_NETFWD(0x46ca70, NetMsg_RepayBond, uint32_t*, CompanyCmd_RepayBond)

RT3_NETFWD(0x46ca90, NetMsg_Bankrupt, uint32_t*, CompanyCmd_Bankrupt)

RT3_NETFWD(0x46cab0, NetMsg_IssueShares, uint32_t*, CompanyCmd_IssueShares)

RT3_NETFWD(0x46cad0, NetMsg_BuyBack, uint32_t*, CompanyCmd_BuyBack)

RT3_NETFWD(0x46caf0, NetMsg_SetDividend, uint32_t*, CompanyCmd_SetDividend)

RT3_NETFWD(0x46cb10, NetMsg_BuyTerritory, uint32_t*, CompanyCmd_BuyTerritory)

RT3_NETFWD(0x46cb30, NetMsg_BuildingInvest, uint32_t*, BuildingPacket_Invest)

RT3_NETFWD(0x46cb50, NetMsg_BuildingRelease, uint32_t*, BuildingPacket_Release)

RT3_NETFWD(0x46cb70, NetMsg_BuildingTake, uint32_t*, BuildingPacket_Take)

RT3_NETFWD(0x46cb90, NetMsg_SellTrainPacket, uint8_t*, NetMsg_SellTrain)

RT3_NETFWD(0x46cbb0, NetMsg_ReplaceEnginesPacket, uint8_t*, NetMsg_ReplaceEngines)

RT3_NETFWD(0x46cbd0, NetMsg_TrainCommand, uint32_t*, TrainCmd_Apply)

int32_t __fastcall Net_SendFiled48(const uint8_t* msg) { return Net_SendFiled(msg, 0xc, 8, 0x48); }
RT3_REPLACE(0x46cbf0, Net_SendFiled48);

int32_t __fastcall Net_SendFiled4c(const uint8_t* msg) { return Net_SendFiled(msg, 0xa, 6, 0x4c); }
RT3_REPLACE(0x46cc60, Net_SendFiled4c);

// 0x46ccd0 (vtable stub).
int32_t __fastcall Return1_46ccd0(void*) { return 0x1; }
RT3_REPLACE(0x46ccd0, Return1_46ccd0);

// 0x46cce0 (network handler). Pauses the world locally and starts the
// resync clock (0x6cd91c, 0x6ce990 / 0x6cd8e4). Returns 1.
int32_t NetMsg_PauseForSync() {
    World_SetSpeed(g_world, 0, 0, 0, 0);
    g_6cd91c = 1;
    const int32_t t = ClockMs();
    g_6ce990 = t;
    g_6cd8e4 = t;
    return 1;
}
RT3_REPLACE(0x46cce0, NetMsg_PauseForSync);

// 0x46cd10 / 0x4721a0 / 0x4720b0 (network handlers, return 1).
int32_t __fastcall NetMsg_Store6d1284(uint8_t* m) {
    g_6d1284 = *F<int32_t*>(m, 8);
    g_6d1280 = 3;
    return 1;
}
RT3_REPLACE(0x46cd10, NetMsg_Store6d1284);

// 0x46cd30 (network handler). Sends this peer's profile (message 0x54, 0x2c
// bytes): its address as name (also kept at the first entry's +0x258c),
// flag 1 of +0x5c, the game version, 0x6226d0 and +0x2694. Returns 1.
int32_t NetMsg_SendProfile() {
    char addr[0x100];
    const char* a = Net_LocalAddress(addr);
    strcpy((char*)Engine_FirstListEntry() + 0x258c, a);
    const float version = Game_VersionNumber();
    F<float>((uint8_t*)Engine_FirstListEntry(), 0x268c) = version;
    F<int32_t>((uint8_t*)Engine_FirstListEntry(), 0x2690) = g_6226d0;
    uint8_t pkt[0x12c];
    strcpy((char*)pkt, (const char*)Engine_FirstListEntry() + 0x258c);
    pkt[0x1f] = ((uint8_t*)Engine_FirstListEntry())[0x5c] & 1;
    F<float>(pkt, 0x20) = Game_VersionNumber();
    F<int32_t>(pkt, 0x24) = g_6226d0;
    F<uint32_t>(pkt, 0x28) = F<uint32_t>((uint8_t*)Engine_FirstListEntry(), 0x2694);
    Net_Send(g_session, 0, nullptr, 0x54, 0x2c, pkt, 3, 0);
    return 1;
}
RT3_REPLACE(0x46cd30, NetMsg_SendProfile);

// 0x46ce10 (network handler). Another peer's texture d[1] takes the packet's
// name (+0x258c) and values (+0x268c..), flag 1 from byte 0x1f. Returns 1.
int32_t __fastcall NetMsg_TextureInfo(uint8_t* m, const int32_t* d) {
    const int32_t key = d[1];
    if (Global6d40e4() == key) return 1;
    uint8_t* t = Textures_Find(key);
    if (!t) return 1;
    uint8_t* src = F<uint8_t*>(m, 8);
    strcpy((char*)t + 0x258c, (const char*)src);
    F<uint32_t>(t, 0x268c) = F<uint32_t>(src, 0x20);
    F<uint32_t>(t, 0x2690) = F<uint32_t>(src, 0x24);
    F<uint32_t>(t, 0x2694) = F<uint32_t>(src, 0x28);
    if (src[0x1f]) F<uint32_t>(t, 0x5c) |= 1;
    Net_UnlockQueueArg(d[1]);
    return 1;
}
RT3_REPLACE(0x46ce10, NetMsg_TextureInfo);

// 0x46ce90 (network handler). Counts download replies (0x6d128c) while one
// runs (0x6d127c): 0 fails it (state 8), 0xff ends it (0xd), and enough
// replies (0x6d4100) complete it (9). Returns 1.
int32_t __fastcall NetMsg_DownloadReply(uint8_t* m) {
    if (!g_6d127c) return 1;
    ++g_6d128c;
    const int32_t v = *F<const int32_t*>(m, 8);
    if (!v) {
        g_6d1280 = 8;
        return 1;
    }
    if (v == 0xff) {
        g_6d1280 = 0xd;
        return 1;
    }
    if (g_6d128c >= Global6d4100()) g_6d1280 = 9;
    return 1;
}
RT3_REPLACE(0x46ce90, NetMsg_DownloadReply);

int32_t __fastcall NetMsg_TextureFlag4(void*, const int32_t* d) {
    if (uint8_t* t = Textures_Find(d[1])) {
        F<uint32_t>(t, 0x5c) |= 4;
        Net_UnlockQueueArg(d[1]);
    }
    return 1;
}
RT3_REPLACE(0x46cf10, NetMsg_TextureFlag4);

// 0x46cf40 (network handler). Copies the packet into the world at +0x66be.
// Returns 1.
int32_t __fastcall NetMsg_CopyToWorld(uint8_t* m) {
    memcpy(g_world + 0x66be, F<const void*>(m, 8), F<uint32_t>(m, 0xc));
    return 1;
}
RT3_REPLACE(0x46cf40, NetMsg_CopyToWorld);

// 0x46cf70 (network handler). Clears flag 1 of every queued entry (+0x5c).
int32_t NetMsg_ClearQueueFlag1() {
    for (uint8_t* e = *(uint8_t**)(uintptr_t)Net_LockQueue(); e; e = *(uint8_t**)e) F<uint32_t>(e, 0x5c) &= ~1u;
    Net_UnlockQueue();
    return 1;
}
RT3_REPLACE(0x46cf70, NetMsg_ClearQueueFlag1);

// 0x46cfa0 (vtable stub).
int32_t __fastcall Return1_46cfa0(void*) { return 0x1; }
RT3_REPLACE(0x46cfa0, Return1_46cfa0);

// 0x46cfb0 (network handler). Saves the game under the packet's name (+8)
// and flags (+4) with 0x6ce9b8 set meanwhile. Returns 1.
int32_t __fastcall NetMsg_SaveGame(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    g_6ce9b8 = 1;
    World_SaveGame(g_world, 0, (const char*)d + 8, F<uint32_t>(d, 4));
    g_6ce9b8 = 0;
    return 1;
}
RT3_REPLACE(0x46cfb0, NetMsg_SaveGame);

// 0x46cfe0 (network handler). Starts a file transfer in the first free
// slot (10 x 0x5c at 0x6ce290, file +0x58): the packet is copied in and
// its file (d+8, in the save folder when d+0x4c) opened (mode 0x5ca334).
// Returns 1.
int32_t __fastcall NetMsg_BeginTransfer(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    int32_t i = 0;
    uint32_t p = 0x6ce2e8;
    for (; p < 0x6ce680; p += 0x5c, ++i)
        if (!*(const int32_t*)p) break;
    if (p >= 0x6ce680 || i == 10) return 1;
    memcpy((void*)(0x6ce290 + i * 0x5c), d, 0x17 * 4);
    const char* name = (const char*)d + 8;
    char path[0x12c];
    auto fopen_ = (void*(__cdecl*)(const char*, const char*))0x5a1887;
    void* f;
    if (F<int32_t>(d, 0x4c)) {
        FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(hook::As<void*>(g_app)), name, 0, 0, 0, 0, 0, 0);
        f = fopen_(path, (const char*)0x5ca334);
    } else {
        f = fopen_(name, (const char*)0x5ca334);
    }
    *(void**)(0x6ce2e8 + i * 0x5c) = f;
    return 1;
}
RT3_REPLACE(0x46cfe0, NetMsg_BeginTransfer);

// 0x46d090 (network handler). Writes a file transfer chunk (d+6, length
// +4) to transfer d[0]'s file, adds it to the received count (+0x54 of the
// slot) and shows the progress (string 0x10f6, percent of +0x50).
// Returns 1.
int32_t __fastcall NetMsg_TransferChunk(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    const uint16_t len = F<uint16_t>(d, 4);
    if (!len) return 1;
    int32_t i = 0;
    uint32_t p = 0x6ce2e8;
    for (; p < 0x6ce680; p += 0x5c, ++i)
        if (*(const int32_t*)(p - 0x10) == F<int32_t>(d, 0) && *(const int32_t*)p) break;
    if (p >= 0x6ce680 || i == 10) return 1;
    uint8_t* slot = (uint8_t*)(0x6ce290 + i * 0x5c);
    ((size_t(__cdecl*)(const void*, size_t, size_t, void*))0x5a276f)(d + 6, len, 1, F<void*>(slot, 0x58));
    F<int32_t>(slot, 0x54) += F<uint16_t>(d, 4);
    if (!g_windowManager) return 1;
    const float got = (float)F<int32_t>(slot, 0x54), total = (float)F<int32_t>(slot, 0x50);
    const int32_t pct = Ftol((double)got / (double)total * Fl(0x5c8c28));
    char num[0x190], text[0x200];
    const char* s = Num_Format(num, 1, (double)pct, 10, 1, 0, 0);
    const char* who = Net_PlayerNameById(F<int32_t>(slot, 0));
    FormatText(text, GetString(0x10f6), (const char*)(slot + 8), who, s, 0, 0, 0, 0, 0);
    Status_SetMessage(g_windowManager, 0, text);
    return 1;
}
RT3_REPLACE(0x46d090, NetMsg_TransferChunk);

// 0x46d1d0 (network handler). Frees the buffer (+0x10) of the transfer slot
// (10 entries of 0x5c bytes at 0x6ce2d8) keyed by the packet and clears the
// slot. Returns 1.
int32_t __fastcall NetMsg_EndTransfer(uint8_t* m) {
    const int32_t key = *F<const int32_t*>(m, 8);
    int32_t i = 0;
    for (uint32_t p = 0x6ce2d8; p < 0x6ce670; p += 0x5c, ++i)
        if (*(const int32_t*)p == key) {
            if (i != 10 && *(void**)(0x6ce2e8 + i * 0x5c)) {
                ((void(__cdecl*)(void*))0x5a1740)(*(void**)(0x6ce2e8 + i * 0x5c));
                memset((void*)(0x6ce290 + i * 0x5c), 0, 0x17 * 4);
            }
            return 1;
        }
    return 1;
}
RT3_REPLACE(0x46d1d0, NetMsg_EndTransfer);

// 0x46d230 (callback). Sets 0x6ce9c8; returns 1.
int32_t Set6ce9c8() { return g_6ce9c8 = 1; }
RT3_REPLACE(0x46d230, Set6ce9c8);

RT3_NOINLINE void CloseFile6ce994() {
    void*& f = g_6ce994;
    if (!f) return;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    f = nullptr;
}
RT3_REPLACE(0x46d240, CloseFile6ce994);

// 0x46d260. Appends an 18-byte record {u16, 4 dwords} to the table 0x6cd990 (max 128).
void __fastcall Queue6cd990_Push(uint16_t a, int32_t b, int32_t c, int32_t d, int32_t e) {
    int32_t n = g_6ce9a0;
    if (n >= 0x80) return;
    uint8_t* r = (uint8_t*)0x6cd990 + n * 18;
    F<uint16_t>(r, 0) = a;
    F<int32_t>(r, 2) = b;
    F<int32_t>(r, 6) = c;
    F<int32_t>(r, 0xa) = d;
    F<int32_t>(r, 0xe) = e;
    g_6ce9a0 = n + 1;
}
RT3_REPLACE(0x46d260, Queue6cd990_Push);

// 0x46d2b0. Whether every session player (0x6cd8d8) has at least version v.
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v) {
    if (!hook::As<void*>(g_session)) return 1;
    float cv = Net_CommonVersion();
    float need = (float)VersionInt(v);
    return cv < need || cv != cv ? 0 : 1;
}
RT3_REPLACE(0x46d2b0, Net_AllHaveVersion);

// 0x46d300. Feeds this engine's resync transfers (the slots of 0x6ce290 opened
// by it): while the send queue holds under 25 messages, up to 25 chunks of 3840
// bytes go out as message 0x80 {crc, length, data} (unless the transfer is
// addressed, +4) and are counted in +0x54; at the end of the file the end mark
// 0x81 {crc} follows and the slot is freed. Open transfers show their progress
// (string 0x10f5: file, receiver or 0x5ca2bc, percent). Keeps s in 0x6ce9cc.
RT3_NOINLINE void __fastcall Net_FeedResync(uint8_t* s) {
    if (Global6d40e4()) {
        for (uint32_t off = 0; off < 0x398; off += 0x5c) {
            uint8_t* rec = (uint8_t*)0x6ce290 + off;
            if (F<int32_t>(rec, 0) != Global6d40e4()) continue;
            int32_t queued = 0, bytes = 0;
            if (Net_SendQueueTotals(g_6d40d0, 0, &queued, &bytes) && (uint32_t)queued < 0x19) {
                for (int32_t chunk = 0;;) {
                    struct {
                        uint32_t crc;
                        uint16_t len;
                        uint8_t data[0xf00];
                    } pkt;
                    pkt.crc = F<uint32_t>(rec, 0x48);
                    int32_t n = (int32_t)((size_t(__cdecl*)(void*, size_t, size_t, void*))0x5a1b05)(pkt.data, 1, 0xf00,
                                                                                                  F<void*>(rec, 0x58));
                    if (n && !F<int32_t>(rec, 4)) {
                        pkt.len = (uint16_t)n;
                        Net_Send(g_session, 0, nullptr, 0x80, n + 6, (const uint8_t*)&pkt, 3, 0);
                        F<int32_t>(rec, 0x54) += n;
                    }
                    if (n == 0xf00) {
                        if (++chunk < 0x19) continue;
                        break;
                    }
                    uint32_t crc = F<uint32_t>(rec, 0x48);
                    Net_Send(g_session, 0, nullptr, 0x81, 4, (const uint8_t*)&crc, 3, 0);
                    ((int(__cdecl*)(void*))0x5a1740)(F<void*>(rec, 0x58));
                    memset(rec, 0, 0x5c);
                    break;
                }
            }
            if (!g_windowManager || !F<void*>(rec, 0x58)) continue;
            const char* who = F<int32_t>(rec, 4) ? Net_PlayerNameById(F<int32_t>(rec, 4)) : (const char*)0x5ca2bc;
            float sent = (float)F<int32_t>(rec, 0x54);
            float total = (float)F<int32_t>(rec, 0x50);
            double scale = *(const float*)0x5c8c28;
            int32_t pct;
            __asm {
                fld sent
                fdiv total
                fmul scale
                mov eax, 0x5a10d0
                call eax
                mov pct, eax
            }
            char num[0x130];
            const char* pctText = Num_Format(num, 1, (double)pct, 0xa, 1, 0, 0);
            char line[0xf00];
            FormatText(line, GetString(0x10f5), (const char*)rec + 8, who, pctText, 0, 0, 0, 0, 0);
            Status_SetMessage(g_windowManager, 0, line);
        }
    }
    g_6ce9cc = s;
}
RT3_REPLACE(0x46d300, Net_FeedResync);

// 0x46d530. Used slots of the table 0x6ce2d8 (10 x 0x5c bytes).
int32_t CountUsedSlots6ce2d8() {
    int32_t n = 0;
    for (int32_t i = 0; i < 10; ++i)
        if (*(const int32_t*)(0x6ce2d8 + i * 0x5c)) ++n;
    return n;
}
RT3_REPLACE(0x46d530, CountUsedSlots6ce2d8);

// 0x46d5b0. This year's +0x159 stat plus the total +0x105 (unrounded).
double __fastcall Train_Total159(uint8_t* t) { return Train_Float159(t, 0, 0) + F<float>(t, 0x105); }
RT3_REPLACE(0x46d5b0, Train_Total159);

// 0x46d5d0 / 0x46d5f0. Whether train t can run its orders (0x46d5f0 without
// electric track).
RT3_NOINLINE int32_t __fastcall Train_CanRun(uint8_t* t) { return Orders_Drivable((uint32_t*)t, 0, t, -1, 0, 0, -1); }
RT3_REPLACE(0x46d5d0, Train_CanRun);

RT3_NOINLINE int32_t __fastcall Train_CanRunNonElectric(uint8_t* t) { return Orders_Drivable((uint32_t*)t, 0, t, -1, 0, 1, -1); }
RT3_REPLACE(0x46d5f0, Train_CanRunNonElectric);

RT3_NOINLINE void __fastcall Obj_SetDateMinutes(uint8_t* o, int, const GameDate* d) {
    F<int32_t>(o, 0x8f38) = d ? DateToMinutes(d) : 0;
}
RT3_REPLACE(0x46d610, Obj_SetDateMinutes);

// 0x46d640. The host offers the resync save 0x6ce630 (in the save folder) to the
// other players: a free transfer slot (0x6ce290, 0x5c bytes, 10 = none) gets
// {engine value, 0, name +8, CRC +0x48, open +0x4c, size +0x50, file +0x58} and
// goes out reliably as message 0x7f. Nothing without other players or the file.
RT3_NOINLINE void Net_OfferResync() {
    if (!g_6d40dc || Global6d4100() <= 1) return;
    int32_t slot = FirstFreeSlot6ce2e8();
    if (slot == 10) return;
    uint8_t* rec = (uint8_t*)0x6ce290 + slot * 0x5c;
    memset(rec, 0, 0x5c);
    F<int32_t>(g_app, 0x6c) = 0;
    char path[0x12c];
    FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(hook::As<void*>(g_app)), (const char*)0x6ce630, 0, 0, 0, 0,
               0, 0);
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ce2c0);
    if (!f) return;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    F<int32_t>(rec, 0x4c) = 1;
    int32_t bytes;
    void* fn = (void*)&File_GetSize;
    char* p = path;
    __asm {
        mov eax, p
        call fn
        mov bytes, eax
    }
    F<int32_t>(rec, 0x50) = bytes;
    F<uint32_t>(rec, 0x48) = Crc32String((const char*)0x6ce630, 1);
    F<int32_t>(rec, 0) = Global6d40e4();
    F<int32_t>(rec, 4) = 0;
    F<void*>(rec, 0x58) = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c9678);
    strcpy((char*)rec + 8, (const char*)0x6ce630);
    Net_Send(g_session, 0, nullptr, 0x7f, 0x5c, rec, 3, 0);
}
RT3_REPLACE(0x46d640, Net_OfferResync);

// Reverse engineered by GPT6.1 SOL MEDIUM (entry stub for 0x46d780).
// The original receives the packet in EAX, not on the stack or in ECX.
__declspec(naked) void Net_ApplyTrainPosition() {
    __asm {
        push ecx
        push edx
        push eax
        call Net_ApplyTrainPosition_Impl
        add esp, 4
        pop edx
        pop ecx
        ret
    }
}
RT3_REPLACE(0x46d780, Net_ApplyTrainPosition);

// 0x46d7f0. Selects the scenario with this file name: from the scenario list (kind
// 7, option +0x6c set), else from the kind-12 list (option +0x6c clear, name kept
// at 0x6ce630). Returns whether it was found.
RT3_NOINLINE int32_t __fastcall Scenario_SelectByName(const char* name) {
    F<int32_t>(g_app, 0x6c) = 1;
    if (FindInList(name, 7)) return 1;
    F<int32_t>(g_app, 0x6c) = 0;
    return FindInList(name, 0xc);
}
RT3_REPLACE(0x46d7f0, Scenario_SelectByName);

// 0x46d980. Name in EBX.
__declspec(naked) void Profiles_IndexOf() {
    __asm {
        push ebx
        call Profiles_IndexOfImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46d980, Profiles_IndexOf);

// 0x46d9e0. The index of the player profile (table 0x6ceb9c) whose name matches
// that of texture entry key, or 0xff; releases the entry's lock.
RT3_NOINLINE int32_t __stdcall Profiles_IndexOfEntry(int32_t key) {
    int32_t idx = 0xff;
    uint8_t* e = Textures_Find(key);
    if (!e) return idx;
    for (int32_t i = 0; i < Collection_Count(g_players); ++i) {
        Collection* t = g_players;
        uint8_t* p = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (!Stricmp((const char*)p + 8, (const char*)e + 8)) {
            idx = i;
            break;
        }
    }
    Net_UnlockQueueArg(key);
    return idx;
}
RT3_REPLACE(0x46d9e0, Profiles_IndexOfEntry);

// 0x46da70. Position-weighted checksum of the map bitmap +0x213d.
RT3_NOINLINE int32_t MapBitmapChecksum() {
    uint8_t* map = g_map;
    int32_t n = (F<int32_t>(map, 0x2161) * F<int32_t>(map, 0x215d) + 7) / 8;
    int32_t sum = 0;
    const uint8_t* buf = F<uint8_t*>(map, 0x213d);
    for (int32_t off = 0; off < n; ) {
        int32_t v = *(const int32_t*)(buf + off);
        off += 4;
        if (v) sum += v * off;
    }
    return sum;
}
RT3_REPLACE(0x46da70, MapBitmapChecksum);

// 0x46dac0. State packet 0x6ce9d0: type 8, the map's cargo value x 0x5c8c28
// truncated, the world clock in minutes. Null without a world and map.
uint8_t* Net_BuildCargoPacket() {
    if (!g_world || !g_map) return nullptr;
    g_6ce9d0 = 8;
    g_6ce9d1 = Ftol((double)Map_TotalCargoValue(g_map, 0, 0) * Fl(0x5c8c28));
    g_6ce9d5 = DateToMinutes((const GameDate*)(g_world + 0xd));
    return (uint8_t*)0x6ce9d0;
}
RT3_REPLACE(0x46dac0, Net_BuildCargoPacket);

// Network state packets (type, data, the world clock in minutes); null
// without a world and map.
uint8_t* Net_BuildClockPacketA() {
    uint8_t* w = g_world;
    if (!w || !g_map) return nullptr;
    g_6ce9dc = 0xa;
    g_6ce9dd = 0;
    g_6ce9e1 = DateToMinutes((const GameDate*)(w + 0xd));
    return (uint8_t*)0x6ce9dc;
}
RT3_REPLACE(0x46db10, Net_BuildClockPacketA);

uint8_t* Net_BuildClockPacket9() {
    uint8_t* w = g_world;
    if (!w || !g_map) return nullptr;
    g_6ce9e9 = g_6cd92c;
    g_6ce9e8 = 9;
    g_6ce9ed = DateToMinutes((const GameDate*)(w + 0xd));
    return (uint8_t*)0x6ce9e8;
}
RT3_REPLACE(0x46db50, Net_BuildClockPacket9);

uint8_t* Net_BuildMapPacket() {
    uint8_t* w = g_world;
    if (!w || !g_map) return nullptr;
    g_6ce9f4 = 7;
    g_6ce9f5 = MapBitmapChecksum();
    g_6ce9f9 = DateToMinutes((const GameDate*)(w + 0xd));
    return (uint8_t*)0x6ce9f4;
}
RT3_REPLACE(0x46db90, Net_BuildMapPacket);

// 0x46dbd0. Companies checksum packet 0x6cea00: type 6, the world clock in
// minutes and the sum over the companies (weights index + 1) of their idle
// buildings, train count and fields +0x3b / +0x7664 / +0x3f / +0x37 (this
// peer's pending value 0x622094 counts for the player's own slot). Null
// without a world or companies.
uint8_t* Net_BuildCompaniesPacket() {
    if (!g_world || !g_companies) return nullptr;
    g_6cea00 = 6;
    g_6cea01 = 0;
    g_6cea05 = DateToMinutes((const GameDate*)(g_world + 0xd));
    int32_t sum = 0;
    for (int32_t k = 1; k < Collection_Count(g_companies) + 1; ++k) {
        Collection* c = g_companies;
        uint8_t* p = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)(k - 1), 0));
        const int32_t w = F<int32_t>(p, 0) + 1;
        const int32_t a = F<int32_t>(p, 0x3b), b = F<int32_t>(p, 0x7664), e = F<int32_t>(p, 0x3f);
        sum += (Company_CountIdleBuildings(p) + a + b + e) * w;
        sum += Company_TrainCount(p) * w;
        int32_t v = F<int32_t>(p, 0x37);
        if (g_world && Obj_Get21(g_world) && g_622094 != -1 &&
            Obj_Get21(g_world) == w - 1)
            v = g_622094;
        sum += v * w;
    }
    g_6cea01 = sum;
    return (uint8_t*)0x6cea00;
}
RT3_REPLACE(0x46dbd0, Net_BuildCompaniesPacket);

// 0x46dd10. Players checksum packet 0x6cea0c: type 5, the world clock in
// minutes and the sum over the players (weights index + 1) of their name
// CRCs and state fields (+0x291 / +0x1e1 / +4, colour +0x15c; this peer's
// pending colour 0x622090 counts for its own slot). Null without a world or
// players.
uint8_t* Net_BuildPlayersPacket() {
    if (!g_world || !g_players) return nullptr;
    g_6cea0c = 5;
    g_6cea0d = 0;
    g_6cea11 = DateToMinutes((const GameDate*)(g_world + 0xd));
    int32_t sum = 0;
    for (int32_t k = 1; k < Collection_Count(g_players) + 1; ++k) {
        Collection* c = g_players;
        uint8_t* p = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)(k - 1), 0));
        const int32_t w = F<int32_t>(p, 0) + 1;
        sum += (int32_t)Crc32String((const char*)p + 8, 1);
        sum += ((int8_t)p[0x291] + F<int32_t>(p, 0x1e1) + F<int32_t>(p, 4)) * w;
        int32_t colour = p[0x15c];
        if (g_world && Obj_Get25(g_world) && g_622090 != -1 &&
            Obj_Get25(g_world) == w - 1)
            colour = g_622090;
        sum += colour * w;
    }
    g_6cea0d = sum;
    return (uint8_t*)0x6cea0c;
}
RT3_REPLACE(0x46dd10, Net_BuildPlayersPacket);

// 0x46de40. Builds the 9-byte state packet 0x6cea18: type 4, the world
// clock in minutes and the random seed. Returns the packet.
uint8_t* Net_BuildStatePacket() {
    g_6cea18 = 4;
    g_6cea19 = 0;
    g_6cea1d = DateToMinutes((const GameDate*)(g_world + 0xd));
    g_6cea19 = GetSeed();
    return (uint8_t*)0x6cea18;
}
RT3_REPLACE(0x46de40, Net_BuildStatePacket);

// 0x46de80. Buildings checksum packet 0x6cea24: type 1, the world clock in
// minutes and, over the placed buildings of 0x62b26c (+0x3d4 == 1, weight
// id + 1), their state (+0x246 set, +0x3cc, +0x29e), truncated position x
// and z, and the map heights (0x62c120, x 0x5c8c28) under their footprint
// (bounds of +0x24e +0x10, in quarter cells). Null without a world or
// buildings.
uint8_t* Net_BuildBuildingsPacket() {
    if (!g_world || !g_buildings) return nullptr;
    g_6cea24 = 1;
    g_6cea25 = 0;
    g_6cea29 = DateToMinutes((const GameDate*)(g_world + 0xd));
    int32_t sum = 0;
    for (int32_t i = 0; i + 1 < Collection_Count(g_buildings) + 1; ++i) {
        Collection* c = g_buildings;
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (Obj_Flag3d4(o) != 1) continue;
        const int32_t w = (int32_t)Collection_IdAt(g_buildings, 0, (uint32_t)i, 0) + 1;
        uint32_t x, y, z;
        SceneObj_GetPos(o, 0, &x, &y, &z);
        const int32_t state = ((F<int32_t>(o, 0x246) != 0 ? 1 : 0) + F<int32_t>(o, 0x3cc) + F<int32_t>(o, 0x29e)) * w;
        const int32_t px = Ftol(*(const float*)&x);
        const int32_t pz = Ftol(*(const float*)&z);
        sum = pz + sum + state + px;
        int32_t box[4];
        Points_Bounds(hook::As<void*>(g_buildingTypes), 0, (const uint32_t*)(F<uint8_t*>(o, 0x24e) + 0x10), box);
        const int32_t x0 = box[0] >> 2, x1 = box[1] >> 2, y0 = box[2] >> 2, y1 = box[3] >> 2;
        for (int32_t gy = y0; gy <= y1; ++gy)
            for (int32_t gx = x0; gx <= x1; ++gx) {
                const float hgt = (float)Map_GridFloat(g_map, 0, gx, gy);
                sum += Ftol((double)hgt * Fl(0x5c8c28)) * w;
            }
    }
    g_6cea25 = sum;
    return (uint8_t*)0x6cea24;
}
RT3_REPLACE(0x46de80, Net_BuildBuildingsPacket);

 // namespace

// 0x46e030. Track checksum packet 0x6cea30: type 2, the world clock in
// minutes and, unless a tool is open (0x6d1a8c) outside a resync
// (0x6ce9a4), over the built track pieces of 0x6cfca8 (+0x216 == 2, weight
// id + 1) their centre cell, height, state fields and the length of the sum
// of their four effect vectors.
uint8_t* Net_BuildTrackPacket() {
    if (!g_world || !g_tracks) return nullptr;
    g_6cea30 = 2;
    g_6cea31 = 0;
    g_6cea35 = DateToMinutes((const GameDate*)(g_world + 0xd));
    int32_t sum = 0;
    if (!g_6d1a8c || g_6ce9a4) {
        for (int32_t i = 0; i + 1 < Collection_Count(g_tracks) + 1; ++i) {
            Collection* c = g_tracks;
            uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
            if (o[0x216] != 2) continue;
            const int32_t w = (int32_t)Collection_IdAt(g_tracks, 0, (uint32_t)i, 0) + 1;
            float x, y;
            Obj_CenterCell(o, 0, &x, &y);
            const int32_t cx = Ftol((double)x * Fl(0x5c8c28));
            const int32_t cy = Ftol((double)y * Fl(0x5ce2c4));
            sum -= (cy - cx) * w;
            sum += Ftol((double)Obj_MidAboveGround(o) * Fl(0x5c8c28)) * w;
            sum += ((F<int32_t>(o, 0x20e) != -1 ? 1 : 0) + F<uint16_t>(o, 0x22e) + o[0x218] + F<int32_t>(o, 0x202) + o[0x216]) * w;
            float a[3], b[3], cc[3], d[3];
            Effect_GetVectors(o, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)cc, (uint32_t*)d);
            float s0 = (float)((double)b[2] + a[2]);
            float s1 = (float)((double)b[1] + a[1]);
            float s2 = (float)((double)b[0] + a[0]);
            s0 = (float)((double)cc[2] + s0);
            s1 = (float)((double)cc[1] + s1);
            s2 = (float)((double)s2 + cc[0]);
            s0 = (float)((double)d[2] + s0);
            s1 = (float)((double)d[1] + s1);
            s2 = (float)((double)s2 + d[0]);
            const double len2 = (double)s2 * s2 + (double)s1 * s1 + (double)s0 * s0;
            sum += Ftol(len2 * Fl(0x5c8c28)) * w;
        }
    }
    g_6cea31 = sum;
    return (uint8_t*)0x6cea30;
}
RT3_REPLACE(0x46e030, Net_BuildTrackPacket);

// 0x46e250. Trains checksum packet 0x6cea3c: type 3, the world clock in
// minutes and, over the trains of 0x6cfcbc (weight id + 1), their lead
// car's position, state and speed fields, the cargo floats, the lead car's
// piece / tilt and each further car (Train_NthCarKind1) of kind 1.
uint8_t* Net_BuildTrainsPacket() {
    if (!g_world || !g_trains) return nullptr;
    g_6cea3c = 3;
    g_6cea3d = 0;
    g_6cea41 = DateToMinutes((const GameDate*)(g_world + 0xd));
    int32_t sum = 0;
    auto q = [](float v) { return Ftol((double)v * Fl(0x5ce2c4)); };
    for (int32_t i = 0; i + 1 < Collection_Count(g_trains) + 1; ++i) {
        Collection* c = g_trains;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        const int32_t w = (int32_t)Collection_IdAt(g_trains, 0, (uint32_t)i, 0) + 1;
        float px, py, pz;
        SceneObj_GetPos((uint8_t*)Obj_City41(t), 0, (uint32_t*)&px, (uint32_t*)&py, (uint32_t*)&pz);
        const int32_t a = q(px);
        const int32_t b = Ftol((double)pz * Fl(0x5c8c28));
        sum += (b - a + F<int32_t>(t, 0xe8)) * w;
        sum += Obj_ByteF9(t) * w;
        sum += Train_FirstCarType126(t) * w;
        const float cc = F<float>(t, 0xcc);
        const int32_t owner = F<int32_t>(t, 0x51);
        int32_t v = Ftol(Train_Float159(t, 0, 0) + F<float>(t, 0x105));
        v -= q(cc);
        sum += (v + owner) * w;
        const float f[8] = { F<float>(t, 0xe4), F<float>(t, 0xe0), F<float>(t, 0xdc), F<float>(t, 0xf5),
                             F<float>(t, 0x65), F<float>(t, 0x61), F<float>(t, 0xf0), F<float>(t, 0xec) };
        const int32_t d1 = q(F<float>(t, 0x1a7));
        int32_t e = Ftol((double)F<float>(t, 0x8c) * Fl(0x5c8c28)) - d1;
        e -= q(F<float>(t, 0x5d));
        e -= q(F<float>(t, 0x59));
        e -= q(F<float>(t, 0x55));
        e += t[0x1c5];
        for (int32_t k = 0; k < 8; ++k) e -= q(f[k]);
        e += t[0x8b] + F<int32_t>(t, 0xd8) + F<int32_t>(t, 0xb0) + F<int32_t>(t, 0x94) + F<int32_t>(t, 0x90);
        sum += e * w;
        uint8_t* lead = (uint8_t*)Obj_City41(t);
        const int32_t piece = F<int32_t>(lead, 0x356);
        const float pos = F<float>(lead, 0x35e);
        const int32_t tilt = Ftol((double)Car_TiltValue(lead) * Fl(0x5c8c28));
        sum += (tilt - q(pos) + piece) * w;
        int32_t n = 2;
        for (uint8_t* car = (uint8_t*)Train_NthCarKind1(t, 0, 1); car; car = (uint8_t*)Train_NthCarKind1(t, 0, n++))
            sum += (F<int32_t>(car, 0x38c) - q(F<float>(car, 0x394))) * w;
    }
    g_6cea3d = sum;
    return (uint8_t*)0x6cea3c;
}
RT3_REPLACE(0x46e250, Net_BuildTrainsPacket);

// 0x46e5c0. The multiplayer finance sync message (16-byte header, +0xd company
// count, +0xe player count). Mode 1 builds it into a malloc'd buffer (*out,
// returns its size): per company its cash, this year's two book figures
// (0x2c / 0x2d), the five loan / bond figures 0xe..0x12 and its share count;
// per player the cash and the ten holdings +0x15d. Otherwise it applies the
// message in place, overwriting whatever differs (book figures through
// Company_AddStat), and returns the companies' part size.
RT3_NOINLINE int32_t __fastcall Finance_SyncMessage(uint8_t* msg, int32_t mode, void** out) {
    Collection* cos = g_companies;
    Collection* pls = g_players;
    auto year = [] { return (int32_t)F<uint16_t>(g_world, 0xd); };
    auto book = [&](int32_t i, int32_t stat) {
        float v = 0.0f;
        uint32_t id = Collection_IdAt(cos, 0, i, 0);
        if (Collection_IsValid(cos, 0, id)) v = (float)Company_Stat((uint8_t*)Collection_Get(cos, 0, id), 0, stat, year());
        return v;
    };
    if (mode == 1) {
        static uint8_t coRecs[0x24 * 256], plRecs[0x2c * 256];
        int32_t nc = 0, np = 0;
        if (cos)
            for (; nc < Collection_Count(cos); ++nc) {
                uint8_t* c = (uint8_t*)Collection_Get(cos, 0, Collection_IdAt(cos, 0, nc, 0));
                uint8_t* r = coRecs + nc * 0x24;
                F<float>(r, 0) = (float)Company_Stat(c, 0, 0xd, 0x2329);
                F<float>(r, 4) = book(nc, 0x2c);
                F<float>(r, 8) = book(nc, 0x2d);
                for (int32_t s = 0; s < 5; ++s) F<float>(r, 0xc + s * 4) = (float)Company_Stat(c, 0, 0xe + s, 0x2329);
                F<int32_t>(r, 0x20) = F<int32_t>(c, 0x47);
            }
        msg[0xd] = (uint8_t)nc;
        if (pls)
            for (; np < Collection_Count(pls); ++np) {
                uint8_t* p = (uint8_t*)Collection_Get(pls, 0, Collection_IdAt(pls, 0, np, 0));
                uint8_t* r = plRecs + np * 0x2c;
                F<float>(r, 0) = (float)F<double>(p, 0x154);
                memcpy(r + 4, p + 0x15d, 0x28);
            }
        msg[0xe] = (uint8_t)np;
        int32_t size = np * 0x2c + nc * 0x24 + 0x10;
        uint8_t* buf = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)(size);
        memcpy(buf, msg, 0x10);
        if (nc) memcpy(buf + 0x10, coRecs, nc * 0x24);
        if (np) memcpy(buf + 0x10 + nc * 0x24, plRecs, np * 0x2c);
        *out = buf;
        return size;
    }
    int32_t ret = 0;
    if (msg[0xd]) {
        uint8_t* r = msg + 0x10;
        if (cos)
            for (int32_t i = 0; i < Collection_Count(cos); ++i, r += 0x24) {
                uint8_t* c = (uint8_t*)Collection_Get(cos, 0, Collection_IdAt(cos, 0, i, 0));
                double cash = (double)F<float>(r, 0);
                if (!(Company_Stat(c, 0, 0xd, 0x2329) == cash)) F<double>(c, 0x11f7) = cash;
                float b1 = book(i, 0x2c), b2 = book(i, 0x2d);
                if (!(F<float>(r, 4) == b1)) Company_AddStat(c, 0, 4, (float)((double)F<float>(r, 4) - b1));
                if (!(F<float>(r, 8) == b2)) Company_AddStat(c, 0, 0xc, (float)((double)F<float>(r, 8) - b2));
                for (int32_t s = 0; s < 5; ++s) {
                    double cur = Company_Stat(c, 0, 0xe + s, 0x2329);
                    double v = (double)F<float>(r, 0xc + s * 4);
                    if (!(v == cur)) F<double>(c, 0x124f + s * 0x58) = v;
                }
                if (F<int32_t>(r, 0x20) != F<int32_t>(c, 0x47)) F<int32_t>(c, 0x47) = F<int32_t>(r, 0x20);
            }
        ret = msg[0xd] * 0x24;
    }
    if (msg[0xe] && pls) {
        uint8_t* r = msg + ret + 0x10;
        for (int32_t j = 0; j < Collection_Count(pls); ++j, r += 0x2c) {
            uint8_t* p = (uint8_t*)Collection_Get(pls, 0, Collection_IdAt(pls, 0, j, 0));
            double cash = (double)F<float>(r, 0);
            if (!(F<double>(p, 0x154) == cash)) F<double>(p, 0x154) = cash;
            memcpy(p + 0x15d, r + 4, 0x28);
        }
    }
    return ret;
}
RT3_REPLACE(0x46e5c0, Finance_SyncMessage);

__declspec(naked) void Player_OnLeft() {
    __asm {
        push esi
        call Player_OnLeft_Impl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46ecd0, Player_OnLeft);

// 0x46ed30. Request in EBX.
__declspec(naked) void Companies_Clash() {
    __asm {
        push ebx
        call Companies_ClashImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46ed30, Companies_Clash);

// 0x46edb0. Validate flag in EAX, data on the stack.
__declspec(naked) void NetTrain_Create() {
    __asm {
        push [esp + 4]
        push eax
        call NetTrain_CreateImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x46edb0, NetTrain_Create);

// 0x46eec0. Whether a train is in state 1 or 2 (only for sessions at version 0x5c9800+).
RT3_NOINLINE int32_t __fastcall Trains_AnyActive() {
    if (hook::As<void*>(g_session)) {
        float cv = Net_CommonVersion();
        if (cv < *(const float*)0x5c9800 || cv != cv) return 0;
    }
    Collection* c = g_trains;
    if (!c) return 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        c = g_trains;
        int32_t st = F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0xe8);
        if (st == 1 || st == 2) return 1;
        c = g_trains;
    }
    return 0;
}
RT3_REPLACE(0x46eec0, Trains_AnyActive);

// 0x46ef40 body (message m in ESI). A network building message {x, y,
// angle, company, type, editor}: phase 1 only answers whether it fits (1,
// else -1). Otherwise a fitting building is added, turned and placed; with
// trains running (0x46eec0) a kind-4 building's placement waits in the
// 32-entry ring 0x6ce808 (next 0x6ce9b0) instead of moving now. A refused
// placement is removed again. Returns the building id or -1.
int32_t __cdecl BuildingMsg_ApplyImpl(uint8_t* m, int32_t phase) {
    const int32_t ok = Sites_CanPlace(g_buildingTypes, 0, F<uint32_t>(m, 0x10), F<float>(m, 0), F<float>(m, 4), F<float>(m, 8),
                                      F<int32_t>(m, 0xc), F<int32_t>(m, 0x14), nullptr, 0, 1, -1, 0);
    if (phase == 1) return ok ? 1 : -1;
    if (!ok) return -1;
    g_6ce9ac = Trains_AnyActive();
    uint8_t* type = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(m, 0x10));
    const int32_t id = (int32_t)Buildings_Add(g_buildings, 0, type, F<float>(m, 0), F<float>(m, 4), 0, F<int32_t>(m, 0xc), 0);
    if (Collection_IsValid(g_buildings, 0, (uint32_t)id)) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, (uint32_t)id);
        SceneObj_SetAngle2(b, 0, F<uint32_t>(m, 8));
        if (g_6ce9ac && Obj_StateKindIs4(b)) {
            const int32_t k = g_6ce9b0;
            uint8_t* slot = (uint8_t*)0x6ce808 + k * 0xc;
            F<int32_t>(slot, 0) = id;
            F<uint32_t>(slot, 4) = F<uint32_t>(m, 0);
            F<uint32_t>(slot, 8) = F<uint32_t>(m, 4);
            g_6ce9b0 = (k + 1) % 32;
        } else {
            Building_MoveTo(b, 0, F<float>(m, 0), F<float>(m, 4), 0, 0.0f);
        }
        SceneObj_ResetAttachmentLists(b);
        if (!Building_Place((uint8_t*)Collection_Get(g_buildings, 0, (uint32_t)id), 0, F<float>(m, 0), F<float>(m, 4),
                            F<uint32_t>(m, 0xc))) {
            Collection* bs = g_buildings;
            ((void(__fastcall*)(Collection*, int, int32_t))(*(void***)bs)[8 / 4])(bs, 0, id);
        }
    }
    g_6ce9ac = 0;
    return id;
}

// 0x46ef40. Message in ESI, phase on the stack.
__declspec(naked) int32_t BuildingMsg_Apply() {
    __asm {
        push [esp + 4]
        push esi
        call BuildingMsg_ApplyImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x46ef40, BuildingMsg_Apply);

// 0x46f0a0 body (message m in ECX, phase in EAX). A network track-building
// message: phase 1 only queues it (0x46a5f0) once its area is free
// (0x46a660). Otherwise the build settings (+0x46.. into the editor
// options and the electric flag), the grade globals (+0x22 / +0x26 / +0x2a)
// and, for a drag (+6), its anchor (+0x12, or a new node at (+0xa, +0xe)
// within +0x16 for company +4, kept only when new) are applied, a lost
// anchor is re-placed (QUIRK: its owner keeps the upper half of the
// network's address), and the line is extended to (+0x1a, +0x1e) as the
// player's tool does. The network's drag state is then cleared and, when a
// piece resulted, the build state reset and window 0x808 refreshed.
// Returns whether a piece resulted.
RT3_NOINLINE int32_t __fastcall TrackMsg_ApplyImpl(uint8_t* m, int32_t phase) {
    if (phase == 1) {
        if (Obj_RectFree(g_session, 0, (const int32_t*)(m + 0x32), m[4]) == 1) {
            NetQueue_Push(g_session, 0, (const uint32_t*)(m + 0x32), m[4]);
            return 1;
        }
        return 0;
    }
    auto tracks = [] { return hook::As<uint8_t*>(g_tracks); };
    auto valid = [&](int32_t id) { return Collection_IsValid((Collection*)tracks(), 0, (uint32_t)id) != 0; };
    Train_ResetMotion(tracks());
    uint8_t* ed = g_app;
    F<int32_t>(ed, 0x140) = m[0x46];
    F<int32_t>(ed, 0x13c) = m[0x47];
    F<int32_t>(ed, 0x138) = (int8_t)m[0x48];
    F<int32_t>(ed, 0x144) = (int8_t)m[0x49];
    F<int32_t>(g_world, 0x4c74) = (int8_t)m[0x4a];
    g_5f2b4c = F<uint32_t>(m, 0x22);
    g_6cd988 = F<uint32_t>(m, 0x26);
    g_6ce6f8 = F<uint32_t>(m, 0x2a);
    if (F<int32_t>(m, 6)) {
        if (valid(F<int32_t>(m, 0x12)) && !F<int32_t>(m, 0x2e)) {
            F<int32_t>(tracks(), 0x125) = F<int32_t>(m, 0x12);
        } else {
            int32_t missing = (int32_t)(uintptr_t)m;
            F<int32_t>(m, 0x12) = TrackNet_FindOrAddNode((Collection*)tracks(), 0, F<float>(m, 0xa), F<float>(m, 0xe), F<float>(m, 0x16), m[5], &missing, 1, m[4], 1,
                                                         -1, 0);
            if (!missing) F<int32_t>(m, 0x12) = -1;
        }
        if (valid(F<int32_t>(m, 0x12))) {
            F<int32_t>(tracks(), 0x118) = F<int32_t>(m, 6);
            F<uint32_t>(tracks(), 0x11d) = F<uint32_t>(m, 0xa);
            F<uint32_t>(tracks(), 0x121) = F<uint32_t>(m, 0xe);
            Obj_CopyCorners(tracks(), 0, (uint8_t*)Collection_Get((Collection*)tracks(), 0, F<uint32_t>(m, 0x12)));
        }
        F<int32_t>(tracks(), 0x125) = F<int32_t>(m, 0x12);
    }
    if (F<int32_t>(tracks(), 0x125) == -1 && g_6d1a8c) F<int32_t>(tracks(), 0x125) = F<int32_t>(g_6d1a8c, 0x78);
    if (F<int32_t>(tracks(), 0x125) != -1 && valid(F<int32_t>(tracks(), 0x125)))
        Track_UpdateColor((uint8_t*)Collection_Get((Collection*)tracks(), 0, F<uint32_t>(tracks(), 0x125)));
    if (F<int32_t>(tracks(), 0x125) != -1 && !valid(F<int32_t>(tracks(), 0x125))) {
        uint8_t* t = tracks();
        const int32_t style = ExtendStyle(t, m[5]);
        const int32_t owner = (int32_t)(((uint32_t)(uintptr_t)t & 0xffff0000u) | m[4]);
        F<int32_t>(tracks(), 0x125) = hook::Orig<int32_t(__fastcall*)(Collection*, int, float, float, float, int32_t, int32_t, int32_t, int32_t)>(0x493cf0)(
            (Collection*)t, 0, F<float>(t, 0x11d), F<float>(t, 0x121), 0.0f, 0, owner, style, 0);
    }
    F<int32_t>(tracks(), 0x9c) = 0;
    g_6ce99c = Route_GradeFactor(F<float>(m, 0x1a), F<float>(m, 0x1e), F<float>(m, 0x22), F<float>(m, 0x26), F<float>(m, 0x2a));
    const int32_t last = TrackNet_ExtendTo((Collection*)tracks(), 0, F<float>(m, 0x1a), F<float>(m, 0x1e), 1, m[4], m[5], g_6ce99c, nullptr);
    if (valid(last)) {
        F<int32_t>(tracks(), 0x125) = last;
        g_6ce998 = F<int32_t>(tracks(), 0x9c);
    }
    if (F<int32_t>(tracks(), 0x118)) Obj_DecayCounters(tracks());
    tracks()[0x11c] = 0;
    F<int32_t>(tracks(), 0x118) = 0;
    Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
    Tracks_ApplyPendingKinds((Collection*)tracks());
    Obj_FreeList1c9(tracks());
    if (valid(last)) {
        TrackList_ResetBuildState(tracks());
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xbb, 0x808, 0, 0, 0);
    }
    g_5f2b4c = 0x49741cf0;
    return valid(last) ? 1 : 0;
}

__declspec(naked) int32_t TrackMsg_Apply() {
    __asm {
        mov edx, eax
        jmp TrackMsg_ApplyImpl
    }
}
RT3_REPLACE(0x46f0a0, TrackMsg_Apply);

// 0x46f410. Cancels the current mouse action: a pending placement in the dialog
// 0x6d1a8c (+0x83; cursor 4), a pick in panel 0x6d3ac8 (+0x78; 0x6d1aa4 = -1), or
// a drag of panel 0x6d1720 (+0x94); the cursor goes back to 1.
RT3_NOINLINE void CancelMouseAction() {
    if (uint8_t* d = g_6d1a8c) {
        if (!F<int32_t>(d, 0x83)) return;
        F<int32_t>(d, 0x83) = 0;
        Cursor_Set(g_cursorManager, 0, 4, 0, 0);
        return;
    }
    if (uint8_t* p = g_6d3ac8) {
        if (!F<int32_t>(p, 0x78)) return;
        SetGlobal6d1aa4(-1);
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
        return;
    }
    if (uint8_t* p = g_6d1720) {
        if (!F<int32_t>(p, 0x94)) return;
        Panel_SetDragging(p, 0, 0);
    }
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
}
RT3_REPLACE(0x46f410, CancelMouseAction);

 // namespace

// 0x46f4a0 (net callback). Session events (msg +0x10): 1..3 / 7..10 set the
// connection state (0x6cd8d8 +0xc: 1, 2, 3, 4..7; 1 also stores +0x1c); 4
// (host migrated) takes the flagged player's name as host and, in a game,
// drops old messages, cancels the mouse action and reports it (string 0xb77
// when this peer became host, which also tells the others; else 0xb78); 5
// takes the flagged player's name; 6 (player joined) caps the AI slots and in
// a game logs the arrival (own: its line +0x78; others: string 0xb76, giving
// them the first free colour slot 0x9864 and, as host, the notice). Always 1.
int32_t __fastcall NetMsg_SessionEvent(uint8_t* msg) {
    uint8_t* gs = g_session;
    switch (F<int32_t>(msg, 0x10)) {
    case 1:
        F<int32_t>(gs, 0xc) = 1;
        F<uint32_t>(gs, 0x8f1c) = F<uint32_t>(msg, 0x1c);
        break;
    case 2:
        F<int32_t>(gs, 0xc) = 2;
        break;
    case 3:
        F<int32_t>(gs, 0xc) = 3;
        break;
    case 7:
        F<int32_t>(gs, 0xc) = 4;
        break;
    case 8:
        F<int32_t>(gs, 0xc) = 5;
        break;
    case 9:
        F<int32_t>(gs, 0xc) = 6;
        break;
    case 10:
        F<int32_t>(gs, 0xc) = 7;
        break;
    case 5:
        if (Net_FindFlaggedPlayer()) Obj_SetName8f48(g_session, 0, (const char*)Net_FindFlaggedPlayer() + 8);
        break;
    case 4: {
        Obj_SetName8f48(g_session, 0, (const char*)Net_FindFlaggedPlayer() + 8);
        if (!g_world || !hook::As<void*>(g_messageLog)) break;
        Net_DropOldMessages(g_session, 0, (uint8_t*)Engine_FirstListEntry(), 1);
        CancelMouseAction();
        uint8_t* p = F<uint8_t*>(msg, 8);
        if (F<int32_t>(Engine_FirstListEntry(), 0x48) == F<int32_t>(p, 0x48)) {
            FormatText((char*)0x6ce700, GetString(0xb77), 0, 0, 0, 0, 0, 0, 0, 0);
            LogLine460();
            SendSessionNotice460();
        } else {
            FormatText((char*)0x6ce700, GetString(0xb78), (const char*)p + 8, 0, 0, 0, 0, 0, 0, 0);
            LogLine460();
        }
        break;
    }
    case 6: {
        NewGame_CapAiSlots();
        if (g_world && hook::As<void*>(g_messageLog)) {
            uint8_t* p = F<uint8_t*>(msg, 8);
            if (F<int32_t>(p, 4) == Global6d40e4()) {
                Log_Add(g_messageLog, 0, (const char*)p + 0x78, 4, 0, 0);
            } else {
                FormatText((char*)0x6ce700, GetString(0xb76), (const char*)p + 8, 0, 0, 0, 0, 0, 0, 0);
                LogLine460();
                uint8_t* slots = g_session + 0x9864;
                for (int32_t i = 0; i < 0x10; ++i) {
                    if (slots[i] != 0xff) continue;
                    const char* name = (const char*)F<uint8_t*>(msg, 8) + 8;
                    int32_t k;
                    __asm {
                        push ebx
                        mov ebx, name
                        call Profiles_IndexOf
                        pop ebx
                        mov k, eax
                    }
                    slots[i] = (uint8_t)k;
                    break;
                }
                if (g_6d40dc && !Stricmp((const char*)g_session + 0x8f48, (const char*)Engine_FirstListEntry() + 8))
                    SendSessionNotice460();
            }
        }
        Net_DropOldMessages(g_session, 0, F<uint8_t*>(msg, 8), 1);
        break;
    }
    }
    return 1;
}
RT3_REPLACE(0x46f4a0, NetMsg_SessionEvent);

// 0x46f870. Message in EAX; returns with ECX as its local slot holds.
__declspec(naked) void Net_ApplyCash() {
    __asm {
        push ecx
        push esp
        push eax
        call Net_ApplyCashImpl
        add esp, 8
        pop ecx
        ret
    }
}
RT3_REPLACE(0x46f870, Net_ApplyCash);

// 0x46f8f0. Position in EAX, key on the stack.
__declspec(naked) void Companies_ForEntry() {
    __asm {
        push [esp + 4]
        push eax
        call Companies_ForEntryImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x46f8f0, Companies_ForEntry);

// 0x46f960. Message in EAX, sender on the stack.
__declspec(naked) int32_t Chat_HandleCommand() {
    __asm {
        push dword ptr [esp + 4]
        push eax
        call Chat_HandleCommandImpl
        ret 4
    }
}
RT3_REPLACE(0x46f960, Chat_HandleCommand);

// 0x470210 (network handler). A chat message from `from` that is not a
// command goes to the lobby chat (no world) or the message log (format
// 0x5ce18c with the sender's name). Returns 1.
int32_t __fastcall NetMsg_Chat(uint8_t* m, uint8_t* from) {
    int32_t r;
    __asm {
        push from
        mov eax, m
        call Chat_HandleCommand
        mov r, eax
    }
    if (r != 1) return 1;
    char text[0x200];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(text, (const char*)0x5ce18c, from + 8, F<const char*>(m, 8));
    if (!g_world) Lobby_AddChatText(text);
    else Log_Add(g_messageLog, 0, text, 5, 1, 0);
    return 1;
}
RT3_REPLACE(0x470210, NetMsg_Chat);

// 0x470290 (net handler). Scenario info from the host (0x109 bytes at msg
// +8): selects the scenario when its name (+0x11) differs (keeping its
// result); while joining (0x6d127c == 3) a failed selection waits (state 3,
// 0x6d1284), a found one moves to state 4 and acknowledges with 0x53
// (entry +0x5c bit 2). A selected scenario's info replaces 0x6cec7c except
// its title (+0x44), which becomes 0x6ce670 and refreshes the dialog when
// it was newly selected. Then reseeds from +0x83 and sets 0x6ce9bc when the
// name contains 0x5c9cf8.
int32_t __fastcall NetMsg_ScenarioInfo(uint8_t* msg) {
    const uint8_t* info = F<uint8_t*>(msg, 8);
    const char* name = (const char*)info + 0x11;
    int32_t changed = 0, found = 1;
    if (Stricmp((const char*)g_newGame + 0x11, name)) {
        found = Scenario_SelectByName(name);
        changed = 1;
    }
    if (g_6d127c == 3) {
        if (found != 1) {
            g_6d1284 = 1;
            g_6d1280 = 3;
            return 1;
        }
        g_6d1280 = 4;
        F<uint32_t>(Engine_FirstListEntry(), 0x5c) |= 2;
        Net_Send(g_session, 0, nullptr, 0x53, 0, nullptr, 1, 0);
    }
    if (found == 1) {
        if (g_6d40dc) GameSpy_Pump();
        char title[0x80];
        strcpy(title, (const char*)g_newGame + 0x44);
        memcpy(g_newGame, info, 0x109);
        strcpy((char*)g_newGame + 0x44, title);
        if (changed == 1) {
            strcpy((char*)g_newGame + 0x44, (const char*)0x6ce670);
            MpDialog_Refresh(1);
        }
    }
    SetSeed(F<int32_t>(g_newGame, 0x83));
    g_6ce9bc =
        ((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)((const char*)g_newGame + 0x11, (const char*)0x5c9cf8) ? 1 : 0;
    return 1;
}
RT3_REPLACE(0x470290, NetMsg_ScenarioInfo);

// 0x4703f0 (network handler). The game resumes: unpaused (+0x19, +0x66ae),
// date stamped, the players' colours taken from the queue entries, the
// clock (0x6ce98c) reset and the tooltip cleared; the host resumes speed 3.
// Returns 1.
int32_t NetMsg_Resume() {
    F<int32_t>(g_world, 0x19) = 0;
    Obj_StampDate(g_session, 0, 1);
    F<int32_t>(g_world, 0x66ae) = 1;
    Cursor_Set(g_cursorManager, 0, 1, 0, 1);
    for (uint8_t* e = *(uint8_t**)(uintptr_t)Net_LockQueue(); e; e = *(uint8_t**)e) {
        const char* name = (const char*)e + 8;
        int32_t k;
        __asm {
            push ebx
            mov ebx, name
            call Profiles_IndexOf
            pop ebx
            mov k, eax
        }
        if (k == 0xff) continue;
        Collection* c = g_players;
        ((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)k, 0)))[0x15c] = e[0x2690];
    }
    Net_UnlockQueue();
    g_6ce98c = ClockMs();
    if (g_windowManager) Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1);
    if (g_6d40dc) World_SetSpeed(g_world, 0, 3, 1, 0);
    return 1;
}
RT3_REPLACE(0x4703f0, NetMsg_Resume);

// 0x4704d0 (network handler). With +0x50 set the session's minute follows
// the next queued part and the message is sent on (0xc, flags 0x11).
int32_t __fastcall NetMsg_Forward0c(uint8_t* m, uint8_t* from) {
    if (!F<int32_t>(from, 0x50)) return 1;
    uint8_t* part = Net_CountMessagePart(g_session, 0, 1);
    uint8_t* s = g_session;
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
    Net_Send(g_session, 0, nullptr, 0xc, F<int32_t>(m, 0xc), F<const uint8_t*>(m, 8), 0x11, 0);
    return 1;
}
RT3_REPLACE(0x4704d0, NetMsg_Forward0c);

// 0x470530 (network handler). Sets the game speed from the packet (d[0],
// announce d[1]) unless syncing; a pause change adjusts the pause count
// 0x6cd8e8 and 0x5f2b38 becomes 60. Returns 1.
int32_t __fastcall NetMsg_SetSpeed(uint8_t* m) {
    if (F<int32_t>(m, 0xc) <= 0 || g_6cd91c) return 1;
    const int8_t* d = F<const int8_t*>(m, 8);
    if (!World_SetSpeed(g_world, 0, d[0], 0, d[1])) return 1;
    if (!F<int32_t>(g_world, 0x19)) ++g_6cd8e8;
    else if (g_6cd8e8) --g_6cd8e8;
    g_5f2b38 = 0x3c;
    return 1;
}
RT3_REPLACE(0x470530, NetMsg_SetSpeed);

// 0x470590 (network handler). Sends the next queued outgoing packet (the
// first engine entry's queue +0x64) to the host (sess +0x8f48) under serial
// d[0] and frees it. Returns 1.
int32_t __fastcall NetMsg_SendQueued(uint8_t* m) {
    const uint32_t serial = *F<const uint32_t*>(m, 8);
    uint32_t* q = F<uint32_t*>((uint8_t*)Engine_FirstListEntry(), 0x64);
    uint8_t* p = (uint8_t*)(uintptr_t)q[0];
    if (p) {
        const uint32_t next = F<uint32_t>(p, 0);
        q[0] = next;
        if ((uint32_t)(uintptr_t)p == q[1]) q[1] = next;
        --q[2];
        uint8_t* s = g_session;
        Net_Send(s, 0, (const char*)(s + 0x8f48), F<uint16_t>(p, 0x14), F<int32_t>(p, 0xc), F<const uint8_t*>(p, 8), F<uint32_t>(p, 0x10), (int32_t)serial);
        NetPacket_Free(g_session, 0, p);
    }
    return 1;
}
RT3_REPLACE(0x470590, NetMsg_SendQueued);

// 0x4705f0. Looks up the session's entry for o's id (*o[8]) in the first texture
// entry's hash (+0x60) and, when present, hands it to 0x521790 (a no-op returning 1).
int32_t __fastcall Net_TouchEntry(uint8_t* o) {
    uint8_t* first = (uint8_t*)Engine_FirstListEntry();
    int32_t h = Hash_Get(F<HashMap*>(first, 0x60), 0, *F<uint32_t*>(o, 8));
    if (h) Return1_521790(hook::As<void*>(g_6d40d0), 0, h, (int32_t)(uintptr_t)(g_session + 0x8f48));
    return 1;
}
RT3_REPLACE(0x4705f0, Net_TouchEntry);

// 0x470630 (network handler). A building message is applied (phase 1):
// refused ones go back to the sender empty, accepted ones to everyone (0x13)
// with the session's minute following the next queued part. Returns 1.
int32_t __fastcall NetMsg_Building(uint8_t* m, uint8_t* from) {
    uint8_t* d = F<uint8_t*>(m, 8);
    int32_t r;
    __asm {
        push esi
        mov esi, d
        push 1
        call BuildingMsg_Apply
        pop esi
        mov r, eax
    }
    if (r == -1) {
        Net_Send(g_session, 0, (const char*)(from + 8), 0x13, 0, nullptr, 0x21, 0);
        return 1;
    }
    uint8_t* part = Net_CountMessagePart(g_session, 0, 1);
    uint8_t* s = g_session;
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
    Net_Send(g_session, 0, nullptr, 0x13, 0x1c, d, 0x11, 0);
    return 1;
}
RT3_REPLACE(0x470630, NetMsg_Building);

// 0x4706b0 (net handler). Building placement {x, y, angle, company, type,
// editor, key} (msg +8): for a known company, a placeable site gets a new
// building (0x62b26c) at the angle, queued for its rise (ring 0x6ce808, 32
// entries, while trains run and it is a rising kind) or set down, and placed
// (removed again when that fails). Unless it exists afterwards the host
// sends its CRC (0x6e). Empty messages report string 0xb7c. The placer's
// tool is released either way.
int32_t __fastcall NetMsg_PlaceBuilding(uint8_t* m) {
    if (F<int32_t>(m, 0xc) <= 0) {
        Net_QueueText(g_session, 0, GetString(0xb7c), 1, 0, 0, 0);
        PlayerOnLeft452(nullptr);
        return 1;
    }
    uint32_t* d = F<uint32_t*>(m, 8);
    if (!CompanyForEntry452(Collection_PositionOf(g_companies, 0, d[3], 0), d[6])) {
        PlayerOnLeft452(d);
        return 1;
    }
    const float x = *(const float*)&d[0], y = *(const float*)&d[1];
    uint32_t id = 0xffffffff;
    if (Sites_CanPlace(g_buildingTypes, 0, d[4], x, y, *(const float*)&d[2], (int32_t)d[3], (int32_t)d[5], nullptr, 0, 1, -1, 0)) {
        g_6ce9ac = Trains_AnyActive();
        uint8_t* type = (uint8_t*)Collection_Get(g_buildingTypes, 0, d[4]);
        id = Buildings_Add(g_buildings, 0, type, x, y, 0, (int32_t)d[3], 0);
        if (Collection_IsValid(g_buildings, 0, id)) {
            uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
            SceneObj_SetAngle2(b, 0, d[2]);
            if (g_6ce9ac && Obj_StateKindIs4(b)) {
                int32_t ring = g_6ce9b0;
                uint32_t* e = (uint32_t*)(0x6ce808 + ring * 12);
                e[0] = id, e[1] = d[0], e[2] = d[1];
                g_6ce9b0 = (ring + 1) % 32;
            } else {
                Building_MoveTo(b, 0, x, y, 0, 0.0f);
            }
            SceneObj_ResetAttachmentLists(b);
            uint8_t* placed = (uint8_t*)Collection_Get(g_buildings, 0, id);
            if (!Building_Place(placed, 0, x, y, d[3])) {
                Collection* all = g_buildings;
                ((void(__thiscall*)(Collection*, uint32_t))(*(void***)all)[2])(all, id);
            }
        }
        g_6ce9ac = 0;
    }
    if (!Collection_IsValid(g_buildings, 0, id) && g_6d40dc) {
        F<int32_t>(g_session, 0x8f38) = DateToMinutes((const GameDate*)(g_world + 0xd));
        uint32_t crc = Crc32((const uint8_t*)d, 0x1c);
        Net_Send(g_session, 0, nullptr, 0x6e, 4, (const uint8_t*)&crc, 0x13, 0);
    }
    PlayerOnLeft452(d);
    return 1;
}
RT3_REPLACE(0x4706b0, NetMsg_PlaceBuilding);

// 0x4708c0 (network handler 0x14). A client's track-building message p
// (data +8, size +0xc) from `from`: the host queues it (TrackMsg_Apply
// phase 1); when accepted the session's minute (+0x8f38) follows the next
// queued part and the message goes to everyone as 0x15, else the sender
// gets an empty 0x15 back. Always 1.
int32_t __fastcall NetMsg_TrackBuildRequest(uint8_t* p, uint8_t* from) {
    uint8_t* const data = F<uint8_t*>(p, 8);
    if (TrackMsg_ApplyImpl(data, 1) == 1) {
        uint8_t* s = g_session;
        uint8_t* part = Net_CountMessagePart(s, 0, 1);
        uint8_t* s2 = g_session;
        F<int32_t>(s2, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
        Net_Send(g_session, 0, nullptr, 0x15, F<int32_t>(p, 0xc), data, 0x19, 0);
    } else {
        Net_Send(g_session, 0, (const char*)(from + 8), 0x15, 0, nullptr, 0x21, 0);
    }
    return 1;
}
RT3_REPLACE(0x4708c0, NetMsg_TrackBuildRequest);

// 0x470950 (net handler). Track build result {player, company b4, ...} (msg
// +8, 0x4b bytes): empty ones report string 0xe35. For a known company a
// non-forced (+4 +0xb bit 8) build is applied with the world's +0x4aab
// cleared; done, the host (0x6d40dc, outside 0x6ce9b4) bills 0x6ce998 to the
// company (0x6b, applied locally too) and counts it (+0x987c, +0x9880 by
// 0x70 per pending); refused or forced, the host sends its CRC (0x6e). The
// sender's own result resets its tool (0x6d1a8c +0x83) and cursor.
int32_t __fastcall NetMsg_TrackBuildResult(uint8_t* m) {
    uint8_t* gs;
    uint8_t* d;
    if (F<int32_t>(m, 0xc) <= 0) {
        Net_QueueText(g_session, 0, GetString(0xe35), 1, 0, 0, 0);
        goto reset;
    }
    d = F<uint8_t*>(m, 8);
    {
        const uint32_t pos = Collection_PositionOf(g_companies, 0, d[4], 0);
        const uint32_t key = F<uint32_t>(d, 0);
        uint8_t* company;
        __asm {
            push key
            mov eax, pos
            call Companies_ForEntry
            mov company, eax
        }
        if (company) {
            bool refused = (F<uint8_t*>(m, 4)[0xb] & 8) != 0;
            if (!refused) {
                const int32_t saved = F<int32_t>(g_world, 0x4aab);
                F<int32_t>(g_world, 0x4aab) = 0;
                int32_t r;
                __asm {
                    xor eax, eax
                    mov ecx, d
                    call TrackMsg_Apply
                    mov r, eax
                }
                F<int32_t>(g_world, 0x4aab) = saved;
                refused = r != 1;
                if (!refused && g_6d40dc && !g_6ce9b4) {
                    F<int32_t>(g_session, 0x8f38) = DateToMinutes((const GameDate*)(g_world + 0xd));
                    uint8_t bill[8];
                    *(uint32_t*)bill = hook::As<uint32_t>(g_6ce998);
                    bill[4] = d[4];
                    Net_Send(g_session, 0, nullptr, 0x6b, 5, bill, 0x13, 0);
                    ++F<int32_t>(g_session, 0x987c);
                    gs = g_session;
                    const int32_t add = Obj_Sum987c(gs) * 0x70;
                    gs = g_session;
                    F<int32_t>(gs, 0x9880) += add;
                    uint8_t* b = bill;
                    __asm {
                        mov eax, b
                        call Net_ApplyCash
                    }
                }
            }
            if (refused && g_6d40dc) {
                F<int32_t>(g_session, 0x8f38) = DateToMinutes((const GameDate*)(g_world + 0xd));
                uint32_t crc = Crc32(d, 0x4b);
                Net_Send(g_session, 0, nullptr, 0x6e, 4, (const uint8_t*)&crc, 0x13, 0);
            }
        }
    }
    if ((uint32_t)Global6d40e4() != F<uint32_t>(d, 0)) return 1;
reset:
    F<uint32_t>(g_6d1a8c, 0x83) = 0;
    Cursor_Set(g_cursorManager, 0, 4, 0, 0);
    return 1;
}
RT3_REPLACE(0x470950, NetMsg_TrackBuildResult);

 // namespace

// 0x470b20 (network message 0x16, at the host). A train purchase request:
// checked (QUIRK: the validate pass always succeeds) and passed on to
// everyone as message 0x17, the session (+0x8f38) noting the message date;
// a failed check (never) would answer the sender with an empty 0x17.
int32_t __fastcall NetMsg_BuyTrainRequest(uint8_t* msg, uint8_t* from) {
    uint8_t* data = *(uint8_t* const*)(msg + 8);
    if (NetTrainCreate484(1, data) == -1) {
        Net_Send(g_session, 0, (const char*)from + 8, 0x17, 0, nullptr, 0x21, 0);
        return 1;
    }
    uint8_t* s = g_session;
    if (uint8_t* part = Net_CountMessagePart(s, 0, 1))
        F<int32_t>(g_session, 0x8f38) = DateToMinutes((const GameDate*)part);
    else
        F<int32_t>(g_session, 0x8f38) = 0;
    Net_Send(g_session, 0, nullptr, 0x17, F<int32_t>(msg, 0xc), data, 0x19, 0);
    return 1;
}
RT3_REPLACE(0x470b20, NetMsg_BuyTrainRequest);

// 0x470bb0 (network message 0x17). A train purchase: empty means refused
// (string 0xb7e); else, when company +0x10 belongs to the sender profile
// (+0xc), the train is built here. The host sends the checksum of the
// request (0x6e) when it could not be built; the buyer's train list (0x6d3ac8)
// is told the new train (-1 on failure).
int32_t __fastcall NetMsg_BuyTrain(uint8_t* msg) {
    int32_t id = -1;
    if (F<int32_t>(msg, 0xc) <= 0) {
        Net_QueueText(g_session, 0, GetString(0xb7e), 1, 0, 0, 0);
        SetGlobal6d1aa4(id);
        return 1;
    }
    uint8_t* data = *(uint8_t* const*)(msg + 8);
    const uint32_t pos = Collection_PositionOf(g_companies, 0, F<uint32_t>(data, 0x10), 0);
    if (CompanyForEntry484(pos, F<uint32_t>(data, 0xc))) {
        uint8_t orders[0x28];
        Cursor33_Init(orders);
        bool ok = true;
        for (int32_t i = 0; i < F<int32_t>(data, 0); ++i) {
            uint8_t* e = data + 0x14 + i * 0x33;
            if (!(e[0x28] & 0x80) && !Collection_IsValid(g_stations, 0, F<uint16_t>(e, 0x29))) {
                ok = false;
                break;
            }
            TrainOrders_Insert(orders, 0, i, e);
        }
        if (ok) {
            id = (int32_t)Train_Create(F<uint32_t>(data, 4), orders, 1, F<int32_t>(data, 0x10));
            if (Collection_IsValid(g_trains, 0, (uint32_t)id)) {
                uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, (uint32_t)id);
                if (F<int32_t>(data, 8) >= 0) Train_SetRunning(t, 0, F<int32_t>(data, 8));
                if (Obj_City41(t)) {
                    SceneObj_GetPos((uint8_t*)Obj_City41(t), 0, (uint32_t*)0x6ce6f0, (uint32_t*)0x6ce6f4, (uint32_t*)0x6ce988);
                } else {
                    g_6ce6f0 = 0xbf800000;
                    g_6ce6f4 = 0xbf800000;
                    g_6ce988 = 0xbf800000;
                }
            }
        }
        if (!Collection_IsValid(g_trains, 0, (uint32_t)id) && g_6d40dc) {
            F<int32_t>(g_session, 0x8f38) = DateToMinutes((const GameDate*)(g_world + 0xd));
            uint32_t crc[1];
            crc[0] = Crc32(data, 0x14);
            Net_Send(g_session, 0, nullptr, 0x6e, 4, (const uint8_t*)crc, 0x13, 0);
        }
    }
    if (Global6d40e4() == F<int32_t>(data, 0xc)) SetGlobal6d1aa4(id);
    return 1;
}
RT3_REPLACE(0x470bb0, NetMsg_BuyTrain);

// 0x470d60 (network handler). The first engine list entry takes d[0]
// (+0x2588), the seed d[2] (also 0x5f2b6c) and the finance sync. Returns 1.
int32_t __fastcall NetMsg_SyncFinance(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    if (uint8_t* e = (uint8_t*)Engine_FirstListEntry()) {
        F<int32_t>(e, 0x2588) = F<int32_t>(d, 0);
        g_5f2b6c = F<int32_t>(d, 8);
        SetSeed(F<int32_t>(d, 8));
        Finance_SyncMessage(d, 0, nullptr);
    }
    return 1;
}
RT3_REPLACE(0x470d60, NetMsg_SyncFinance);

// 0x470da0 (network handler). A new company request (d) that does not clash
// goes to everyone (0x1a, flags 0x19, the session's minute following the next
// queued part); a clash sends its code back to the sender. Returns 1.
int32_t __fastcall NetMsg_NewCompany(uint8_t* m, uint8_t* from) {
    uint8_t* d = F<uint8_t*>(m, 8);
    int32_t r = Companies_ClashCall(d);
    if (r) {
        Net_Send(g_session, 0, (const char*)(from + 8), 0x1a, 4, (const uint8_t*)&r, 0x21, 0);
        return 1;
    }
    uint8_t* part = Net_CountMessagePart(g_session, 0, 1);
    uint8_t* s = g_session;
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
    Net_Send(g_session, 0, nullptr, 0x1a, F<int32_t>(m, 0xc), d, 0x19, 0);
    return 1;
}
RT3_REPLACE(0x470da0, NetMsg_NewCompany);

// 0x470e30 (network handler). Answer to this peer's company request: a
// direct reply (4 bytes) or the broadcast (founded when it does not clash;
// for this peer's own entry the clash code). A clash code shows 0xb7f (1)
// / 0xb80 with the arrow cursor. Returns 1.
int32_t __fastcall NetMsg_CompanyAnswer(uint8_t* m) {
    int32_t r;
    if (F<int32_t>(m, 0xc) == 4) {
        r = *F<const int32_t*>(m, 8);
    } else {
        int32_t* d = F<int32_t*>(m, 8);
        r = Companies_ClashCall(d);
        if (!r) {
            Msg_FoundCompany(d);
            return 1;
        }
        uint8_t* rec = (uint8_t*)Collection_Get(g_players, 0, (uint32_t)d[0]);
        if (Stricmp445((const char*)rec + 8, (const char*)Engine_FirstListEntry() + 8)) return 1;
    }
    if (r) {
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
        Net_QueueText(g_session, 0, GetString(r == 1 ? 0xb7f : 0xb80), 1, 0, 0, 0);
    }
    return 1;
}
RT3_REPLACE(0x470e30, NetMsg_CompanyAnswer);

RT3_CASH_CHEAT(0x470ed0, NetMsg_CheatCashUp, 0x4c4b40, 0x4a989680)

RT3_CASH_CHEAT(0x470fa0, NetMsg_CheatCashDown, -100000, 0xc7c35000)

int32_t __fastcall Net_Relay1e(const uint8_t* msg) { return Net_RelayStamped(msg, 0x1e); }
RT3_REPLACE(0x471070, Net_Relay1e);

int32_t __fastcall Net_Relay20(const uint8_t* msg) { return Net_RelayStamped(msg, 0x20); }
RT3_REPLACE(0x4710d0, Net_Relay20);

int32_t __fastcall Net_Relay22(const uint8_t* msg) { return Net_RelayStamped(msg, 0x22); }
RT3_REPLACE(0x471130, Net_Relay22);

int32_t __fastcall Net_Relay24(const uint8_t* msg) { return Net_RelayStamped(msg, 0x24); }
RT3_REPLACE(0x471190, Net_Relay24);

// More stamped relays (0x4711f0..0x473220): message type and flags differ only.
int32_t __fastcall Net_Relay26(const uint8_t* msg) { return Net_RelayStamped(msg, 0x26); }
RT3_REPLACE(0x4711f0, Net_Relay26);

int32_t __fastcall Net_Relay28(const uint8_t* msg) { return Net_RelayStamped(msg, 0x28); }
RT3_REPLACE(0x471250, Net_Relay28);

int32_t __fastcall Net_Relay2a(const uint8_t* msg) { return Net_RelayStamped(msg, 0x2a); }
RT3_REPLACE(0x4712b0, Net_Relay2a);

int32_t __fastcall Net_Relay2c(const uint8_t* msg) { return Net_RelayStamped(msg, 0x2c); }
RT3_REPLACE(0x471310, Net_Relay2c);

int32_t __fastcall Net_Relay2e(const uint8_t* msg) { return Net_RelayStamped(msg, 0x2e); }
RT3_REPLACE(0x471370, Net_Relay2e);

int32_t __fastcall Net_Relay30(const uint8_t* msg) { return Net_RelayStamped(msg, 0x30); }
RT3_REPLACE(0x4713d0, Net_Relay30);

int32_t __fastcall Net_Relay32(const uint8_t* msg) { return Net_RelayStamped(msg, 0x32); }
RT3_REPLACE(0x471430, Net_Relay32);

int32_t __fastcall Net_Relay34(const uint8_t* msg) { return Net_RelayStamped(msg, 0x34); }
RT3_REPLACE(0x471490, Net_Relay34);

int32_t __fastcall Net_Relay36(const uint8_t* msg) { return Net_RelayStamped(msg, 0x36); }
RT3_REPLACE(0x4714f0, Net_Relay36);

int32_t __fastcall Net_Relay38(const uint8_t* msg) { return Net_RelayStamped(msg, 0x38); }
RT3_REPLACE(0x471550, Net_Relay38);

int32_t __fastcall Net_Relay3a(const uint8_t* msg) { return Net_RelayStamped(msg, 0x3a); }
RT3_REPLACE(0x4715b0, Net_Relay3a);

RT3_TRAINFWD(0x471610, NetMsg_TrainForward3c, 0x3c)

RT3_TRAINMSG(0x471680, NetMsg_TrainRunning, Train_SetRunning(t, 0, d[2]))

RT3_TRAINFWD(0x4716c0, NetMsg_TrainForward3e, 0x3e)

RT3_TRAINMSG(0x471730, NetMsg_TrainCurrentOrder, TrainOrders_SetCurrent(t, 0, d[2], 1))

RT3_TRAINFWD(0x471780, NetMsg_TrainForward40, 0x40)

RT3_TRAINMSG(0x4717f0, NetMsg_TrainRemoveOrder, TrainOrders_Remove(t, 0, d[2]))

RT3_TRAINFWD(0x471830, NetMsg_TrainForward42, 0x42)

// 0x4718a0 (network handler). Inserts order d+8 into train d[1] before its
// current one; a train that can no longer run stops (state 0xa, or 0x13 for
// a city kind 2 train that could run without electric track). Returns 1.
int32_t __fastcall NetMsg_InsertOrder(uint8_t* m) {
    if (F<int32_t>(m, 0xc) <= 0) return 1;
    uint8_t* d = F<uint8_t*>(m, 8);
    if (!Collection_IsValid(g_trains, 0, F<uint32_t>(d, 4))) return 1;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(d, 4));
    TrainOrders_Insert(t, 0, F<int32_t>((uint8_t*)Identity(t), 4), d + 8);
    if (Orders_Drivable((uint32_t*)t, 0, t, -1, 0, 0, -1)) return 1;
    if (Obj_CityKindIs2(t) && Orders_Drivable((uint32_t*)t, 0, t, -1, 0, 1, -1)) Train_SetState(t, 0, 0x13, 0.0f);
    else Train_SetState(t, 0, 0xa, 0.0f);
    return 1;
}
RT3_REPLACE(0x4718a0, NetMsg_InsertOrder);

RT3_TRAINFWD(0x471950, NetMsg_TrainForward44, 0x44)

RT3_PEER_ANSWER(0x4719c0, Peer_AnswerRequest49, 8, 0x49, 0xc)

 // namespace

// 0x471a30 (net handler). Takeover vote request {requester, company b4, target
// b5, vote mask +8} from `from` (EDX): the requester itself and players
// without shares of the target just answer (0x49; own bit k + 0x10 set, and
// the yes bit k for the requester); a shareholder is asked (string 0xb81,
// 30 s, answered by 0x4719c0 with a copy of the request).
int32_t __fastcall NetMsg_TakeoverVote(uint8_t* m, uint8_t* from) {
    uint32_t* req = F<uint32_t*>(m, 8);
    const int32_t k = MyProfileIndex451();
    if ((uint32_t)Global6d40e4() == req[0]) {
        req[2] |= (1u << ((k + 0x10) & 31)) | (1u << (k & 31));
    } else {
        Collection* players = g_players;
        uint8_t* p = (uint8_t*)Collection_Get(players, 0, Collection_IdAt(players, 0, (uint32_t)k, 0));
        const uint8_t* b = (const uint8_t*)req;
        if (p && F<int32_t>(p, 0x1e1) && F<int32_t>(p, 0x15d + b[5] * 4) > 0) {
            uint8_t* target = (uint8_t*)Collection_Get(g_companies, 0, b[5]);
            uint8_t* buyer = (uint8_t*)Collection_Get(g_companies, 0, b[4]);
            FormatText((char*)0x6ce700, GetString(0xb81), (const char*)buyer + 4, (const char*)target + 4, 0, 0, 0, 0, 0, 0);
            uint8_t* q = Net_QueueText(g_session, 0, (const char*)0x6ce700, 3, 0x7530, 0, 0x4719c0);
            uint32_t* copy = ((uint32_t*(__cdecl*)(size_t))0x53b070)(0xc);
            copy[0] = req[0], copy[1] = req[1], copy[2] = req[2];
            F<uint32_t*>(q, 0x1c) = copy;
            return 1;
        }
        req[2] |= 1u << ((k + 0x10) & 31);
    }
    Net_Send(g_session, 0, (const char*)from + 8, 0x49, F<int32_t>(m, 0xc), (const uint8_t*)req, 1, 0);
    return 1;
}
RT3_REPLACE(0x471a30, NetMsg_TakeoverVote);

RT3_PEER_COLLECT(0x471b90, Peer_CollectAnswer47, 8, 0xc, 0x47)

// 0x471c90 (network handler). Rebuilds the merger vote (0x2a bytes: d[4],
// d[5], d+6, then present / for marks with mask d+8) and resolves it.
// Returns 1.
int32_t __fastcall NetMsg_MergerVote(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    uint8_t st[0x2a] = {};
    F<uint16_t>(st, 8) = F<uint16_t>(d, 6);
    Vote_BuildState(st, d, F<uint32_t>(d, 8), 0xa, 0x1a);
    Obj_CopyTo6d10d8(st);
    Merger_ResolveVote();
    return 1;
}
RT3_REPLACE(0x471c90, NetMsg_MergerVote);

RT3_PEER_ANSWER(0x471d50, Peer_AnswerRequest4d, 6, 0x4d, 0xa)

// 0x471dc0 (network handler). A merger request (d, 10 bytes): this peer's
// own one is answered "yes" at once (both bits, message 0x4d); others ask
// the player (string 0xb82 with the companies, 30 s, answer callback
// 0x471d50 owning a copy of the request). Returns 1.
int32_t __fastcall NetMsg_MergerRequest(uint8_t* m, uint8_t* from) {
    uint8_t* req = F<uint8_t*>(m, 8);
    const char* me = (const char*)Engine_FirstListEntry() + 8;
    int32_t k;
    __asm {
        push ebx
        mov ebx, me
        call Profiles_IndexOf
        pop ebx
        mov k, eax
    }
    if (Global6d40e4() == F<int32_t>(req, 0)) {
        F<uint32_t>(req, 6) |= 1u << (k + 0x10) | 1u << k;
        Net_Send(g_session, 0, (const char*)(from + 8), 0x4d, F<int32_t>(m, 0xc), req, 1, 0);
        return 1;
    }
    uint8_t* player = (uint8_t*)Collection_Get(g_players, 0, req[5]);
    uint8_t* company = (uint8_t*)Collection_Get(g_companies, 0, req[4]);
    FormatText((char*)0x6ce700, GetString(0xb82), (const char*)(company + 4), (const char*)(player + 8), 0, 0, 0, 0, 0, 0);
    uint8_t* q = Net_QueueText(g_session, 0, (const char*)0x6ce700, 3, 0x7530, 0, 0x471d50);
    uint8_t* copy = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0xa);
    F<uint32_t>(copy, 0) = F<uint32_t>(req, 0);
    F<uint32_t>(copy, 4) = F<uint32_t>(req, 4);
    F<uint16_t>(copy, 8) = F<uint16_t>(req, 8);
    F<uint8_t*>(q, 0x1c) = copy;
    return 1;
}
RT3_REPLACE(0x471dc0, NetMsg_MergerRequest);

RT3_PEER_COLLECT(0x471ed0, Peer_CollectAnswer4b, 6, 0xa, 0x4b)

// 0x471fd0 (network handler). Builds the proxy fight state from the
// packet (two 16-bit values d[4] / d[5]; per player slot "present" and
// "for" marks: inactive (+0x1e1 / +4 clear) or voting in mask d+6) and
// resolves it. Returns 1.
int32_t __fastcall NetMsg_ProxyFight(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    uint32_t state[10] = {};
    uint8_t* st = (uint8_t*)state;
    F<uint16_t>(st, 4) = d[4];
    F<uint16_t>(st, 6) = d[5];
    for (int32_t i = 0; i < Collection_Count(g_players); ++i) {
        st[8 + i] = 1;
        Collection* c = g_players;
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!F<int32_t>(e, 0x1e1) || !F<int32_t>(e, 4) || (F<uint32_t>(d, 6) & (1u << i))) st[0x18 + i] = 1;
    }
    Obj_CopyTo6d1a08(state);
    ProxyFight_Resolve();
    return 1;
}
RT3_REPLACE(0x471fd0, NetMsg_ProxyFight);

// 0x472080 (network handler). Asks for the state (message 0x6f) unless
// syncing. Returns 1.
int32_t NetMsg_RequestState() {
    if (g_6d40dc && !g_6cd91c) Net_Send(g_session, 0, nullptr, 0x6f, 0, nullptr, 1, 0);
    return 1;
}
RT3_REPLACE(0x472080, NetMsg_RequestState);

int32_t NetMsg_Copy9058() {
    if (g_6d40dc) {
        uint8_t* s = g_session;
        F<int32_t>(s, 0x9054) = F<int32_t>(s, 0x9058);
    }
    return 1;
}
RT3_REPLACE(0x4720b0, NetMsg_Copy9058);

// 0x4720d0 (network handler). A peer asks to join (packet name, detail
// level +0x20): code 2 when the name is taken, else a free slot by detail
// level or claim (with / without a company); with none left the lobby is
// broadcast instead. Not while 0x6d40dc is clear. Returns 1.
// QUIRK: the second claim's result is not kept.
int32_t __fastcall NetMsg_JoinRequest(uint8_t* m, uint8_t* from) {
    uint8_t* d = F<uint8_t*>(m, 8);
    int32_t r = 0xff;
    if (!g_6d40dc) return 1;
    if (Obj_NameContains(g_session, 0, (const char*)d)) {
        r = 2;
    } else {
        r = DetailLevelFor(0, F<int32_t>(d, 0x20));
        if (r == 0xff) {
            r = NewGame_ClaimSlot((int)(uintptr_t)from, 1);
            if (r == 0xff) {
                NewGame_ClaimSlot((int)(uintptr_t)from, 0);
                Net_Send(g_session, 0, nullptr, 2, 0x109, g_newGame, 3, 0);
                return 1;
            }
        }
    }
    Net_Send(g_session, 0, (const char*)(from + 8), 0x52, 4, (const uint8_t*)&r, 0x23, 0);
    return 1;
}
RT3_REPLACE(0x4720d0, NetMsg_JoinRequest);

int32_t __fastcall NetMsg_Store9860(uint8_t* m) {
    F<int32_t>(g_session, 0x9860) = *F<int32_t*>(m, 8);
    return 1;
}
RT3_REPLACE(0x4721a0, NetMsg_Store9860);

// 0x4721c0 (network handler). Copies the name d+8 into record d[1]
// (position) of 0x6ceb9c. Returns 1.
int32_t __fastcall NetMsg_SetName(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    Collection* c = g_players;
    uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, F<uint32_t>(d, 4), 0));
    strcpy((char*)e + 8, (const char*)d + 8);
    return 1;
}
RT3_REPLACE(0x4721c0, NetMsg_SetName);

// 0x472200 (network handler). Record d[0] (position) of 0x6ceb9c gets d[1]
// at +0x1e1. Returns 1.
int32_t __fastcall NetMsg_Set1e1(uint8_t* m) {
    const uint32_t* d = F<const uint32_t*>(m, 8);
    Collection* c = g_players;
    F<uint32_t>((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, d[0], 0)), 0x1e1) = d[1];
    return 1;
}
RT3_REPLACE(0x472200, NetMsg_Set1e1);

// 0x472230 (network handler). Releases the 16 reserved slots of the session
// (+0x9864, 0xff free): their records in 0x6ceb9c lose +0x1e1 and get
// +0x293. Returns 1.
int32_t NetMsg_ReleaseSlots() {
    uint8_t* slots = g_session + 0x9864;
    for (int32_t i = 0; i < 0x10; ++i) {
        if (slots[i] == 0xff) continue;
        Collection* c = g_players;
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, slots[i], 0));
        F<int32_t>(e, 0x1e1) = 0;
        e[0x293] = 1;
        slots[i] = 0xff;
    }
    return 1;
}
RT3_REPLACE(0x472230, NetMsg_ReleaseSlots);

// 0x472290 (network handler). Answers whether scenario `name` (packet) can
// be played: its id, or 0xff when it allows fewer players (capped at 4)
// than the lobby has or too few free entries. Returns 1.
int32_t __fastcall NetMsg_CheckScenario(uint8_t* m, uint8_t* from) {
    int32_t r = Scenario_SelectByName(F<const char*>(m, 8));
    if (r) {
        char path[0x100];
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(path, (const char*)0x5c89e4, F<const char*>(m, 8));
        uint8_t* b = ((uint8_t*(__cdecl*)(size_t))0x5a125d)(0x403c2);
        Block_Load(path, b, 0);
        int32_t players = F<int32_t>(b, 0x3b6);
        if (players >= 4) players = 4;
        F<int32_t>(b, 0x3b6) = players;
        const int32_t inLobby = F<uint16_t>(g_newGame, 0x79);
        if (inLobby > players || Lobby_CountFreeEntries(b) + F<uint16_t>(g_newGame, 0x79) > F<int32_t>(b, 0x3ae)) r = 0xff;
        ((void(__cdecl*)(void*))0x5a1145)(b);
    }
    Net_Send(g_session, 0, (const char*)(from + 8), 0x5a, 4, (const uint8_t*)&r, 1, 0);
    return 1;
}
RT3_REPLACE(0x472290, NetMsg_CheckScenario);

int32_t __fastcall Net_Relay5c(const uint8_t* msg) { return Net_RelayStamped(msg, 0x5c, 0x11); }
RT3_REPLACE(0x472360, Net_Relay5c);

// 0x4723c0 (network handler). Electrifies company d[0]'s track; peers newer
// than version 100 also get the cost ({cost, company} through the cash
// update). Returns 1.
int32_t __fastcall NetMsg_Electrify(uint8_t* m) {
    if (F<int32_t>(m, 0xc) <= 0) return 1;
    const uint32_t* d = F<const uint32_t*>(m, 8);
    if (!Collection_IsValid(g_companies, 0, d[0])) return 1;
    if ((double)Net_CommonVersion() <= Fl(0x5c8c28)) {
        TrackNet_ElectrifyCompany(g_tracks, 0, d[0], 0, nullptr, nullptr);
        return 1;
    }
    uint32_t pkt[2];
    pkt[0] = 0;
    ((uint8_t*)pkt)[4] = *(const uint8_t*)d;
    TrackNet_ElectrifyCompany(g_tracks, 0, d[0], 0, (float*)pkt, nullptr);
    uint32_t* p = pkt;
    __asm {
        mov eax, p
        call Net_ApplyCash
    }
    return 1;
}
RT3_REPLACE(0x4723c0, NetMsg_Electrify);

// 0x472450. Relays msg as a stamped 0x5e (flags 0x11), unless its payload names
// (+8) a known player: then an empty 0x5e goes to the player named at to + 8.
int32_t __fastcall Net_Relay5e(const uint8_t* msg, const char* to) {
    const uint8_t* data = F<const uint8_t*>((void*)msg, 8);
    if (Net_HasPlayer(g_session, 0, (const char*)data + 8)) {
        Net_Send(g_session, 0, to + 8, 0x5e, 0, nullptr, 0x21, 0);
        return 1;
    }
    return Net_RelayStamped(msg, 0x5e, 0x11);
}
RT3_REPLACE(0x472450, Net_Relay5e);

// 0x4724e0 (network handler). Renames player d[1] (name d+8, also its
// texture d[0], whose key +0x48 follows when it has +0x50, and the host
// name sess +0x8f48); this peer's own request ends the busy count; an empty
// reply queues string 0xb83. Returns 1.
int32_t __fastcall NetMsg_RenamePlayer(uint8_t* m) {
    if (F<int32_t>(m, 0xc) > 0) {
        uint8_t* d = F<uint8_t*>(m, 8);
        const char* name = (const char*)d + 8;
        Collection* c = g_players;
        strcpy((char*)Collection_Get(c, 0, Collection_IdAt(c, 0, F<uint32_t>(d, 4), 0)) + 8, name);
        uint8_t* t = Textures_Find(F<int32_t>(d, 0));
        strcpy((char*)t + 8, name);
        if (F<int32_t>(t, 0x50)) {
            F<uint32_t>(t, 0x48) = Crc32String(name, 1);
            strcpy((char*)g_session + 0x8f48, name);
        }
        Net_UnlockQueueArg(F<int32_t>(d, 0));
        if (Global6d40e4() != F<int32_t>(d, 0)) return 1;
    } else {
        Net_QueueText(g_session, 0, GetString(0xb83), 1, 0, 0, 0);
    }
    if (--g_6d10a0 == 0) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    return 1;
}
RT3_REPLACE(0x4724e0, NetMsg_RenamePlayer);

int32_t __fastcall Net_Relay60(const uint8_t* msg) { return Net_RelayStamped(msg, 0x60, 0x11); }
RT3_REPLACE(0x4725e0, Net_Relay60);

 // namespace

// 0x472640 (network handler). Sets player d[1]'s (an active one) colour
// byte (+0x15c) to d[8]; this peer's own failed request restores the old
// colour (0x622090) and queues string 0xb84. Either way the busy count
// 0x6d10a0 drops. Returns 1.
int32_t __fastcall NetMsg_PlayerColour(uint8_t* m) {
    if (F<int32_t>(m, 0xc) > 0) {
        uint8_t* d = F<uint8_t*>(m, 8);
        const int32_t mine = Global6d40e4() == F<int32_t>(d, 0);
        Collection* c = g_players;
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, F<uint32_t>(d, 4), 0));
        if (F<int32_t>(e, 0x1e1)) e[0x15c] = d[8];
        if (!mine) return 1;
    } else {
        uint8_t* p = (uint8_t*)Obj_Ref25(g_world);
        p[0x15c] = hook::As<uint8_t>(g_622090);
        Net_QueueText(g_session, 0, GetString(0xb84), 1, 0, 0, 0);
    }
    g_622090 = -1;
    if (--g_6d10a0 == 0) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    return 1;
}
RT3_REPLACE(0x472640, NetMsg_PlayerColour);

// 0x472700 / 0x472870 (network handlers). A company name (d+8) / colour (+8)
// request: refused (empty 0x62 / 0x64 to the sender) when a company already
// has it, else sent to everyone. Return 1.
int32_t __fastcall NetMsg_CompanyNameRequest(uint8_t* m, uint8_t* from) {
    uint8_t* d = F<uint8_t*>(m, 8);
    Collection* cos = g_companies;
    for (int32_t i = 0; i < Collection_Count(g_companies); ++i) {
        cos = g_companies;
        if (!Stricmp447((const char*)Collection_Get(cos, 0, Collection_IdAt(cos, 0, (uint32_t)i, 0)) + 4, (const char*)d + 8)) {
            Net_Send(g_session, 0, (const char*)(from + 8), 0x62, 0, nullptr, 0x21, 0);
            return 1;
        }
    }
    Session_StampNextPart();
    Net_Send(g_session, 0, nullptr, 0x62, F<int32_t>(m, 0xc), d, 0x11, 0);
    return 1;
}
RT3_REPLACE(0x472700, NetMsg_CompanyNameRequest);

// 0x4727e0 (network handler). Renames the company entry of player d[1]
// (key d[0], name d+8); for this peer's own request a failure queues
// string 0xb85; either way the busy count 0x6d10a0 drops (cursor back at
// 0). Returns 1.
int32_t __fastcall NetMsg_RenameEntry(uint8_t* m) {
    int32_t mine = 0, found = 0;
    if (F<int32_t>(m, 0xc) > 0) {
        uint8_t* d = F<uint8_t*>(m, 8);
        const int32_t key = F<int32_t>(d, 0);
        if (Global6d40e4() == key) mine = 1;
        const int32_t pos = F<int32_t>(d, 4);
        uint8_t* e;
        __asm {
            push key
            mov eax, pos
            call Companies_ForEntry
            mov e, eax
        }
        if (e) {
            StrCopyN((char*)e + 4, (const char*)d + 8, 0x32);
            found = 1;
        }
        if (!mine) return 1;
        if (found) goto done;
    }
    Net_QueueText(g_session, 0, GetString(0xb85), 1, 0, 0, 0);
done:
    if (--g_6d10a0 == 0) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    return 1;
}
RT3_REPLACE(0x4727e0, NetMsg_RenameEntry);

int32_t __fastcall NetMsg_CompanyColourRequest(uint8_t* m, uint8_t* from) {
    uint8_t* d = F<uint8_t*>(m, 8);
    for (int32_t i = 0; i < Collection_Count(g_companies); ++i) {
        Collection* cos = g_companies;
        if (F<int32_t>((uint8_t*)Collection_Get(cos, 0, Collection_IdAt(cos, 0, (uint32_t)i, 0)), 0x37) == F<int32_t>(d, 8)) {
            Net_Send(g_session, 0, (const char*)(from + 8), 0x64, 0, nullptr, 0x21, 0);
            return 1;
        }
    }
    Session_StampNextPart();
    Net_Send(g_session, 0, nullptr, 0x64, F<int32_t>(m, 0xc), d, 0x11, 0);
    return 1;
}
RT3_REPLACE(0x472870, NetMsg_CompanyColourRequest);

// 0x472940 (network handler). Like 0x4727e0 for the company colour (+0x37 of
// player d[1]'s entry, old value 0x622094, failure string 0xb86).
int32_t __fastcall NetMsg_CompanyColour(uint8_t* m) {
    int32_t mine = 0, found = 0;
    if (F<int32_t>(m, 0xc) > 0) {
        uint8_t* d = F<uint8_t*>(m, 8);
        const int32_t key = F<int32_t>(d, 0);
        if (Global6d40e4() == key) mine = 1;
        const int32_t pos = F<int32_t>(d, 4);
        uint8_t* e;
        __asm {
            push key
            mov eax, pos
            call Companies_ForEntry
            mov e, eax
        }
        if (e) {
            found = 1;
            F<uint32_t>(e, 0x37) = F<uint32_t>(d, 8);
        }
        if (!mine) return 1;
        if (found) goto done;
    }
    if (uint8_t* c = (uint8_t*)Obj_Company21(g_world)) F<int32_t>(c, 0x37) = g_622094;
    Net_QueueText(g_session, 0, GetString(0xb86), 1, 0, 0, 0);
done:
    g_622094 = -1;
    if (--g_6d10a0 == 0) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    return 1;
}
RT3_REPLACE(0x472940, NetMsg_CompanyColour);

int32_t __fastcall Net_Relay74(const uint8_t* msg) { return Net_RelayStamped(msg, 0x74, 0x11); }
RT3_REPLACE(0x4729f0, Net_Relay74);

// 0x472a50 (network handler). Deletes vehicle d[0] (with 0x6ce9a8 set when
// every peer has version 1.02). Returns 1.
int32_t __fastcall NetMsg_DeleteVehicle(uint8_t* m) {
    const uint32_t id = *F<const uint32_t*>(m, 8);
    if (Collection_IsValid(g_buildings, 0, id)) {
        if (Net_AllHaveVersion(1.02f)) g_6ce9a8 = 1;
        Collection* c = g_buildings;
        // EDX holds the vtable, as in the original's call.
        ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)c)[2])(c, (int)(uintptr_t) * (void***)c, id);
        g_6ce9a8 = 0;
    }
    return 1;
}
RT3_REPLACE(0x472a50, NetMsg_DeleteVehicle);

int32_t __fastcall Net_Relay72(const uint8_t* msg) { return Net_RelayStamped(msg, 0x72, 0x11); }
RT3_REPLACE(0x472aa0, Net_Relay72);

// 0x472b00 (net handler). City buildings from the host (msg +8: u16, count,
// then 0x12-byte {city, type, x, y, angle} records; a single one with a
// nonzero first word is a 0x14-byte {u16, u16, type, x, y, angle} block):
// with the site checks relaxed (0x62b2fc +0x9c and unbounded box) and the
// cities' build counter (0x62bae0 +0x88) held, places each placeable one and
// re-weights its city. 0x6ce9a8 marks peers from v1.02 during it. QUIRK: the
// single block checks the city by its whole first dword (count in the high
// word).
int32_t __fastcall NetMsg_PlaceBuildings(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    if (!F<uint16_t>(d, 2)) return 1;
    if (Net_AllHaveVersion(BitsF(0x3f828f5c))) g_6ce9a8 = 1;
    auto relax = [](int32_t on) {
        uint8_t* s = hook::As<uint8_t*>(g_buildingTypes);
        F<int32_t>(s, 0x9c) = on;
        F<uint32_t>(s, 0xa0) = 0x497423f0, F<uint32_t>(s, 0xa4) = 0xc97423f0;
        F<int32_t>(s, 0xa8) = 0xf423f, F<int32_t>(s, 0xac) = (int32_t)0xfff0bdc1;
        F<int32_t>(s, 0xb0) = 0xf423f, F<int32_t>(s, 0xb4) = (int32_t)0xfff0bdc1;
    };
    auto unbound = []() {
        uint8_t* s = hook::As<uint8_t*>(g_buildingTypes);
        F<int32_t>(s, 0xa8) = 0xf423f, F<int32_t>(s, 0xac) = (int32_t)0xfff0bdc1;
        F<int32_t>(s, 0xb0) = 0xf423f, F<int32_t>(s, 0xb4) = (int32_t)0xfff0bdc1;
    };
    auto place = [](uint32_t type, float x, float y, float angle, uint32_t city) {
        if (!Sites_CanPlace(g_buildingTypes, 0, type, x, y, angle, 0, 0, nullptr, 0, 1, -1, 0)) return;
        uint8_t* t = (uint8_t*)Collection_Get(g_buildingTypes, 0, type);
        Buildings_Add(g_buildings, 0, t, x, y, *(const uint32_t*)&angle, 0, 1);
        if (Collection_IsValid(g_cities, 0, city))
            Obj_SumBuildingWeights(Collection_Get(g_cities, 0, city));
    };
    if (F<uint16_t>(d, 2) == 1 && F<uint16_t>(d, 0)) {
        uint32_t blk[5];
        memcpy(blk, d, 0x14);
        ++F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88);
        relax(1);
        place(blk[1], *(const float*)&blk[2], *(const float*)&blk[3], *(const float*)&blk[4], blk[0]);
        --F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88);
        unbound();
    } else {
        ++F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88);
        relax(1);
        const uint8_t* r = d + 4;
        for (int32_t k = 0; k < F<uint16_t>(d, 2); ++k, r += 0x12)
            place(*(const uint32_t*)(r + 2), *(const float*)(r + 6), *(const float*)(r + 0xa), *(const float*)(r + 0xe), *(const uint16_t*)r);
        --F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88);
        unbound();
    }
    uint8_t* s = hook::As<uint8_t*>(g_buildingTypes);
    F<int32_t>(s, 0x9c) = 0;
    F<uint32_t>(s, 0xa0) = 0x497423f0;
    F<uint32_t>(s, 0xa4) = 0xc97423f0;
    g_6ce9a8 = 0;
    return 1;
}
RT3_REPLACE(0x472b00, NetMsg_PlaceBuildings);

// 0x472db0 / 0x472dc0 (network handlers). The packet (+8) goes to the cash /
// train position update (both take it in EAX). Always 1.
int32_t __fastcall NetMsg_Cash(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    __asm {
        mov eax, d
        call Net_ApplyCash
    }
    return 1;
}
RT3_REPLACE(0x472db0, NetMsg_Cash);

int32_t __fastcall NetMsg_TrainPosition(uint8_t* m) {
    uint8_t* d = F<uint8_t*>(m, 8);
    __asm {
        mov eax, d
        call Net_ApplyTrainPosition
    }
    return 1;
}
RT3_REPLACE(0x472dc0, NetMsg_TrainPosition);

// 0x472dd0 (network handler). Player d[0] (another one) is named in the
// world's status line (+0x4b47, string 0xf4e; empty when gone) and +0x4a73
// is set. Returns 1.
int32_t __fastcall NetMsg_PlayerStatus(uint8_t* m) {
    const uint32_t* d = F<const uint32_t*>(m, 8);
    if (!g_world) return 1;
    const uint32_t id = d[0];
    if ((uint32_t)Obj_Get25(g_world) == id) return 1;
    if (Collection_IsValid(g_players, 0, id)) {
        uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, d[0]);
        char text[0x100];
        FormatText(text, GetString(0xf4e), (const char*)(p + 8), 0, 0, 0, 0, 0, 0, 0);
        strcpy((char*)g_world + 0x4b47, text);
    } else {
        g_world[0x4b47] = 0;
    }
    F<int32_t>(g_world, 0x4a73) = 1;
    return 1;
}
RT3_REPLACE(0x472dd0, NetMsg_PlayerStatus);

// 0x472ea0. Message in ECX, other player in EDX.
__declspec(naked) void Msg_MarkSeen() {
    __asm {
        push edx
        push ecx
        call Msg_MarkSeenImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x472ea0, Msg_MarkSeen);

int32_t __fastcall Net_Relay76(const uint8_t* msg) { return Net_RelayStamped(msg, 0x76, 0x11); }
RT3_REPLACE(0x472f10, Net_Relay76);

// 0x472f70 (network message 0x76). The AI of company *data buys a train
// (0x409830, on the train chain).
int32_t __fastcall NetMsg_AiBuyTrain(const uint8_t* msg) {
    const uint32_t id = **(const uint32_t* const*)(msg + 8);
    if (Collection_IsValid(g_companies, 0, id)) {
        Collection_Get(g_companies, 0, id);
        ((void(__fastcall*)(void*))0x409830)(Collection_Get(g_companies, 0, id));
    }
    return 1;
}
RT3_REPLACE(0x472f70, NetMsg_AiBuyTrain);

int32_t __fastcall Net_Relay78(const uint8_t* msg) { return Net_RelayStamped(msg, 0x78, 0x11); }
RT3_REPLACE(0x472fb0, Net_Relay78);

// 0x473010 (network handler). Bulldozes at (d[2], d[3]) radius 0.5 for
// company d[1] (event 0xba / 0x808 when something went); this peer's own
// request (d[0]) resets the bulldozer cursor (0x15, sound 0x39). Returns 1.
int32_t __fastcall NetMsg_Bulldoze(uint8_t* m) {
    const uint32_t* d = F<const uint32_t*>(m, 8);
    if (Collection_IsValid(g_companies, 0, d[1])) {
        float cost;
        char msg[0x3e4];
        float x, y;
        memcpy(&x, &d[2], 4), memcpy(&y, &d[3], 4);
        if (Map_Bulldoze(g_map, 0, x, y, 0.5f, d[1], 0, &cost, msg))
            Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
    }
    if ((uint32_t)Global6d40e4() == d[0]) {
        F<int32_t>(g_6d1a8c, 0x83) = 0;
        Cursor_Set(g_cursorManager, 0, 0x15, 0, 0);
        Ui_PlaySound(0x39, 0, 0x3f000000);
    }
    return 1;
}
RT3_REPLACE(0x473010, NetMsg_Bulldoze);

int32_t __fastcall Net_Relay7a(const uint8_t* msg) { return Net_RelayStamped(msg, 0x7a, 0x11); }
RT3_REPLACE(0x4730c0, Net_Relay7a);

 // namespace

// 0x473120 (network handler). Company d[1] absorbs company d[2]; the
// cursor resets when the message is from this peer (0x6d40e4). Returns 1.
int32_t __fastcall NetMsg_AbsorbCompany(uint8_t* m) {
    const uint32_t* d = F<const uint32_t*>(m, 8);
    if (Collection_IsValid(g_companies, 0, d[1]))
        Company_Absorb((uint8_t*)Collection_Get(g_companies, 0, d[1]), 0, d[2], 0);
    if ((uint32_t)Global6d40e4() == d[0]) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    return 1;
}
RT3_REPLACE(0x473120, NetMsg_AbsorbCompany);

int32_t __fastcall Net_Relay7c(const uint8_t* msg) { return Net_RelayStamped(msg, 0x7c, 0x11); }
RT3_REPLACE(0x473180, Net_Relay7c);

// 0x4731e0 (network handler). Dissolves company d[1] if it exists.
int32_t __fastcall NetMsg_DissolveCompany(uint8_t* m) {
    const uint32_t* d = F<const uint32_t*>(m, 8);
    if (Collection_IsValid(g_companies, 0, d[1])) Company_Dissolve((uint8_t*)Collection_Get(g_companies, 0, d[1]));
    return 1;
}
RT3_REPLACE(0x4731e0, NetMsg_DissolveCompany);

int32_t __fastcall Net_Relay7e(const uint8_t* msg) { return Net_RelayStamped(msg, 0x7e, 0x11); }
RT3_REPLACE(0x473220, Net_Relay7e);

// 0x473280. Resets the multiplayer session state (0x6ce808 / 0x6ce290 blocks and
// flags, seed 7, timeout 0x5f2b4c = 1e6) and registers the game's message handlers
// on the lobby object (0x6cd8d8): the default 0x46f4a0 and 129 id -> handler pairs.
RT3_NOINLINE void MpSession_Reset() {
    memset((void*)0x6ce808, 0, 0x60 * 4);
    g_6ce9a0 = 0;
    g_5f2b4c = 0x49741cf0;
    static const uint32_t zero1[] = { 0x6ce998, 0x6ce99c, 0x6ce98c, 0x6ce990, 0x6ce9a8, 0x6ce9ac, 0x6ce9b0 };
    for (uint32_t a : zero1) hook::Global<int32_t>(a) = 0;
    g_5f2b6c = 7;
    SetSeed(7);
    void* lobby = hook::As<void*>(g_session);
    memset((void*)0x6ce290, 0, 0xe6 * 4);
    static const uint32_t zero2[] = { 0x6ce800, 0x6ce628, 0x6ce9b4, 0x6ce9b8, 0x6ce9bc };
    for (uint32_t a : zero2) hook::Global<int32_t>(a) = 0;
    g_6ce630 = 0;
    g_6ce9c0 = 0;
    g_6ce9c4 = 0;
    g_6ce9c8 = 0;
    Obj_Set8(lobby, 0, 0x46f4a0);
    static const uint32_t handlers[][2] = {
{ 0x1, 0x470210 },
{ 0x2, 0x470290 },
{ 0x3, 0x46c3c0 },
{ 0x4, 0x46c420 },
{ 0x5, 0x46c760 },
{ 0x6, 0x46c770 },
{ 0x7, 0x46c7c0 },
{ 0x8, 0x46c810 },
{ 0x9, 0x46c870 },
{ 0x68, 0x46c840 },
{ 0xa, 0x4703f0 },
{ 0xb, 0x4704d0 },
{ 0xc, 0x470530 },
{ 0xd, 0x46c8c0 },
{ 0xe, 0x46c990 },
{ 0xf, 0x470590 },
{ 0x10, 0x4705f0 },
{ 0x11, 0x46c390 },
{ 0x12, 0x470630 },
{ 0x13, 0x4706b0 },
{ 0x14, 0x4708c0 },
{ 0x15, 0x470950 },
{ 0x16, 0x470b20 },
{ 0x17, 0x470bb0 },
{ 0x18, 0x46c9d0 },
{ 0x19, 0x470da0 },
{ 0x1a, 0x470e30 },
{ 0x1b, 0x46c9e0 },
{ 0x1c, 0x470ed0 },
{ 0x1d, 0x471070 },
{ 0x1e, 0x46c9f0 },
{ 0x1f, 0x4710d0 },
{ 0x20, 0x46ca10 },
{ 0x21, 0x471130 },
{ 0x22, 0x46ca30 },
{ 0x23, 0x471190 },
{ 0x24, 0x46ca50 },
{ 0x25, 0x4711f0 },
{ 0x26, 0x46ca70 },
{ 0x27, 0x471250 },
{ 0x28, 0x46ca90 },
{ 0x29, 0x4712b0 },
{ 0x2a, 0x46cab0 },
{ 0x2b, 0x471310 },
{ 0x2c, 0x46cad0 },
{ 0x2d, 0x471370 },
{ 0x2e, 0x46caf0 },
{ 0x2f, 0x4713d0 },
{ 0x30, 0x46cb10 },
{ 0x31, 0x471430 },
{ 0x32, 0x46cb30 },
{ 0x33, 0x471490 },
{ 0x34, 0x46cb50 },
{ 0x35, 0x4714f0 },
{ 0x36, 0x46cb70 },
{ 0x37, 0x471550 },
{ 0x38, 0x46cb90 },
{ 0x39, 0x4715b0 },
{ 0x3a, 0x46cbb0 },
{ 0x3b, 0x471610 },
{ 0x3c, 0x471680 },
{ 0x3d, 0x4716c0 },
{ 0x3e, 0x471730 },
{ 0x3f, 0x471780 },
{ 0x40, 0x4717f0 },
{ 0x41, 0x471830 },
{ 0x42, 0x4718a0 },
{ 0x43, 0x471950 },
{ 0x44, 0x46cbd0 },
{ 0x46, 0x46cbf0 },
{ 0x47, 0x471c90 },
{ 0x48, 0x471a30 },
{ 0x49, 0x471b90 },
{ 0x4a, 0x46cc60 },
{ 0x4b, 0x471fd0 },
{ 0x4c, 0x471dc0 },
{ 0x4d, 0x471ed0 },
{ 0x4e, 0x46ccd0 },
{ 0x4f, 0x472080 },
{ 0x50, 0x4720b0 },
{ 0x51, 0x4720d0 },
{ 0x52, 0x46cd10 },
{ 0x53, 0x46cd30 },
{ 0x54, 0x46ce10 },
{ 0x55, 0x4721a0 },
{ 0x56, 0x4721c0 },
{ 0x57, 0x472200 },
{ 0x58, 0x472230 },
{ 0x59, 0x472290 },
{ 0x5a, 0x46ce90 },
{ 0x5b, 0x472360 },
{ 0x5c, 0x4723c0 },
{ 0x5d, 0x472450 },
{ 0x5e, 0x4724e0 },
{ 0x5f, 0x4725e0 },
{ 0x60, 0x472640 },
{ 0x61, 0x472700 },
{ 0x62, 0x4727e0 },
{ 0x63, 0x472870 },
{ 0x64, 0x472940 },
{ 0x65, 0x46cf10 },
{ 0x66, 0x46cf40 },
{ 0x67, 0x470d60 },
{ 0x69, 0x46cf70 },
{ 0x6a, 0x470fa0 },
{ 0x6b, 0x472db0 },
{ 0x6c, 0x472dc0 },
{ 0x6d, 0x472dd0 },
{ 0x6e, 0x472ea0 },
{ 0x6f, 0x46cce0 },
{ 0x70, 0x46cfa0 },
{ 0x71, 0x472aa0 },
{ 0x72, 0x472b00 },
{ 0x73, 0x4729f0 },
{ 0x74, 0x472a50 },
{ 0x75, 0x472f10 },
{ 0x76, 0x472f70 },
{ 0x77, 0x472fb0 },
{ 0x78, 0x473010 },
{ 0x79, 0x4730c0 },
{ 0x7a, 0x473120 },
{ 0x7b, 0x473180 },
{ 0x7c, 0x4731e0 },
{ 0x7d, 0x473220 },
{ 0x7e, 0x46cfb0 },
{ 0x7f, 0x46cfe0 },
{ 0x80, 0x46d090 },
{ 0x81, 0x46d1d0 },
{ 0x83, 0x46d230 }
    };
    for (auto& h : handlers) Obj_MapInsertNew(lobby, 0, (uint16_t)h[0], (int32_t)h[1]);
}
RT3_REPLACE(0x473280, MpSession_Reset);

// 0x473a60. Registers the ten multiplayer sync-check categories (names
// 0x5ce40c.., checksum callbacks) with the lobby.
RT3_NOINLINE void RegisterSyncChecks() {
    static const uint32_t kChecks[10][2] = {
        { 0x5ce40c, 0x46de80 }, { 0x5ce404, 0x46e030 }, { 0x5ce3fc, 0x46e250 }, { 0x5ce3f0, 0x46de40 },
        { 0x5ce3e8, 0x46dd10 }, { 0x5ce3e0, 0x46dbd0 }, { 0x5ce3d4, 0x46db90 }, { 0x5ce3cc, 0x46dac0 },
        { 0x5ce3bc, 0x46db50 }, { 0x5ce3ac, 0x46db10 },
    };
    uint8_t* lobby = g_session;
    for (int i = 0; i < 10; ++i) Obj_AddMessage(lobby, 0, i + 1, 0, (int32_t)kChecks[i][1], (const char*)kChecks[i][0]);
}
RT3_REPLACE(0x473a60, RegisterSyncChecks);

// 0x473b40. Sends the n (0x6ce9a0) pending 18-byte records at 0x6cd990 to the
// player named at +0x8f48 as message 0x71 {word 0, word n, records}, reliably,
// and clears them.
void Net_SendPendingRecords() {
    int32_t n = g_6ce9a0;
    if (n <= 0) return;
    int32_t size = n * 0x12 + 4;
    uint8_t* buf = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)((size_t)size);
    F<uint16_t>(buf, 0) = 0;
    F<uint16_t>(buf, 2) = (uint16_t)g_6ce9a0;
    memcpy(buf + 4, (const void*)0x6cd990, (size_t)g_6ce9a0 * 0x12);
    uint8_t* s = g_session;
    Net_Send(s, 0, (const char*)s + 0x8f48, 0x71, size, buf, 5, 0);
    g_6ce9a0 = 0;
    ((void(__cdecl*)(void*))0x5a1145)(buf);
}
RT3_REPLACE(0x473b40, Net_SendPendingRecords);

// 0x473bf0. Applies the queued building moves (32 slots at 0x6ce808: {building,
// x, z}) once no train is running (checked once; with 16 or more queued the
// moves go ahead anyway). Buildings of kind 3 (type +0xb9) stay put; every
// applied slot is cleared.
RT3_NOINLINE void __fastcall Buildings_ApplyQueuedMoves() {
    uint32_t* q = (uint32_t*)0x6ce808;
    int32_t queued = 0;
    for (int k = 0; k < 32; ++k)
        if (q[k * 3]) ++queued;
    int32_t busy = queued >= 16 ? 0 : -1;
    for (int k = 0; k < 32; ++k) {
        uint32_t* e = q + k * 3;
        if (!e[0]) continue;
        if (busy == -1) busy = Trains_AnyActive();
        if (busy) return;
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, e[0]);
        uint8_t* type = (*(uint8_t*(__fastcall**)(uint8_t*))(*(uint8_t**)b + 0x80))(b);
        if (type[0xb9] != 3) Building_MoveTo(b, 0, BitsF(e[1]), BitsF(e[2]), 0, 0.0f);
        e[0] = 0;
    }
}
RT3_REPLACE(0x473bf0, Buildings_ApplyQueuedMoves);

RT3_NOINLINE void* __fastcall ReturnThis473cc0(void* o) { return o; }
RT3_REPLACE(0x473cc0, ReturnThis473cc0);

RT3_NOINLINE void __fastcall Nop473cd0(void*) { __asm nop }
RT3_REPLACE(0x473cd0, Nop473cd0);

// 0x473ce0. Initialises a 0x187-byte news entry: kind, title (0x78),
// text (0xf0), stamps the current date and the next serial 0x6cea48.
RT3_NOINLINE void __fastcall News_Init(uint8_t* e, int, int32_t kind, const char* title, const char* text, int32_t a, int32_t b) {
    F<int32_t>(e, 0) = kind;
    StrCopyN((char*)e + 0x19, title, 0x78);
    StrCopyN((char*)e + 0x92, text, 0xf0);
    F<int32_t>(e, 0x183) = b;
    F<int32_t>(e, 8) = DateToMinutes((const GameDate*)(g_world + 0xd));
    F<uint16_t>(e, 0xc) = F<uint16_t>(g_world, 0xd);
    e[0xe] = g_world[0xf];
    e[0x10] = g_world[0x11];
    e[0x18] = 0;
    F<int32_t>(e, 4) = a;
    F<int32_t>(e, 0x14) = g_6cea48;
    g_6cea48++;
}
RT3_REPLACE(0x473ce0, News_Init);

// 0x473d70 / 0x473da0. Collection subclass 0x5ce418 (inline 0x187-byte records, 10 + 5).
RT3_NOINLINE Collection* __fastcall Collection5ce418_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce418;
    Collection_Init(c, 0, 1, 0x187, 10, 5, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x473d70, Collection5ce418_Construct);

RT3_NOINLINE void __fastcall Collection5ce418_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce418;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x473da0, Collection5ce418_Destruct);

// 0x473dc0. Record with the highest serial +0x14 (ID from +0); 0 if none.
RT3_NOINLINE void* __fastcall Table_Newest14(Collection* t) {
    uint32_t sel = 0;
    int32_t best = -1;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) > best) {
            sel = F<uint32_t>(r, 0);
            best = F<int32_t>(r, 0x14);
        }
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x473dc0, Table_Newest14);

// 0x473e20. Any record with byte +0x18 clear and +4 set.
RT3_NOINLINE int32_t __fastcall Table_AnyPending(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (!r[0x18] && F<int32_t>(r, 4)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x473e20, Table_AnyPending);

// 0x473e70. Record with the lowest serial +0x14 (skipping flagged +0x18 ones unless `all`); 0 if none.
RT3_NOINLINE void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all) {
    uint32_t sel = 0;
    int32_t best = 0x7fffffff;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) >= best) continue;
        if (!all && r[0x18]) continue;
        sel = F<uint32_t>(r, 0);
        best = F<int32_t>(r, 0x14);
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x473e70, Table_Oldest14);

// 0x473ee0 / 0x473f30. Record whose +0x14 is v - 1 / v + 1; 0 if none.
RT3_NOINLINE void* __fastcall Table_FindField14Prev(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) == v - 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x473ee0, Table_FindField14Prev);

RT3_NOINLINE void* __fastcall Table_FindField14Next(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) == v + 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x473f30, Table_FindField14Next);

// 0x473f80. Adds a news entry, first dropping the oldest while more than 25.
RT3_NOINLINE uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b) {
    while (Collection_Count(t) > 0x19) {
        uint32_t id = F<uint32_t>(Table_Oldest14(t, 0, 1), 0);
        void** vt = *(void***)t;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(t, id);
    }
    uint8_t local[0x188] = {};  // original: uninitialised stack (overwritten by News_Init)
    ReturnThis473cc0(local);
    uint32_t id = Collection_Allocate(t, 0, local, 0, 0, 0);
    News_Init((uint8_t*)Collection_Get(t, 0, id), 0, (int32_t)id, title, text, a, b);
    Nop473cd0(local);
    return id;
}
RT3_REPLACE(0x473f80, News_Add);

}  // namespace rt3
