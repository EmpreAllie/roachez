#include "roachez.h"

extern CEngine* Engine;

CUIStaticBitmap:: CUIStaticBitmap()
{
   m_Type = "CUIStaticBitmap";
}

CUIStaticBitmap:: CUIStaticBitmap(std::string Name, int X, int Y, std::string ImageFileName)
{
   m_Type = "CUIStaticBitmap";
   m_Name = std::move(Name);
   m_X = X;
   m_Y = Y;
   m_Texture = IMG_LoadTexture(Engine->m_Video->m_Renderer, ImageFileName.c_str());
}

CUIStaticBitmap:: ~CUIStaticBitmap()
{
   SDL_DestroyTexture(m_Texture);
}

void CUIStaticBitmap:: render(SDL_Renderer* Renderer)
{
   SDL_Rect DstRect = { m_X, m_Y, 0, 0 };
   SDL_QueryTexture(m_Texture, nullptr, nullptr, &DstRect.w, &DstRect.h);
   SDL_RenderCopy(Renderer, m_Texture, nullptr, &DstRect);
}
