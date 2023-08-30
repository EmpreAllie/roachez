#include "roachez.h"
#include <iostream>
#include <cstring>

CEngine* Engine;
FILE * f;

int main(int argc, char* argv[])
{
   srand(time(NULL));

   f = fopen(LOGFILENAME, "w");

   SDL_Init(SDL_INIT_EVERYTHING);
   TTF_Init();
   IMG_Init(IMG_INIT_PNG);
   Mix_Init(MIX_INIT_FLAC);

   Engine = new CEngine();

   if (Engine->initialize())
   {
      Engine->loadRecords();

      Engine->createWindow("WMainMenu");
      if (Engine->m_PlayerName.empty()) Engine->createWindow("WPlayerNameInput");

      Engine->loop();
      Engine->saveRecords();
   }

   SAFE_DELETE(Engine);

   Mix_Quit();
   IMG_Quit();
   TTF_Quit();
   SDL_Quit();

   fclose(f);
   return 0;
}
