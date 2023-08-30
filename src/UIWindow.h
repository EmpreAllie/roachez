#pragma once

#include <vector>

class CUIWindow: public CUIObject
{
public:
   CUIWindow();
   CUIWindow(std::string Name);
   ~CUIWindow();

   CUIWindow* m_Parent;

   void render(SDL_Renderer* Renderer);
   void initialize(void);
   void update(void);
   bool m_DestroyMe;
   bool m_IsPressed;

   std::vector<CUIObject*> m_Widgets;

   SDL_Texture* m_Background;
   void setBackground(std::string BGFileName);
};