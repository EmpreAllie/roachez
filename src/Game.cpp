#include "roachez.h"

CGame:: CGame()
{
   m_Type = "CGame";
   m_State = GAME_FROZEN;
   m_PlayerName.clear();
   m_Score = 0;
   m_SlipperTexture[SLIPPER_DEFAULT] = IMG_LoadTexture(Engine->m_Video->m_Renderer, "./Images/SlipperDefault.png");
   m_SlipperTexture[SLIPPER_PRESSED] = IMG_LoadTexture(Engine->m_Video->m_Renderer, "./Images/SlipperPressed.png");
   int w, h;
   SDL_QueryTexture(m_SlipperTexture[SLIPPER_DEFAULT], nullptr, nullptr, &w, &h);
   m_SlipperRect = {Engine->m_MouseRect.x, Engine->m_MouseRect.y, w, h};
   m_Level = LEVEL1;
   m_AllTime = 0;
   m_Timeout = 0;
   m_SlipperTimer = 0;
   m_SlipperState = SLIPPER_DEFAULT;
}

CGame:: ~CGame()
{
   clearRoaches();
   SDL_DestroyTexture(m_SlipperTexture[SLIPPER_DEFAULT]);
   SDL_DestroyTexture(m_SlipperTexture[SLIPPER_PRESSED]);
}

void CGame:: updateState()
{
   int delta = ALLTIME / 4;
   m_Level = (ELevel)((ALLTIME - m_AllTime - 1) / delta);

   m_SlipperRect.x = Engine->m_MouseRect.x;
   m_SlipperRect.y = Engine->m_MouseRect.y;

   for (auto w: Engine->m_Windows.back()->m_Widgets)
   {
      if (w->m_Name == "ScoreNum")
      {
         w->setText(std::to_string(m_Score));
         break;
      }
   }


   for (size_t i = 0; i < m_Roaches.size(); i++)
   {
      if (m_Roaches[i])
      {
         m_Roaches[i]->update();
         if (m_Roaches[i]->m_State == ROACH_DEAD || m_Roaches[i]->m_X > GAMEWIDTH + ROACHSIZE || m_Roaches[i]->m_X < 0 - ROACHSIZE || m_Roaches[i]->m_Y > GAMEHEIGHT + ROACHSIZE || m_Roaches[i]->m_Y < 0 - ROACHSIZE)
         {
            SAFE_DELETE(m_Roaches[i]);
         }
      }
      else
      {
         m_Roaches[i] = new CRoach();
      }
   }

}

void CGame:: render()
{
   if (Engine->m_Windows.back()->m_Name == "WGameplay")
   {
      for (size_t i = 0; i < m_Roaches.size(); i++)
      {
         if (m_Roaches[i]) m_Roaches[i]->render(Engine->m_Video->m_Renderer);
      }

      SDL_RenderCopy(Engine->m_Video->m_Renderer, m_SlipperTexture[m_SlipperState], nullptr, &m_SlipperRect);
   }
}

void CGame:: clearRoaches()
{
   for (size_t i = 0; i < m_Roaches.size(); i++)
   {
      SAFE_DELETE(m_Roaches[i]);
   }
}

