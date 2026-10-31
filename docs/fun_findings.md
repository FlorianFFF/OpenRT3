# Fun & interesting findings

Things noticed while rebuilding RT3_mp.exe function by function. Addresses are
the original exe's; the rewrite's name is in parentheses.

## Economy & simulation

- **The economy only *looks* busy.** A region's monthly cargo economy isn't updated
  every tick. The first time it runs, the game fills a 256-entry table with
  "phase" numbers 0..13, always from seed 0x61 (the letter 'a'). Each tick only
  every 4th block whose phase equals the day mod 14 gets an update
  (0x450030 `Map_MonthlyEconomy`).
- **War cargo is gold.** In the war scenario modes, Diesel, Weapons and
  Ammunition are worth a flat 20,000 to a station (everything else 10,000),
  decided by comparing the cargo *names* as strings (0x47f910 `Station_PickCargo`).
- **Mail, Passengers and Troops are special by name too.** The cargo loader
  hard-codes those three names to set their special flags (0x41ede0
  `CargoTypes_Load`).
- **Prices decay faster the older the map is.** Up to 20 years after the start
  year, cargo prices erode faster each year, up to three times per month
  (0x42ce00 `Region_UpdateCargoEconomy`).
- **Small demand dies by dice roll.** When a region's demand for a cargo is 48 or
  less, each month it drops by 2 only if a 0..24 roll is at most half the demand
  (same function).
- **Cities claim land Voronoi-style.** Each city claims the 4x4 map blocks within
  its radius, and wins a contested block when it is closer than the current owner
  (0x421730 `Cities_AssignBlocks`).
- **Streets come in 4 flavours.** A city's street pattern variant is just
  `(x + y) & 3` of its position (0x41fb00 `City_LayStreets`).

## Vehicles

- **Planes have wanderlust.** Every tick an aircraft has a 10 in 5000 chance to pick
  a new heading (±25 units). How many times it may do that is limited and counts
  down. The nose pitch is clamped to ±π/8 (0x4578e0 `Aircraft_Fly`).
- **The track junction picker is a small maze.** It first tries the straight link,
  then the alternative route, then switches at the previous or current node.
  Switches that "turn left" the wrong way are refused (0x4ab2d0 `Train_NextNode`).
- **The track router feels its way around obstacles.** It walks the line in 0.362
  steps. Where it crosses blocked ground, it takes the middle of the blocked
  stretch and pushes it sideways, alternating left and right in half-cell steps,
  until it finds free ground. That point becomes a new waypoint. Afterwards,
  waypoints that a later one can see directly are cut out (0x49cc40
  `Track_PlanRoute`).
- **Ramps look for level ground.** To grade a ramp, the game walks back from its
  end until the ground drops below the end height *and* is level there
  (|atan(dh / 33.304)| <= 0.1). It refuses ramps where the ground never rose 175
  units, and it gives up after 290 steps (0x4a0d20 `Track_RampGrade`).

## Scenario hot-fixes

