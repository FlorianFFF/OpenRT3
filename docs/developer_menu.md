# Original developer menu

Start `LaunchDeveloperMenu.bat` in the game folder, or run:

```
rt3re.exe --dev-menu
```

This loads the original `MNUDFLT` menu resource from `RT3.exe` and attaches it to the window. Its
original command IDs use the game's existing command handler. The launcher adds the game's `-s2`
windowed safe-mode preset (with sound), which resets display settings to that preset. Normal
`rt3re.exe` launches are unchanged. The original exe is not modified on disk.

For a windowed game using your current settings, enable the menu without the safe-mode preset by
adding this to `rt3re.ini` beside the launcher:

```ini
[developer]
menu=1
```

The menu is attached on the game's UI thread and reattached after window recreation. Its height is
added to the outer window so the rendering/input area keeps its size. Some entries need a loaded
map or refer to unfinished developer features. Use a test save when experimenting with commands
that change game state.

The opt-in extension is in `src/developer_menu.cpp`; it is separate from the
function-for-function rewrites and disabled during ordinary differential tests.

