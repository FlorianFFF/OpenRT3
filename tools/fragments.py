"""Ghidra split some functions into a head and a separately listed body chunk
(entered only by jumps from the head, sharing its stack frame). Such a chunk is
rewritten as part of its parent; it counts as done when the parent is.

    fragment address -> parent function address
"""
FRAGMENTS = {
    0x47e41f: 0x47e390,  # Region_ClassSupply: the per-class tail
    0x504baf: 0x504ba0,  # StationWindow_CargoHover: the body after the entry test
    0x55ed9a: 0x55ed90,  # Movie_Stop: fall-through body after the first four instructions
    0x454630: 0x454f40,  # TreeTypes_Load: tail-jumped spot table builder (TreeTypes_BuildSpots)
    0x4548c0: 0x454f40,  # TreeTypes_Load: second half of the spot table builder
    0x41e000: 0x41de60,  # epilogue Ghidra split off (a float in .data matched its address)
    0x430b00: 0x430560,  # RuleText_Expand: copy loop past Ghidra's body (matched by record values)
    0x431400: 0x431120,  # switch cases of 0x431120 (jump table 0x43159c)
    0x500020: 0x4ff510,  # epilogue of 0x4ff510 (UTF-16 resource text matched its address)
    0x55d420: 0x55e2b0,  # Scene_CreateRenderer: tail jump to an empty stub
    0x4f0400: 0x4f03f0,  # MpDialog_HandleEvent: fall-through body after the first five instructions
}
