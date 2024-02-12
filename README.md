# Red Panda GB/GBC Emulator



## Dependencies
To compile, you will need:
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/downloads/)
- Within the VS Community installer, select and install `Desktop development with C++`
    - Ensure `MSVC v143 - VS 2022 C++ x86/x64 build tools` is selected. Note other versions may work but are untested.
- Add `C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin` to the PATH environment variable. 

## Build and Run Instructions
### VSCode
Simply run the `run (release)` task via the command palette.

### CMD
Run the following commands:
```shell
msbuild gb-gbc_emu.sln /property:Configuration=Release /property:Platform=x64
./dlls/copyDLLs.bat Release 
./x64/Release/gb-gbc_emu.exe
```