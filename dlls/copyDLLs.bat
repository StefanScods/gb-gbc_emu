:: This script is use to copy the required DLLs to run the emu to the target folder.
:: Expected CWD is the root of the repository -> %DIR%/gb-gbc_emu/
:: Usage: .\copyDLLs.bat "Release"/"Debug" targetDir
:: If no targetDir is supplied, the x64 release and debug directory is targeted.
@echo off

:: Determine whether to copy debug or release dlls.
if "%~1"=="Release" (
    set srcDir=.\dlls\releaseDLLs
) else if "%~1"=="Debug" (
    set srcDir=.\dlls\debugDLLs
) else (
    exit /b 1
)

:: Determine the target directory.
if "%~2"=="" (
    set targetDir=.\x64\%~1
) else (
    set targetDir=%~2
)

Xcopy %srcDir% %targetDir% /i /D