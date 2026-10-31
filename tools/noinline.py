import re, glob, sys
h = 'src/hook/hook.h'
s = open(h, encoding='utf-8').read()
if 'RT3_NOINLINE' not in s:
    marker = '#define RT3_REPLACE(addr, fn) \\'
    assert marker in s
    s = s.replace(marker, '// Keeps a function out of line so the UI difftests can redirect every call to\n'
                  '// it (tests/test_ui.cpp patches the entry point).\n#define RT3_NOINLINE __declspec(noinline)\n\n' + marker, 1)
    open(h, 'w', encoding='utf-8').write(s)
names = ["Font_DrawTextBox", "Glyph_Queue", "Ui_Line", "Window_AddChild", "Window_SendEvent5", "Scrollbar_Construct", "ListBox_Construct",
         "EditBox_Construct", "Control_Construct", "TimerWidget_Construct", "String_Assign", "TextLink_Draw", "Company_Stat", "Table_Entry",
         "Global6d07b4Set2", "Terrain_HeightAt", "Scene_ProjectNormalised", "Fog_ParamsAt", "Camera_InBounds", "Scroll_Visible", "Camera_DetailFactor", "Slider_StepA", "Slider_StepB", "ClockMs", "Ticker_PickNext", "ModelLibrary_Init", "Pack_AcquireCollision", "Emitter_Reset", "Lobby_AddChatText", "Log_Add", "Widget_SetVtblRet", "Window_Load", "Obj_BuildingStateActive", "RulesDialog_FillConditionTypes", "ConditionDialog_ShowValue", "RulesDialog_UpdateButtons4ff7", "ConditionDialog_Store", "RulesDialog_ShowLimits", "Widget_FindChild", "Obj_CityRef", "Obj_City41", "NameTable_ContainsName", "Obj_NamedEntry40", "HandleTable_Acquire", "List_SyncScrollbar"]
names += sys.argv[1:]
for f in glob.glob('src/**/*.cpp', recursive=True):
    s = open(f, encoding='utf-8').read()
    o = s
    for n in names:
        pat = re.compile(r'^((?:[A-Za-z_][\w:]*\*?\s+)+(?:__fastcall\s+|__stdcall\s+)?\*?\s*' + n + r'\([^;{]*\)\s*\{)', re.M)
        s = pat.sub(lambda m: m.group(1) if m.group(1).startswith('RT3_NOINLINE') else 'RT3_NOINLINE ' + m.group(1), s)
    if s != o:
        open(f, 'w', encoding='utf-8').write(s)
        print(f, s.count('RT3_NOINLINE') - o.count('RT3_NOINLINE'))
