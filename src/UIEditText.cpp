#include "roachez.h"

CUIEditText:: CUIEditText()
{
   m_Type = "CUIEditText";
   m_FontSize = DEFAULTFONTSIZE;
   m_FontFileName = DEFAULTFONTFILENAME;
   m_Font = TTF_OpenFont(m_FontFileName.c_str(), m_FontSize);
   m_Color = {255, 0, 255, 255};
   m_Width = DEFAULTFRAMEWIDTH;
   m_Height = getTextHeight();
   m_FrameRect = {m_X - MARGIN, m_Y - MARGIN, (int) (m_Width + MARGIN * 2), (int) (m_Height + MARGIN * 2)};
}

CUIEditText:: CUIEditText(std::string Name, int X, int Y, int FrameWidth, std::string Text)
{
   m_Type = "CUIEditText";
   m_Name = std::move(Name);
   m_X = X;
   m_Y = Y;
   m_Text = std::move(Text);
   m_FontSize = DEFAULTFONTSIZE;
   m_FontFileName = DEFAULTFONTFILENAME;
   m_Font = TTF_OpenFont(m_FontFileName.c_str(), m_FontSize);
   m_Color = {255, 0, 255, 255};
   m_Width = FrameWidth;
   m_Height = getTextHeight();
   m_FrameRect = {m_X - MARGIN, m_Y - MARGIN, (int) (m_Width + MARGIN * 2), (int) (m_Height + MARGIN * 2)};
}

CUIEditText:: ~CUIEditText()
{
   if (m_Texture) SDL_DestroyTexture(m_Texture);
   m_Texture = nullptr;
}


int CUIEditText:: getTextHeight()
{
   int h = 0;
   std::string Text = m_Text;
   m_Text = " ";
   createTextureFromText();
   if (m_Texture)
   {
      SDL_QueryTexture(m_Texture, nullptr, nullptr, nullptr, &h);
      SDL_DestroyTexture(m_Texture);
      m_Texture = nullptr;
   }
   m_Text = std::move(Text);
   return h;
}

void CUIEditText:: setFont(std::string FontFileName, unsigned int Size)
{
   if (m_Font) TTF_CloseFont(m_Font);
   m_FontFileName = std::move(FontFileName);
   if (Size)
      m_Font = TTF_OpenFont(m_FontFileName.c_str(), Size);
   else
      m_Font = TTF_OpenFont(m_FontFileName.c_str(), m_FontSize);
   m_Height = getTextHeight();
}

void CUIEditText:: setFontSize(unsigned int Size)
{
   setFont(m_FontFileName, Size);
}

void CUIEditText:: createTextureFromText()
{
   if (m_Texture)
   {
      SDL_DestroyTexture(m_Texture);
      m_Texture = nullptr;
   }

   if (m_Font)
   {
      if (!m_Text.empty())
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
   }
   else
      Logw("Unable to initialize font for control %s", m_Name.c_str());
}

void CUIEditText:: render(SDL_Renderer* Renderer)
{
   SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
   SDL_RenderDrawRect(Renderer, &m_FrameRect);

   int w, h;
   createTextureFromText();
   if (!m_Texture) return;
   SDL_QueryTexture(m_Texture, nullptr, nullptr, &w, &h);
   SDL_Rect DstRect = {m_X, m_Y, w, h};
   SDL_RenderCopy(Renderer, m_Texture, nullptr, &DstRect);
}

void CUIEditText:: appendChar(const char* Character)
{
   std::string Str = m_Text;
   std::string Char = {Character};
   Str = Str + Char;

   if (m_Font)
   {
      SDL_Surface* Surface = TTF_RenderUTF8_Solid(m_Font, Str.c_str(), m_Color);
      SDL_Texture* Texture = SDL_CreateTextureFromSurface(Engine->m_Video->m_Renderer, Surface);
      int w;
      SDL_QueryTexture(Texture, nullptr, nullptr, &w, nullptr);
      SDL_DestroyTexture(Texture);
      SDL_FreeSurface(Surface);
      if ((unsigned int)w < m_Width)
      {
         m_Text = std::move(Str);
         Engine->m_Sound->playSound(ESound::SOUND_TYPING);
      }
      else
      {
         Engine->m_Sound->playSound(ESound::SOUND_DING);
         return;
      }
   }
   else
      Logw("Unable to initialize font for control %s", m_Name.c_str());

}

void CUIEditText:: backspace()
{
   if (m_Text.empty()) return;

   const char* PtrByte = m_Text.data() + m_Text.size();
   while (--PtrByte >= m_Text.data() && ((*PtrByte & 0b10000000) && !(*PtrByte & 0b01000000)))
   {
      ;
   }
   if (PtrByte >= m_Text.data()) m_Text.resize(PtrByte - m_Text.data());
}

void CUIEditText:: setColor(SDL_Color Color)
{
   m_Color = Color;
}

std::string CUIEditText:: getText()
{
   return m_Text;
}