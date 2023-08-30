#include "roachez.h"

CUIStaticText:: CUIStaticText()
{
   m_Type = "CUIStaticText";
   m_Name.clear();
   m_Text.clear();
   m_FontSize = DEFAULTFONTSIZE;
   m_FontFileName = DEFAULTFONTFILENAME;
   m_Font = TTF_OpenFont(m_FontFileName.c_str(), m_FontSize);
   m_Color = {255, 0, 255, 255};

//   m_AlignHorizontal = LEFT;
//   m_AlignVertical = CENTER;
}

CUIStaticText:: CUIStaticText(std::string Name, int X, int Y, std::string Text)
{
   m_Type = "CUIStaticText";
   m_Name = std::move(Name);
   m_X = X;
   m_Y = Y;
   m_Text = std::move(Text);
   m_FontSize = DEFAULTFONTSIZE;
   m_FontFileName = DEFAULTFONTFILENAME;
   m_Font = TTF_OpenFont(m_FontFileName.c_str(), m_FontSize);
   m_Color = {255, 0, 255, 255};
//   m_AlignHorizontal = LEFT;
//   m_AlignVertical = CENTER;
}

CUIStaticText:: ~CUIStaticText()
{
   TTF_CloseFont(m_Font);
}

void CUIStaticText:: setFont(std::string FontFileName, unsigned int Size)
{
   if (m_Font) TTF_CloseFont(m_Font);
   m_FontFileName = std::move(FontFileName);
   if (Size)
      m_Font = TTF_OpenFont(m_FontFileName.c_str(), Size);
   else
      m_Font = TTF_OpenFont(m_FontFileName.c_str(), m_FontSize);
}

void CUIStaticText:: setFontSize(unsigned int Size)
{
   setFont(m_FontFileName, Size);
}

void CUIStaticText:: setColor(SDL_Color Color)
{
   m_Color = Color;
}

void CUIStaticText:: setText(std::string Text)
{
   m_Text = std::move(Text);
}

void CUIStaticText:: createTextureFromText()
{
   if (m_Texture)
   {
      SDL_DestroyTexture(m_Texture);
      m_Texture = nullptr;
   }

   if (m_Font)
   {
      SDL_Surface* Surface = TTF_RenderUTF8_Solid(m_Font, m_Text.c_str(), m_Color);
      if (Surface)
      {
         m_Texture = SDL_CreateTextureFromSurface(Engine->m_Video->m_Renderer, Surface);
         SDL_FreeSurface(Surface);
      }
      else
         Logw("Unable to create surface for control %s", m_Name.c_str());
   }
   else
      Logw("Unable to initialize font for control %s", m_Name.c_str());
}

void CUIStaticText:: render(SDL_Renderer* Renderer)
{
//   Logi("Called render function for class CUIStaticText");

   int w, h;

   createTextureFromText();
   if (!m_Texture) return;

   SDL_QueryTexture(m_Texture, nullptr, nullptr, &w, &h);
   SDL_Rect DstRect = {m_X, m_Y, w, h};
   SDL_RenderCopy(Renderer, m_Texture, nullptr, &DstRect);
}
