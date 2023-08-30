#pragma once

#include <string>

class CUIStaticBitmap: public CUIObject
{
public:
   CUIStaticBitmap();
   CUIStaticBitmap(std::string Name, int X, int Y, std::string ImageFileName);
   ~CUIStaticBitmap();

   virtual void render(SDL_Renderer* Renderer);
};