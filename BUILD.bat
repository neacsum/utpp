@echo off
rem
rem  Master build file. Execute this script to create all output libraries
rem  Environment variable DEV_ROOT points to the root of development tree.
rem
rem  Syntax:
rem     BUILD [clean | rebuild]

call mklinks.bat
call setup.bat

if "%~1"=="" (msbuild build.proj) else (msbuild build.proj /t:%1)