- **The exe patches specific scenarios on load.** One function (0x442c30
  `Scenario_ApplyFixes`) is a list of hand-made repairs, keyed on the scenario's
  name:
  - *Go West!* / *Orient Express*: some cities get bigger.
  - *Chicago to New York*: a city and an industry that sat in the wrong place are
    moved.
  - *France*: part of the map is repainted from region 5 to region 1 ("5th
    Republic - 1958").
  - *Central Pacific*: the "Amount of track already laid [Company Track Miles]."
    sentence is deleted from the status text in all five languages.
  - *New Beginnings*: "Paris is connected to Berlin" becomes "Your company connects
    Paris to Berlin".
  - *Germantown*: "The American" goal drops from 180,000 to 170,000.
  - *Tex-Mex*: a "25" in the status text becomes "20".
  - Many more fix event-rule states in place.
- **One hot-fix does nothing.** The "Setup P3 Welcome" repair checks that a value
  is 3 and then sets it to 3.
- **Post offices got more expensive in a patch.** For newer save versions every
  "PostOffice" building type costing 25,000 is bumped to 40,000.

## Graphics & sound

- **The sky runs on 144 keyframes per day**, one every 10 minutes of game time.
  Sun, ambient and fog colours are blended between two of them, then darkened by
  the night factor (0x54bea0 `Renderer_SetTimeOfDay`).
- **Camera haze uses √6500.** The haze factor is `(√height − k) / √6500`, clamped
  to 0..1 (0x43a940 `Camera_UpdateHaze`).
- **Water textures animate at 88 ms per frame** through an 8x8 atlas, driven by
  `GetTickCount` plus a per-quad phase (0x5679b0 `Quad_ToVertices`).
- **Weather is seven sound files.** `wea_rain.wav` and `wea_thunder_01..06.wav`,
  loaded into a fixed table of 27 sound slots (0x5373b0 `Weather_LoadSounds`).
- **Wind gusts flip randomly.** Every 32..1024 frames the gust direction may
  reverse, and the gust strength keeps drifting between two limits (0x566670
  `Particles_FrameSetup`).

## Files & localisation

- **There's a hidden language override.** A file `data2\overridelanguage` replaces
  the Windows language. Its first line can name any language folder, and control
  characters are stripped first. Otherwise 20 Spanish locale IDs map to Spanish,
  and the rest of the list is French, German, Chinese, Italian, Portuguese,
  Japanese, Polish, Russian and Hungarian; anything else is English (0x518fa0
  `Language_Select`).
- **The pack files are mounted newest-first.** The 13 data folders are scanned for
  `*.pk4` archives in descending name order. Each archive has its own index of
  `companyLogo` / `portrait` / `trainSkin` entries (0x53cb60
  `NameTable_Construct`).
- **Two building types are renamed on load.** If a building file is literally
  called "ServiceTower" or "Maintenance", the loader patches in special type ids
  0x81 and 0x9d (0x414490 `BuildingType_LoadFootprint`).
- **The cargo file sort is "creative".** The `.cty` filenames are sorted with a
  hand-written selection sort that also swaps names that compare equal (0x41ede0).
- **Saved scenario rules store names, not numbers.** Event triggers and effects are
  saved together with the *names* of the cargo, building and industry they refer
  to. On load the names are looked up again, so the rules survive the game's
  tables being renumbered (0x430d70 / 0x42db20 `EventRules_Save/Load`).

- **Weather comes in three kinds.** 40% heavy (level 255), 30% 200, 30% 230. Each
  spell lasts 40..60 days and starts some hours ahead, aligned to the clock
  (0x4351c0 `Weather_ScheduleNext`).

## System detection

- **Newer Windows is "Windows 2000".** The OS name builder only knows NT 4.0, 2000
  and XP for workstations, and tells them apart by the minor version alone. So
  Vista (6.0) reads "Windows 2000", Windows 7 (6.1) "Windows XP Professional
  Edition", and Windows 8 and newer (which report 6.2 to old programs) "Windows 2000"
  again (0x55d4c0 `Os_VersionName`).
- **The newest CPU it can name is the Pentium 4.** The CPU namer knows AMD 486 / K5
  / K6 / K6-2 / K6-3 / Athlon and Intel 486 through the Pentium 4. Anything newer
  (an Athlon 64 is family 15) gets the generic "unknown" text (0x55ce60 `Cpu_Name`).

## Multiplayer

- **129 message types.** Session setup registers a default handler plus 129
  id → handler pairs on the lobby object (0x473280 `MpSession_Reset`).

## Bugs and quirks kept for fidelity

- **`StrCopyN` writes one byte past the length** it is given (the terminator at
  `dst[n]`). A 100-byte buffer in one of the rewrites overflowed by exactly one
  byte and silently killed the sim run after saving. The only symptom was a
  truncated save trace.
- **The overview map computes a horizontal scroll clamp and then throws it away.**
  The first clamp result is immediately overwritten by the second, while the
  vertical clamp chains the two properly (0x479510 `Overview_UpdateView`).
- **The route cleanup leaks.** When the track router cuts a run of waypoints out
  of its route, it frees only the first one; the rest of the run is never freed.
  Pending waypoints are allocated with the game's `new`, but freed with the C
  runtime's `free` (0x49cc40).
- **Track style 3 reads past its table.** The style tables have three entries
  (styles 1, 2 and 4). Style 3 reads past them, and its last value comes from the
  variable that caches a corner height of the last rebuilt track piece (0x539380
  `Effect_SetStyle`).
- **D3DX writes one float too many.** `D3DXVec3TransformCoord` (0x56cd3c, the
  version linked into the exe) stores a fourth float past its 3-float output. The
  original gets away with it because the next stack slot is unused. In one rewrite
  that slot was the rotation matrix, so every rotated particle corner after the
  first came out wrong. Only a difftest caught it, since particles are never saved.
- **The collision mesh "area" isn't one.** When a collision mesh is loaded, each
  face's area is computed from a cross product that has one sign flipped in every
  component (+bc where -bc belongs). The mean of these wrong areas, plus the
  bounding box diagonal squared, times 10, becomes the mesh's size value (0x566b80
  `CollisionMesh_Load`).
