@echo off
rem Runs a command inside the 32-bit MSVC environment: vc.bat cl /nologo foo.cpp
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" >nul 2>nul
%*
