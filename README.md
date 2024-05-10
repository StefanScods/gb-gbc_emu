# Red Panda GB/GBC Emulator
This is a Nintendo GameBoy / GameBoy Color emulator written in C++.  

<p float="left">
<img src="resources\pokemon.gif" width="400" />
<img src="resources\tetris.gif" width="400" />
<img src="resources\zelda.gif" width="400" /> 
</p>

## Features
- Emulates Gameboy / Gameboy Color within a WxWidgets / SDL2 based application.
- Uses the PortAudio API to play audio.
- Support for memory controllers (a large portion of all games only use the following):
    - No Controller
    - MBC1
    - MBC3
    - MBC5
- Supports emulated battery saves along with save states.
- Support for MBC3's real-time battery-backed clock.
- Visualizer for:
    - Cartridge Specifications
    - CPU State
    - Memory
    - Palettes
    - Tile Set
    - Background / Window Maps
- Basic debugger with support for adding breakpoints within GB/GBC ROMs and stepping the emulated hardware for either a cycle or for a frame.


![resources\allWindows.png](resources\allWindows.png)

## Controls

<table><tr>
<td>
    <table><tr>
    <td> Start </td>
    <td><kbd>e</kbd></td>
    </tr>
    <tr>
    <td> Select </td>
    <td><kbd>q</kbd></td>
    </tr>
    <tr>
    <td> A </td>
    <td><kbd>enter</kbd></td>
    </tr>
    <tr>
    <td >B </td>
    <td><kbd>rShift</kbd></td>
    </tr></table>
    
</td>
<td>
    <table><tr>
    <td> Up </td>
    <td><kbd>w</kbd></td>
    </tr>
    <tr>
    <td> Down </td>
    <td><kbd>s</kbd></td>
    </tr>
    <tr>
    <td> Left </td>
    <td><kbd>a</kbd></td>
    </tr>
    <tr>
    <td> Right </td>
    <td><kbd>d</kbd></td>
    </tr></table>
</td>
</tr> </table>

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

## Future Work / Known Issues
- Fix minor graphics / audio bugs
    - The first scanline is slightly offset during some instances
- Add some high pass audio filtering to the [APU for cleaner audio](https://gbdev.io/pandocs/Audio_details.html#audio-details)
- Overall optimizations / improvements
- Add additional visualization windows
- Add support for the more obscure memory controllers.
- Kirby's Dream Land crashes after loading the first level.

## Resources
- [GB Dev Pandocs](https://gbdev.io/pandocs/Specifications.html)
- [gameboy_opcodes](https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
- [gbops](https://izik1.github.io/gbops/)
- [gbz80 - CPU opcode reference](https://rgbds.gbdev.io/docs/v0.5.1/gbz80.7)
- [GitHub - Baekalfen-PyBoy](https://github.com/Baekalfen/PyBoy)
- [GitHub - geaz-emu-gameboy](https://github.com/geaz/emu-gameboy)
- [javidx9 - NES Emulator From Scratch youtube series](https://youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&si=AnIgsr8xeDOgW3Zx)
- [The Cycle-Accurate Game Boy Docs](https://github.com/geaz/emu-gameboy/blob/master/docs/The%20Cycle-Accurate%20Game%20Boy%20Docs.pdf)
- [Writing a Game Boy emulator, Cinoop](https://cturt.github.io/cinoop.html)
- [codeslinger.co.uk](http://www.codeslinger.co.uk/pages/projects/gameboy/beginning.html)