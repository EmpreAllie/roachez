#pragma once

#include <string>

class CUIObject
{
public:
   std::string m_Name;
   std::string m_Type;
   int m_X;
   int m_Y;
   unsigned int m_Width;
   unsigned int m_Height;
   SDL_Texture* m_Texture;

   CUIObject();
   ~CUIObject();

   virtual void render(SDL_Renderer* Renderer);
   virtual void setText(std::string Text) {}
   virtual std::string getText(void) { return nullptr; }
   virtual void update(void) {}
   virtual void leftClickDown(void) {}
   virtual void leftClickUp(void) {}
   virtual void getRect(SDL_Rect* Rect) {}
   virtual void appendChar(const char* Character) {}
   virtual void backspace(void) {}

   void setWidth(int Width);
   void setHeight(int Height);
   void setPosition(int X, int Y);
   void setName(std::string Name);
   void setTexture(SDL_Texture* Texture);
};