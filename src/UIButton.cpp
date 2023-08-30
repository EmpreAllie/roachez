#include "roachez.h"

CUIButton:: CUIButton()
{
   m_Type = "CUIButton";
   m_State = EState::DEFAULT;

   int w, h;
   SDL_QueryTexture(m_Texture, nullptr, nullptr, &w, &h);

   m_SrcRect.y = 0;
   m_SrcRect.w = w / 4;
   m_SrcRect.h = h;

   m_DstRect.w = w / 4;
   m_DstRect.h = h;
}

CUIButton:: CUIButton(std::string Name, int X, int Y, std::string ImageFileName) : CUIStaticBitmap(Name, X, Y, ImageFileName)
{
   m_Type = "CUIButton";
   m_State = EState::DEFAULT;

   int w, h;
   SDL_QueryTexture(m_Texture, nullptr, nullptr, &w, &h);
   m_SrcRect = {0, 0, w / 4, h};
   m_DstRect = {X, Y, w / 4, h};
}

CUIButton:: ~CUIButton()
{

}

void CUIButton:: update()
{
   if (Engine->m_Windows.back()->m_Name == "WPlayerNameInput")
   {
      if (m_Name == "SavePlayerButton")
      {
         for (auto w: Engine->m_Windows.back()->m_Widgets)
         {
            if (w->m_Name == "PlayerNameInput")
            {
               m_State = w->getText().empty() ? DISABLED : Engine->m_ActiveObject == this ? HOVER : DEFAULT;
               return;
            }
         }
      }
      else if (m_Name == "ExitButton" && Engine->m_PlayerName.empty())
      {
         m_State = DISABLED;
         return;
      }
   }

   if (m_State == DISABLED || (m_State == PRESSED && Engine->m_ActiveObject == this)) return;

   if (Engine->m_ActiveObject == this)
   {
      m_State = Engine->m_Windows.back()->m_IsPressed ? PRESSED : HOVER;
   }
   else
   {
      m_State = DEFAULT;
   }
}

void CUIButton:: getRect(SDL_Rect* Rect)
{
   if (Rect)
   {
      Rect->x = m_DstRect.x;
      Rect->y = m_DstRect.y;
      Rect->w = m_DstRect.w;
      Rect->h = m_DstRect.h;
   }
}

void CUIButton:: leftClickDown()
{
   if (m_State == DISABLED) return;

   m_State = PRESSED;
}

void CUIButton:: leftClickUp()
{
   if (m_State == DISABLED) return;

   m_State = HOVER;

   if (m_Name == "ExitButton")
   {
      if (Engine->m_Windows.back()->m_Name == "WPause")
      {
         SDL_ShowCursor(SDL_DISABLE);
         Engine->m_Game->m_State = GAME_RUNNING;
      }
      Engine->m_Windows.back()->m_DestroyMe = true;
   }
   else if (m_Name == "RecordsButton")
   {
//      Engine->addNoteToRecords(Engine->m_PlayerName, random(1, 200));
      Engine->createWindow("WRecords");
   }
   else if (m_Name == "InfoButton")
   {
      Engine->createWindow("WInfo");
   }
   else if (m_Name == "ClearListButton")
   {
      for (int i = 0; i < DEFAULTVECTORSIZE; i++)
      {
         snprintf(&(Engine->CurrentRecords[i].Name[0]), MAXLENPLAYERNAME, PLAYERNAME);
         Engine->CurrentRecords[i].Score = 0;
      }
      for (auto w: Engine->m_Windows.back()->m_Widgets)
      {
         if (w->m_Name == "CurRecordsName")
         {
            w->setText(PLAYERNAME);
         }
         else if (w->m_Name == "CurRecordsScore")
         {
            w->setText(std::to_string(0));
         }
      }
   }
   else if (m_Name == "PrintFileButton")
   {
      Engine->writeRecordsToTextFile();
   }
   else if (m_Name == "ChangePlayerButton")
   {
      Engine->createWindow("WPlayerNameInput");
   }
   else if (m_Name == "SavePlayerButton")
   {
      for (auto w: Engine->m_Windows.back()->m_Widgets)
      {
         if (w->m_Name == "PlayerNameInput")
         {
            Engine->m_PlayerName = w->getText();
            break;
         }
      }
      for (auto w: Engine->m_Windows[0]->m_Widgets)
      {
         if (w->m_Name == "PlayerName")
         {
            w->setText("Player Name: " + Engine->m_PlayerName);
            break;
         }
      }

      Engine->m_Windows.back()->m_DestroyMe = true;
   }
   else if (m_Name == "StartGameButton")
   {
      Engine->m_Game->m_AllTime = ALLTIME;
      Engine->m_Game->m_Score = 0;
      Engine->m_Game->m_Timeout = SDL_GetTicks();
      Engine->createWindow("WGameplay");
   }
   else if (m_Name == "BackToMenuButton")
   {
      SDL_ShowCursor(SDL_ENABLE);
      Engine->m_Game->m_State = GAME_FROZEN;
      Engine->m_Windows.back()->m_Parent->m_DestroyMe = true;
      Engine->m_Windows.back()->m_DestroyMe = true;
   }
}

void CUIButton:: render(SDL_Renderer* Renderer)
{
   if (m_State == DEFAULT) m_SrcRect.x = 0;
   else if (m_State == HOVER) m_SrcRect.x = 200;
   else if (m_State == PRESSED) m_SrcRect.x = 400;
   else if (m_State == DISABLED) m_SrcRect.x = 600;

   SDL_RenderCopy(Renderer, m_Texture, &m_SrcRect, &m_DstRect);
}