- **A train-wheel/axle helper copies uninitialized stack memory** into car fields
  (0x41ce80 area). It's left as the original code, because reproducing it would
  mean copying stale return addresses.
- **The number formatter can't do small negatives.** The one- and two-decimal styles
  (0x51b700 `Num_Format`) print the truncated integer part, then the absolute value
  of the "rounded" fraction (+0.05 before truncating). For negatives that rounding
  goes the wrong way, and the integer part of anything between -1 and 0 is "0".
  So -0.5 prints as "0.4" and -1.27 as "-1.2". The money-in-thousands style rounds
  first and then drops the minus sign below $1K, so -$400 shows as "$0K".
- **The formatter localises at the end.** It builds every number with ',' and
  '.' and swaps them for the language's separators only at the outermost call,
  tracked by a recursion-depth global. Styles nest: the percentage style formats
  through the decimal style, and money-in-K formats through the plain integer
  style, which can shrink itself to K or M when it doesn't fit.
- **Two route finders, one with a pointer bug.** The track router (0x4a5280) is a
  branch-and-bound depth-first search that tries the neighbour nearest the target
  first. Its older sibling (0x4a5900) tries neighbours in slot order. When the
  older one finds no usable far end, it seeds "best next node" with the *address
  of the cost variable*, so a failed top-level search returns a stack pointer as a
  node ID. Callers check the cost (-1) first, so it never shows. The third link
  slot also skips two of the checks the first two slots get.
- **Old saves keep the old router.** 0x4a65b0 picks the track router by save
  version: games at version 0x67 or later get the smarter distance-ordered search,
  and older ones keep the plain one. Swapping routers mid-game would change AI
  routes, so the old behaviour is frozen for old saves (the scenario hot-fixes
  check the same version number).
- **Trains have gears.** 0x4856b0 shifts a train between gear -1 (reverse) and 4
  and posts "Now in gear %1" to the message log.
- **"Saving Game... 100%" is capped.** The save progress text (0x441d90) truncates
  done / total and clamps at 100, so it never shows 101%. Because it formats
  through the same number formatter, a save of 10¹² or more bytes would show the
  word "Error" (string 64) in place of the number.
- **A track reshaper that reads garbage.** 0x48f7f0 reshapes a track piece from
  cell coordinates. With its "drop onto terrain" flag set, it never fills in the
  heights and passes uninitialized stack to the reshape. The piece's slope-lighting
  mode is computed from those garbage heights before the drop fixes the geometry,
  so the lighting can depend on whatever the stack last held. The track-piece
  initialiser (0x48f2b0) has the same habit: it builds a new piece's mesh from
  never-written heights, and some mesh values survive the drop onto the terrain.
  Both stay as original code: reproducing them would mean copying stack residue.
  (0x490f80, which rotates a piece into place, also skips the heights, but there
  the drop overwrites everything that read them.)
