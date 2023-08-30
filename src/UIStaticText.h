#pragma once

class CUIStaticText: public CUIObject
{
public:
   CUIStaticText();
   CUIStaticText(std::string Name, int X, int Y, std::string Text);
   ~CUIStaticText();

   void setFont(std::string FontFileName, unsigned int Size = 0);
   void setFontSize(unsigned int Size);
   void setColor(SDL_Color Color);
   void createTextureFromText(void);
   void render(SDL_Renderer* Renderer);
   void setText(std::string Text);

   std::string m_Text;
   std::string m_FontFileName;
   TTF_Font* m_Font;
   int m_FontSize;
   SDL_Color m_Color;
/*
   enum EAlign
   {
      LEFT, CENTER, RIGHT,
      TOP, BOTTOM
   };
   EAlign m_AlignHorizontal;
   EAlign m_AlignVertical;
*/
};