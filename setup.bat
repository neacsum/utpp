@echo off
rem
rem setup environment for Visual Stuio 2017 and later
rem

rem VS2017U2 contains vswhere.exe
if "%VSWHERE%"=="" set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set VSInstallDir=%%i
)
echo Visual studio installation folder is: %VSInstallDir%
rem "%VSInstallDir%\common7\tools\vsdevcmd.bat"
"%VSInstallDir%\common7\tools\vsmsbuildcmd.bat"