- **The shipped game still has its leak tracker.** 0x53ae70 walks a table of
  tracked allocations and prints "Memory Leak #n from file '<file>', Line <n>,
  size <n>  ptr <p>   (Last resource loaded before allocation: <name>)" to the
  debugger and the debug log, then "Memory Leaked = N". The debug allocator's
  file/line bookkeeping is compiled into the retail exe.
- **Ghost trains.** Trains have a "ghost" state (0x4a8220): their cars turn 75%
  transparent (opacity 0.25, scene alpha 0x3f) and they are unlinked from the track
  nodes, so other trains pass through them. 0x4ad170 stops a train dead (all speeds
  zeroed) and ghosts it.
- **City connection bonuses need two cities.** 0x4358d0 pays a city's pending
  bonus only when a station in it is also track-connected to a station that lies
  in *another* city. Until then the game tells the company once per city: "...to
  win the connection bonus, this station must be connected to ANOTHER city."
- **AI players wait 45 to 270 days before acting.** A new player's first decision
  delay (0x476140) is a random 45..270 days, or 1..3 days with a fast-AI option.
  Each new player also claims one of 41 portrait slots at random. An unnamed
  player becomes "Unassigned N".
- **The scenario translator's export.** 0x4883f0 writes every scenario text as
  `<id>   "<text>"` with id = (section * 100000 + item) * 10 + sub, newlines turned
  into `\n`, and keeps a running word count, presumably for translation quotes.
- **Fast monitors were hidden.** The display mode list (0x51f540) drops every mode
  above 100 Hz, and it treats an A8R8G8B8 desktop as an unsupported format (a
  fatal error) while accepting R8G8B8, X8R8G8B8 and the two 16-bit formats. Only
  landscape modes of at least 600x400 are listed. (DesyncWatch's all_resolutions
  option NOPs out the 100 Hz cap at runtime; the rewrite reads the live
  instruction so it keeps working.)
- **Scripted history.** 0x435b50 fires fixed events on New Year's Day:
  - Headlines: steel railway bridges (1865, "cheaper than stone"); suspension
    bridges that let track cross lakes and oceans (1895); "Automobile usage
    exploding!" (1920); the Boeing 707 (1958, "passengers will flock from rails to
    air travel").
  - Bigger cargo cars: in 1850, 1900 and 1950 cars grow (10→20, 20→40, 40→80 tons).
  - 1910: towns stop accepting raw coal and oil, which must go to power plants and
    refineries.
  - Industry changes, posted only if those industries exist on the map: from 1877
    Tool and Die, Weapons and Munitions factories refuse iron (ship steel instead);
    in 1929 munitions makers switch to explosive shells; in 1940 weapons makers stop
    making fixed weapons; in 1956 "children no longer want wooden toys", so toy
    factories want plastic instead of lumber.
- **Removing a key that isn't there corrupts the heap.** A map region keeps the
  stations covering it in a small array. 0x42b230 (`Obj_RemoveKey`) allocates
  `count - 1` entries and copies over every entry that doesn't match. If the station
  isn't in the list, it writes `count` entries into the smaller buffer. On an empty
  list it frees nothing and sets the count to -1. The game gets away with it because
  a station always leaves the same catchment it joined: moving it (0x480710)
  removes it with the same radius it was added with.
- **The map size cap is 640 × 640.** The map-size dialog (0x4ea780) shows the
  chosen width × height next to a hard-coded 409,600, which is 640², as the
  largest area allowed.
- **A number formatted for nothing.** The scenario editor's settings page (0x4ca790)
  formats world +0x66ca and then ignores the result: that field gets the scenario
  text at +0x672e. The next field formats the same number again and does use it.
