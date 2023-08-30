del ..\\roachez.exe
c++ -g -Wextra -static-libstdc++ "*.cpp" -o "../roachez.exe" -IC:/msys64/mingw32/include -IC:/msys64/mingw32/include/SDL2 -I./include -Dmain=SDL_main -LC:/msys64/mingw32/lib -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
cd ..
roachez.exe
cd src

