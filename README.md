# Chess C++

Updated version of "Let's make 16 games in C++: Chess" https://www.youtube.com/watch?v=_4EuZI8Q8cs from FamTrinli

### Updates 
Original code calls CreateProcess() from \<windows.h\> but newer compile versions don't accept parameters char\* vs LPWSTR.  
I tried many compile options to make it work, but after a couple of days I still couldn't find an answer.  
Then I decided to create a modern approach. The decision was to open "stockfish.exe" as a background Process from System::Diagnostics, and this is the final project.  