- **"Sticky" tooltips last about 28 hours.** The tooltip setter (0x538c70) normally
  shows a tip for 4 seconds. A sticky tip gets 99,999,999 ms, about 27.8 hours. A
  blank tip expires after 1 ms. Showing the same text again just renews it for
  2 seconds.
- **The portrait picker can loop 2000 times.** Stepping through the 41 player
  portraits (0x4ceb90) skips taken ones, but gives up after 2000 tries. Slot 0 can
  never be marked taken, so every player can pick it.
- **Building names are printf formats.** The building dialog (0x508730) copies the
  localized type name into its label with `sprintf(buf, name)`, so the name itself
  is the format string. A modded building with a `%` in its name would print
  garbage or crash.
- **Two ways to measure the same offset.** On hover (0x558760) the move callback
  gets the mouse position relative to the widget as `x - (parent + offset)`. The
  drag callback in the same function gets `x - parent + offset`, with the widget's
  own offset added instead of subtracted.
- **The resolution list's half-working duplicate filter.** The options screen
  (0x4fe4c0) lists modes from 640×480 to 1600×1200 (in a window: smaller than the
  desktop). It drops a mode when an earlier listed mode has the same size, but only
  if that earlier mode isn't 32-bit, so a size listed in 32-bit first shows up
  twice. The slider for loan amounts (0x47bea0) also has a "top" of 1,000,000,000
  when cash × aggressiveness overflows a float, then clamps it to $4M.
- **The collection walk cache can hand out ID 0.** The game's record tables speed
  up "n-th entry in sort order" lookups (0x518380) by remembering the last position
  walked. When a lookup walks from the tail, it stores the mirrored position
  (count − pos − 1) instead of the real one. A later lookup for the position right
  after that stored value then takes one step from the tail and returns 0 (no
  record). Normal front-to-back loops never trigger it, but mixing a back-half
  lookup with a loop can. The train picker (0x5158f0) would then crash on a null
  train.
- **The effect list selects a row from stack garbage.** The rules dialog's effect
  type list (0x4d4850) is a copy of the condition list (0x4d5d00) with one
  difference: it never zeroes its row counter or the "selected row" variable. The
  row it finally selects is whatever happened to be on the stack, offset by the
  rows listed. This function stays original code in the rebuild.
- **A deleted custom logo can crash the logo picker.** Companies may use a custom
  logo from the pack, stored by name. If that logo is gone, the picker (0x4e2320)
  gets "one past the last logo" as the current index. Pressing "next" only wraps
  when the index hits that exact value, so it keeps counting up past its
  availability table into whatever is on the stack. It then asks for a logo that
  doesn't exist and dereferences null. Player portraits are safe: a missing
  portrait name gives -1, which lands on a valid built-in slot.
