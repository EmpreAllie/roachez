#pragma once

class CUIEditText: public CUIObject
{
public:

   CUIEditText();
   CUIEditText(std::string Name, int X, int Y, int FrameWidth, std::string Text = "");
   ~CUIEditText();

   void setFont(std::string FontFileName, unsigned int Size = 0);
   void setFontSize(unsigned int Size);
   void setColor(SDL_Color Color);
   void createTextureFromText(void);
   void render(SDL_Renderer* Renderer);
   void appendChar(const char* Character);
   void backspace(void);
   std::string getText(void);
   int getTextHeight(void);

   SDL_Rect m_FrameRect;
   std::string m_Text;
   std::string m_FontFileName;
   TTF_Font* m_Font;
   int m_FontSize;
   SDL_Color m_Color;

};