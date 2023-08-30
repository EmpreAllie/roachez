#include "roachez.h"

CUIWindow:: CUIWindow()
{
   m_Type = "CUIWindow";
   m_Name.clear();
   m_X = m_Y = 0;
   m_Width = GAMEWIDTH;
   m_Height = GAMEHEIGHT;
   m_Background = nullptr;
   m_DestroyMe = false;
   m_IsPressed = false;
   m_Parent = nullptr;
}

CUIWindow:: CUIWindow(std::string Name)
{
   m_Type = "CUIWindow";
   m_Name = std::move(Name);
   m_X = m_Y = 0;
   m_Width = GAMEWIDTH;
   m_Height = GAMEHEIGHT;
   m_Background = nullptr;
   m_DestroyMe = false;
   m_IsPressed = false;
   m_Parent = nullptr;
}

CUIWindow:: ~CUIWindow()
{
   for (auto w: m_Widgets)
   {
      SAFE_DELETE(w);
   }

   SDL_DestroyTexture(m_Background);
}

void CUIWindow:: initialize()
{
   if (m_Name == "WMainMenu")
   {
      setBackground("./Images/MainMenuBG.png");

      CUIStaticBitmap* b;
      CUIStaticText* t;
      CUIButton* bt;

      b = new CUIStaticBitmap("Logo", 240, 80, "./Images/MainLogo.png");
      m_Widgets.push_back(static_cast <CUIObject*> (b));

      t = new CUIStaticText("Credits", 10, 5, "Кольцов К. О721Б");
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      t = new CUIStaticText("PlayerName", 10, 45, "Имя игрока: " + Engine->m_PlayerName);
      t->setColor({255, 208, 156, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      bt = new CUIButton("RecordsButton", 0, 420, "./Images/RecordsButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (bt));
      
      bt = new CUIButton("StartGameButton", 200, 420, "./Images/StartGameButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (bt));

      bt = new CUIButton("ChangePlayerButton", 400, 420, "./Images/ChangePlayerButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (bt));

      bt = new CUIButton("InfoButton", 600, 420, "./Images/GameInfoButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (bt));

      bt = new CUIButton("ExitButton", 600, 30, "./Images/ExitButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (bt));
   }
   else if (m_Name == "WRecords")
   {
      setBackground("./Images/RecordsBG.png");

      CUIStaticText* st;
      CUIButton* btn;
     
      btn = new CUIButton("ExitButton", 600, 30, "./Images/BackToMenuButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

      btn = new CUIButton("ClearListButton", 600, 80, "./Images/ClearListButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

      btn = new CUIButton("PrintFileButton", 600, 130, "./Images/PrintFileButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

      st = new CUIStaticText("", 290, 100, "Рекорды");
      st->setColor({117, 247, 104 , 255});
      st->setFontSize(72);
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      int y = 240;
      int step = 30;                 

      st = new CUIStaticText("", 35, y - step, "10 лучших за всё время:");
      st->setColor({117, 247, 104, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      st = new CUIStaticText("", 435, y - step, "Лучшие за текущую сессию:");
      st->setColor({117, 247, 104, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));
      
      for (size_t i = 0; i < DEFAULTVECTORSIZE; i++)
      {
         st = new CUIStaticText("", 35, y + i*step, Engine->AllTimeRecords[i].Name);         
         st->setColor({238, 247, 79, 255});
         m_Widgets.push_back(static_cast <CUIObject*> (st));
         st = new CUIStaticText("", 280, y + i*step, std::to_string(Engine->AllTimeRecords[i].Score)); 
         st->setColor({238, 247, 79, 255});
         m_Widgets.push_back(static_cast <CUIObject*> (st));

         st = new CUIStaticText("CurRecordsName", 435, y + i*step, Engine->CurrentRecords[i].Name);         
         st->setColor({238, 247, 79, 255});
         m_Widgets.push_back(static_cast <CUIObject*> (st));
         st = new CUIStaticText("CurRecordsScore", 680, y + i*step, std::to_string(Engine->CurrentRecords[i].Score));         
         st->setColor({238, 247, 79, 255});
         m_Widgets.push_back(static_cast <CUIObject*> (st));
      }
   }
   else if (m_Name == "WGameplay")
   {
      setBackground("./Images/linoleum.png");

      CUIStaticText* st;

      st = new CUIStaticText("ScoreText", 650, 10, "Очки:");
      st->setColor({92, 73, 58, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      st = new CUIStaticText("ScoreNum", 715, 10, "0");
      st->setColor({92, 73, 58, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));      

      st = new CUIStaticText("Timer", 650, 525, "");
      st->setColor({92, 73, 58, 255});
      st->setFont("./Fonts/foxtail-bold.ttf", 72);
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      SDL_ShowCursor(SDL_DISABLE);
      Engine->m_Game->m_State = GAME_RUNNING;
   }
   else if (m_Name == "WPause")
   {
      SDL_ShowCursor(SDL_ENABLE);
      Engine->m_Game->m_State = GAME_PAUSED;

      setBackground("./Images/PauseBG.png");

      CUIStaticText* st;
      CUIButton* btn;

      st = new CUIStaticText("Пауза", 320, 160, "Пауза");
      st->setFont("./Fonts/foxtail-bold.ttf", 72);
      st->setColor({104, 245, 247, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      btn = new CUIButton("ExitButton", 300, 280, "./Images/ContinueButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

      btn = new CUIButton("BackToMenuButton", 295, 340, "./Images/BackToMenuButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

   }
   else if (m_Name == "WGameEnded")
   {
      setBackground("./Images/GameEndedBG.png");

      CUIStaticText* st;
      CUIButton* btn;

      st = new CUIStaticText("GameEnded", 200, 200, "Игра окончена!");
      st->setFont("./Fonts/foxtail-bold.ttf", 72);
      st->setColor({104, 245, 247, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));
      
      st = new CUIStaticText("Score", 300, 300, "Ваш результат: " + std::to_string(Engine->m_Game->m_Score));
      st->setColor({255, 210, 255, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      btn = new CUIButton("ExitButton", 300, 450, "./Images/BackToMenuButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

      Engine->addNoteToRecords(Engine->m_PlayerName, Engine->m_Game->m_Score);

      SDL_ShowCursor(SDL_ENABLE);
      Engine->m_Game->m_State = GAME_FROZEN;
   }
   else if (m_Name == "WPlayerNameInput")
   {
      setBackground("./Images/ChangePlayerBG.png");

      CUIEditText* et;
      CUIStaticText* st;
      CUIButton* btn;

      st = new CUIStaticText("", 215, 90, "Введите имя игрока:");
      st->setFontSize(72);
      st->setColor({255, 210, 255, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (st));

      et = new CUIEditText("PlayerNameInput", 250, 220, DEFAULTFRAMEWIDTH * 3, Engine->m_PlayerName);
      et->setColor({214, 115, 84, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (et));

      btn = new CUIButton("SavePlayerButton", 300, 250, "./Images/SavePlayerButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

      btn = new CUIButton("ExitButton", 295, 350, "./Images/BackToMenuButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));
   }
   else if (m_Name == "WInfo")
   {
      setBackground("./Images/InfoBG.png");

      CUIStaticText* t;
      CUIButton* btn;

      t = new CUIStaticText("", 210, 60, "Информация об игре");
      t->setColor({255, 210, 255, 255});
      t->setFontSize(72);
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      t = new CUIStaticText("", 200, 150, "Легендарная игра ''Охота на тараканов''");
      t->setColor({255, 208, 156, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      t = new CUIStaticText("", 340, 230, "Правила:");
      t->setFont("./Fonts/foxtail-bold.ttf");
      t->setColor({255, 210, 255, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      t = new CUIStaticText("", 160, 270, "Тыкаешь тапком по тараканам, набираешь очки");
      t->setColor({255, 208, 156, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      t = new CUIStaticText("", 250, 300, "(пока на этом, собственно, всё)");
      t->setFontSize(30);
      t->setColor({255, 208, 156, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      t = new CUIStaticText("", 230, 540, "(c) 2023 Konstantin Koltsov All right reserved idrc");
      t->setFontSize(26);
      t->setColor({255, 208, 156, 255});
      m_Widgets.push_back(static_cast <CUIObject*> (t));

      btn = new CUIButton("ExitButton", 300, 400, "./Images/BackToMenuButton.png");
      m_Widgets.push_back(static_cast <CUIObject*> (btn));

   }
}

void CUIWindow:: render(SDL_Renderer* Renderer)
{
   if (m_Name == "WPause")
   {
      m_Parent->render(Renderer);
      for (size_t i = 0; i < Engine->m_Game->m_Roaches.size(); i++)
      {
         if (Engine->m_Game->m_Roaches[i]) Engine->m_Game->m_Roaches[i]->render(Engine->m_Video->m_Renderer);
      }
   }

   SDL_RenderCopy(Renderer, m_Background, NULL, NULL);
   for (auto w: m_Widgets)
   {
      w->render(Renderer);
   }
}

void CUIWindow:: update()
{
   Engine->m_ActiveObject = nullptr;
   for (auto w: m_Widgets)
   {
      if (w->m_Type == "CUIButton" && Engine->m_ActiveObject == nullptr) 
      {
         SDL_Rect Rect;
         w->getRect(&Rect);
         if (SDL_HasIntersection(&Rect, &(Engine->m_MouseRect)))
         {
            Engine->m_ActiveObject = w;            
         }
      }
      w->update();
   }
}

void CUIWindow:: setBackground(std::string BGFileName)
{
   if (BGFileName.empty()) return;
   m_Background = IMG_LoadTexture(Engine->m_Video->m_Renderer, BGFileName.c_str());
}