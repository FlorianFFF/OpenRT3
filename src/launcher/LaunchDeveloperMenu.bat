@echo off
setlocal
pushd "%~dp0"
rt3re.exe --dev-menu %*
set RT3_MENU_RC=%ERRORLEVEL%
popd
exit /b %RT3_MENU_RC%
