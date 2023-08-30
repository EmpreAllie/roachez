#pragma once

class CUIButton : public CUIStaticBitmap
{
public:

   SDL_Rect m_SrcRect;
   SDL_Rect m_DstRect;

   enum EState
   {
      DEFAULT, HOVER, PRESSED, DISABLED
   };

   EState m_State;

   CUIButton();
   CUIButton(std::string Name, int X, int Y, std::string ImageFileName);
   ~CUIButton();

   void render(SDL_Renderer* Renderer);
   void update(void);
   void getRect(SDL_Rect* Rect);
   void leftClickDown(void);
   void leftClickUp(void);
};