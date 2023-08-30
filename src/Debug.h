#pragma once

class CDebug: public CObject
{
private:

public:
   CDebug();
   ~CDebug();
   void render(SDL_Renderer* Renderer);
};
