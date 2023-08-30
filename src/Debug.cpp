#include "roachez.h"

using namespace std;

CDebug:: CDebug()
{
   m_Type = "CDebug";
}

CDebug:: ~CDebug()
{

}

void CDebug:: render(SDL_Renderer* Renderer)
{
   std::string s = "(null)";
   if (Engine->m_ActiveObject)
   {
      s = Engine->m_ActiveObject->m_Name;
   }
   CUIStaticText* ActiveObj = new CUIStaticText("ActiveObject", 0, GAMEHEIGHT - 30, s);
   ActiveObj->render(Renderer);
   SAFE_DELETE(ActiveObj);
}