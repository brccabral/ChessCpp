# Chess C++

Updated version of "Let's make 16 games in C++: Chess" https://www.youtube.com/watch?v=_4EuZI8Q8cs from FamTrinli

## Updates 
Original code calls CreateProcess() from \<windows.h\> but newer compiler versions don't accept parameters char\* vs LPWSTR.  
I tried many other compiler options to make it work, but after a couple of days I still couldn't find an answer.  
Then I decided to create a modern approach. The decision was to make the code to open "stockfish.exe" as a background Process from System::Diagnostics, and this is the final project.  

## Visual Studio Configuration  

To run this project, you need to download SFML https://www.sfml-dev.org/. Currently I have SFML-2.5.1.
Place it anywhere, I placed at D:\SFML-2.5.1.
To configure just SFML:

```
Menu Project >
    "Project" Properties >
        Configuration - Debug >
        Platform - Win32 or x64 >
            Configuration Properties >
                C/C++ >
                    General >
                        Additional Include Directories >
                            D:\SFML-2.5.1\include
                    Preprocessor > 
                        Preprocessor Definitions >
                            SFML_STATIC;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)
                            or
                            SFML_STATIC;WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)
                Linker >
                    General >
                        Additional Library Directories >
                            D:\SFML-2.5.1\lib
                    Input >
                        Additional Dependencies >
                            opengl32.lib;freetype.lib;winmm.lib;gdi32.lib;sfml-graphics-s-d.lib;sfml-window-s-d.lib;sfml-system-s-d.lib;%(AdditionalDependencies)
```

For this project, a few more options are needed:
```
Menu Project >
    "Project" Properties >
        Configuration Debug >
        Platform Win32 and x64 >
            Configuration Properties >
                Advanced >
                    C++/CLI Properties - Common Language Runtime Support >
                        Common Language Runtime Support /clr
                C/C++ >
                    Command Line >
                        Additional Options > (write in the box)
                            /Zc:twoPhase-

```