- **The game double-checks Windows' font choice.** With GDI fonts on (instead
  of the bitmap fonts in the pack), each font first tries the localized face name
  from the string table. It then asks GDI which face it really got and throws the
  font away when the names differ, falling back to the default face (Arial, or
  Times New Roman for the big "Haet_120" headline font). In language 12 the
  script font (#8) is also shrunk to 73.92% of its size. Bitmap fonts always use
  the 1600-pixel set unless the screen is exactly one of five standard modes, so
  1152×864 gets the 1600-wide font.
- **A default that is set and then overwritten right away.** When a tree view is
  loaded from the pack (0x55fcc0), its field +0xfa is copied in,
  then set to 32 if it was zero... and a few lines later the same field is copied
  from the record again, so the zero comes back. The fallback never takes effect.
- **How the stock price is made.** A company's value per share (0x4248d0) mixes
  four estimates: profit (×10), revenue, dividends and book value, with weights
  40/10/20/30. The best estimate gets 1.25× weight and the worst 0.8×. In its
  first year a company's weights are 0/0/0/100, so it is valued on book value
  alone; the profit, revenue and dividend weights then grow over five years
  (25/25/35, 50/50/65, 75/75/85). The price (0x424fd0) then squashes the market
  cap ratio: below 0.6 and 0.45 the excess is halved, under 0.3 it is floored, and
  above 1.7, 2.5 and 4 it is halved again, with 6 as the ceiling. So stock
  bubbles get softer the bigger they are.
- **A weight that cancels itself out, but only on top.** The earnings-growth score
  (0x4246b0) weights this year by "months gone / 12" times 5. In the sum it then
  divides by that month weight again, so the numerator counts this year as a full
  5. The total it divides by still uses 5 × months/12. Early in the year, this
  year's growth therefore outweighs its share: in January it counts about 12
  times too much.
- **The number formatter can edit memory it was never given.** After formatting,
  the game's number formatter (0x51b700) walks the output and swaps ',' and '.'
  for the locale's separators until it reaches the terminating zero. For an
  unknown style it writes nothing at all, but it still does that walk. A buffer
  that doesn't hold a string yet gets scanned past its end, and every byte 0x2c
  (',') in the memory behind it becomes the locale's thousands separator. In the
  rebuild's tests this hit the stack's security cookie in about 1 run in 30 and
  killed the process. It took a while to track down.
- **Four distances measured and dropped on the floor.** The routine that draws a
  rotated glyph (0x552900) first works out the four corners of the glyph. It then
  calls the 2D distance function (0x51db80, a square root each time) from the
  pivot point to every corner, and throws away all four results (`FSTP ST0`). It
  looks like left-over code from a version that used the radius to rotate the
  corners. The current version rotates with sine and cosine instead.
- **A hidden ping overlay that can leave the network locked.** 0x469720 draws a
  multiplayer overlay with one yellow "name = 123ms" line per player. It is only
  drawn while the switch at 0x6cd910 is on. To read the player list it takes the
  network queue lock (critical section 0x6d4090). If the list pointer is null, it
  returns without unlocking. The main thread then keeps the critical section, and
  the lock depth counter goes up by one every frame. Any other thread that waits
  for that lock would hang. Worth remembering when hunting multiplayer freezes.
- **An argument that only exists for a tail call.** The routine that clears sent
  multiplayer messages (0x522430) looks like it takes no arguments, and the
  decompiler agrees. Its only caller pushes a dummy 0 anyway. The routine never
  reads that value. Its last instruction overwrites the slot with the address of
  the network critical section and jumps straight into Windows'
  `LeaveCriticalSection`. That function then returns to the original caller and
  pops the slot on its way out. The compiler turned "unlock and return" into a
  tail call, and borrowed the routine's own argument slot to do it. The first
  rebuilt version had no argument, and the difftest caught the stack imbalance.
- **The error message blames the wrong file.** When a pack lookup by number fails,
  the game's fatal error (0x53b390) prints "Pack element < name > not found". It
  doesn't print the element it was looking for. The name it shows is whatever
  name was last turned into a hash (kept at pack +0x427c0), which may have nothing
  to do with the failed lookup. While tracking down a bug in the rebuild, the game
  kept complaining about "MouseCursor.imb", a file that was there all along. The
  failing lookup was actually a texture reload with a corrupted id, and the cursor
  had simply been the last named lookup before it.
- **Our compiler deleted a whole camera mode.** The routine that sets the camera
  tilt for a zoom level (0x43a750) has an early return for the free camera,
  followed by one curve per camera mode. Visual C++ 2019 (/O2, x87 maths) compiled
  the rebuilt version without the mode 0 branch at all, pow calls included. The
  early return also read the wrong temporary. The source was correct; only a
  side-by-side difftest showed that our copy "returned the input unchanged". The
  fix was to move the body after the early return into a separate function the
  compiler may not inline. The original 2003 compiler had no such trouble.
- **The train window crashes on a train without an engine type.** The train
  detail window (0x4dc180) builds the locomotive picture name with "%1_NE.imb"
  from the engine type's name. When the engine has no type (id 0), it skips the
  lookup and formats from address 0 + 0x82, which crashes. Normal games never
  have such a train, but an edited or damaged save could.
