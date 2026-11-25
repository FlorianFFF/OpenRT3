@echo off
if not defined RT3_BUILD_LOCKED (
    python "%~dp0tools\runlock.py" --build %*
    exit /b
)
rem Builds (32-bit, the game is a 32-bit exe; needs Visual Studio 2019 C++ build tools):
rem   out\rt3re.dll       the reimplementation, loaded into RT3.exe
rem   out\rt3re_test.dll  same code + differential tests (replaces WinMain with the test runner)
rem   out\rt3re.exe       launcher: starts RT3.exe and loads rt3re.dll into it
rem   out\OpenRT3.exe     standalone: runs the game in its own process with rt3re.dll
rem and copies them into the game folder. "build.bat test" also runs the tests.
setlocal
set VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat
if not exist "%VCVARS%" (echo vcvars32.bat not found, edit VCVARS in build.bat & exit /b 1)
call "%VCVARS%" >nul 2>nul
cd /d "%~dp0"
set GAME=..\Railroad Tycoon 3 Full Game
if not exist out\obj mkdir out\obj
if not exist out\objt mkdir out\objt
set CFLAGS=/nologo /O2 /MT /W3 /EHsc /std:c++17 /Zi /Isrc /D_CRT_SECURE_NO_WARNINGS /MP /arch:IA32 /fp:precise
set LIB_SRC=src\developer_menu.cpp src\hook\*.cpp src\core\*.cpp src\engine\*.cpp src\game\*.cpp src\assets\*.cpp
cl %CFLAGS% /LD src\dllmain.cpp src\harness.cpp src\drawtrace.cpp %LIB_SRC% /Foout\obj\ /Fdout\rt3re.pdb /Fe:out\rt3re.dll /link /MAP:out\rt3re.map user32.lib version.lib || exit /b 1
cl %CFLAGS% /LD tests\*.cpp %LIB_SRC% /Foout\objt\ /Fdout\rt3re_test.pdb /Fe:out\rt3re_test.dll /link /MAP:out\rt3re_test.map user32.lib version.lib || exit /b 1
cl %CFLAGS% src\launcher\launcher.cpp /Foout\obj\ /Fe:out\rt3re.exe || exit /b 1
rem Standalone: loads RT3.exe's image at 0x400000 itself (see openrt3.cpp).
cl %CFLAGS% src\launcher\openrt3.cpp /Foout\obj\ /Fe:out\OpenRT3.exe /link /BASE:0x10000 /FIXED /DYNAMICBASE:NO /SAFESEH:NO /SUBSYSTEM:WINDOWS user32.lib || exit /b 1
copy /y out\OpenRT3.exe "%GAME%" >nul
copy /y out\rt3re.dll "%GAME%" >nul
copy /y out\rt3re_test.dll "%GAME%" >nul
copy /y out\rt3re.exe "%GAME%" >nul
copy /y src\launcher\LaunchDeveloperMenu.bat "%GAME%" >nul
echo built and installed rt3re.dll rt3re_test.dll rt3re.exe OpenRT3.exe
if /i not "%1"=="test" exit /b 0
python tools\checkconv.py || exit /b 1
rem The tests run in the unmodded game: a mirror without mods (tools\stockgame.py).
set STOCK=
for /f "delims=" %%m in ('python tools\stockgame.py') do set STOCK=%%m
if not defined STOCK exit /b 1
pushd "%STOCK%"
"%CD%\rt3re.exe" --test %2
set RC=%ERRORLEVEL%
popd
exit /b %RC%
