#include "roachez.h"

using namespace std;

CUIObject:: CUIObject()
{
   m_Type.clear();
   m_Name.clear();
   m_X = 0;
   m_Y = 0;
   m_Width = 0;
   m_Height = 0;
   m_Texture = nullptr;
}

CUIObject:: ~CUIObject()
{
   if (m_Texture) SDL_DestroyTexture(m_Texture);
   m_Texture = nullptr;
}

void CUIObject:: setPosition(int X, int Y)
{
   m_X = X;
   m_Y = Y;
}

void CUIObject:: setName(std::string Name)
{
   m_Name = std::move(Name);
}

void CUIObject:: setTexture(SDL_Texture* Texture)
{
   if (m_Texture) SDL_DestroyTexture(m_Texture);
   m_Texture = Texture;
}

void CUIObject:: setWidth(int Width)
{
   m_Width = Width;
}

void CUIObject:: setHeight(int Height)
{
   m_Height = Height;
}

void CUIObject:: render (SDL_Renderer* Renderer)
{
   return;
}
