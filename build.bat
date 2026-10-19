@echo off
rem Builds (32-bit, the game is a 32-bit exe; needs Visual Studio 2019 C++ build tools):
rem   out\rt3re.dll       the reimplementation, loaded into RT3_mp.exe
rem   out\rt3re_test.dll  same code + differential tests (replaces WinMain with the test runner)
rem   out\rt3re.exe       launcher
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
set LIB_SRC=src\hook\*.cpp src\core\*.cpp src\engine\*.cpp src\game\*.cpp src\assets\*.cpp
cl %CFLAGS% /LD src\dllmain.cpp src\harness.cpp %LIB_SRC% /Foout\obj\ /Fdout\rt3re.pdb /Fe:out\rt3re.dll /link /MAP:out\rt3re.map user32.lib || exit /b 1
cl %CFLAGS% /LD tests\*.cpp %LIB_SRC% /Foout\objt\ /Fdout\rt3re_test.pdb /Fe:out\rt3re_test.dll /link /MAP:out\rt3re_test.map user32.lib || exit /b 1
cl %CFLAGS% src\launcher\launcher.cpp /Foout\obj\ /Fe:out\rt3re.exe || exit /b 1
copy /y out\rt3re.dll "%GAME%" >nul
copy /y out\rt3re_test.dll "%GAME%" >nul
copy /y out\rt3re.exe "%GAME%" >nul
echo built and installed rt3re.dll rt3re_test.dll rt3re.exe
if /i not "%1"=="test" exit /b 0
python tools\checkconv.py || exit /b 1
pushd "%GAME%"
"%CD%\rt3re.exe" --test %2
set RC=%ERRORLEVEL%
popd
exit /b %RC%
