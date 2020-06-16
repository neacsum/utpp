@echo off
rem
rem Setup environment for MSBuild 2017 and later
rem

if not "%VSCMD_VER%"=="" (
  echo MSBuild environment is already set
  goto :EOF
) 
rem VS2017U2 contains vswhere.exe
if "%VSWHERE%"=="" set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set VSInstallDir=%%i
)
rem echo Visual studio installation folder is: %VSInstallDir%
rem "%VSInstallDir%\common7\tools\vsdevcmd.bat"
"%VSInstallDir%\common7\tools\vsmsbuildcmd.bat"
