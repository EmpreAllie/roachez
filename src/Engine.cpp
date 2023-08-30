#include "roachez.h"
#include <fstream>
#include <iomanip>
#include <inttypes.h>

CEngine:: CEngine()
{
   m_Type = "CEngine";
   m_ActiveObject = nullptr;
   m_PlayerName.clear();
   m_SlipperGodmode = false;
}

CEngine:: ~CEngine()
{
   std::fstream file ("./settings.dat", std::ios::out);
   file << m_PlayerName;
   file.close();

   for (CUIWindow* Window: m_Windows)
   {
      SAFE_DELETE(Window);
   }
   SAFE_DELETE(m_Debug);
   SAFE_DELETE(m_Video);
   SAFE_DELETE(m_Sound);
   SAFE_DELETE(m_Game);
}

bool CEngine:: initialize()
{
   m_Debug = new CDebug();
   IsInitialized = IsInitialized && m_Debug->IsInitialized;
   m_Video = new CVideo();
   IsInitialized = IsInitialized && m_Video->IsInitialized;
	m_Sound = new CSound();
	m_Sound->initialize();
	IsInitialized = IsInitialized && m_Sound->IsInitialized;
   m_Game = new CGame();
   IsInitialized = IsInitialized && m_Game->IsInitialized;

   std::fstream file("./settings.dat", std::ios::in);
   std::getline(file, m_PlayerName);
   file.close();

   return IsInitialized;
}

void CEngine:: loop()
{
   uint32_t FrameStart, FrameTime, FrameDelay = 1000 / m_Video->m_DisplayFPS;

//#define TEST_PERFORMANCE

#if defined TEST_PERFORMANCE
   uint64_t c_all = 0, c_frame = 0;
#endif

   uint32_t GameFrameStart, GameFrameTime;

   GameFrameStart = SDL_GetTicks();
   FrameStart = SDL_GetTicks();

   while (true)
   {
      if (handleEvent())
      {
         break;
      }

      while(!m_Windows.empty())
      {
         if (m_Windows.back()->m_DestroyMe)
         {
            m_ActiveObject = nullptr;
            if (m_Windows.back()->m_Name == "WGameplay") m_Game->clearRoaches();
            destroyWindow();
         }
         else
         {
            break;
         }
      }

      if (m_Windows.empty()) break;

      if (m_Game->m_State == GAME_RUNNING) m_Game->updateState();

      if (m_Windows.back()->m_Name == "WGameplay")
      {
         if (SDL_GetTicks() > m_Game->m_Timeout)
         {
            if (--m_Game->m_AllTime < 0)
            {
               m_Windows.back()->m_DestroyMe = true;
               createWindow("WGameEnded");
            }
            m_Game->m_Timeout = SDL_GetTicks() + DELTATIME;
            char buf[16] = {0};
            snprintf(buf, 16, "%d:%02d", m_Game->m_AllTime / 60, m_Game->m_AllTime % 60);

            for (auto w: m_Windows.back()->m_Widgets)
            {
               if (w->m_Name == "Timer")
               {
                  std::string s = {&buf[0]};
                  w->setText(s);
                  break;
               }
            }
         }
      }

      FrameTime = SDL_GetTicks();

      if (FrameTime - FrameStart > FrameDelay)
      {

#if defined TEST_PERFORMANCE
   c_frame++;
#endif
         SDL_SetRenderDrawColor(m_Video->m_Renderer, 255, 0, 255, 255);
         SDL_RenderClear(m_Video->m_Renderer);

         m_Windows.back()->update();
         m_Windows.back()->render(m_Video->m_Renderer);

         m_Game->render();

         //m_Debug->render(m_Video->m_Renderer);

         SDL_RenderPresent(m_Video->m_Renderer);
         FrameStart = FrameTime;
      }

#if defined TEST_PERFORMANCE
   c_all++;
#endif

   }
#if defined TEST_PERFORMANCE
   Logi("TEST_PERFORMANCE: all: %" PRIu64 ",  frame: %" PRIu64, c_all, c_frame);
#endif
}

bool CEngine:: handleEvent()
{
   SDL_Event event;
   while (SDL_PollEvent(&event))
   {

      switch(event.type)
      {

      case SDL_QUIT:
         goto exit;
         break;

      case SDL_MOUSEMOTION:
      {
         m_MouseRect.x = event.motion.x;
         m_MouseRect.y = event.motion.y;
      }
      break;

      case SDL_MOUSEBUTTONDOWN:
      {
         if (event.button.button == SDL_BUTTON_LEFT)
         {
            m_Windows.back()->m_IsPressed = true;
            if (m_Windows.back()->m_Name == "WGameplay")
            {
               m_Game->m_SlipperState = SLIPPER_PRESSED;
               m_Game->m_SlipperTimer = SDL_GetTicks() + 100;
               m_Sound->playSound(ESound::SOUND_SLAP);
            }
            else
            {
               if (m_ActiveObject) m_ActiveObject->leftClickDown();
            }
         }
      }
      break;

      case SDL_MOUSEBUTTONUP:
      {
         if (event.button.button == SDL_BUTTON_LEFT)
         {
            m_Windows.back()->m_IsPressed = false;
            if (m_Windows.back()->m_Name == "WGameplay")
            {
               m_Game->m_SlipperState = SLIPPER_DEFAULT;
            }
            else
            {
               if (m_ActiveObject) m_ActiveObject->leftClickUp();
            }
         }
      }
      break;

      case SDL_TEXTINPUT:
      {
         if (m_Windows.back()->m_Name == "WPlayerNameInput")
         {
            for (auto w: m_Windows.back()->m_Widgets)
            {
               if (w->m_Name == "PlayerNameInput")
               {
                  w->appendChar(event.text.text);
                  break;
               }
            }
         }
      }
      break;

      case SDL_KEYDOWN:
      {
         int sym = event.key.keysym.sym;
         switch (sym)
         {
            case SDLK_ESCAPE:
               if (m_Windows.back()->m_Name == "WGameplay")
               {
                  createWindow("WPause");
               }
               else if (m_Windows.back()->m_Name == "WPause")
               {
                  for (auto w: m_Windows.back()->m_Widgets)
                  {
                     if (w->m_Type == "CUIButton" && w->m_Name == "ExitButton")
                     {
                        w->leftClickUp();
                     }
                  }
               }
               else if (m_Windows.back()->m_Name == "WPlayerNameInput" && m_PlayerName.empty())
               {
                  goto exit;
               }
               else
               {
                  m_Windows.back()->m_DestroyMe = true;
               }
               break;

            case SDLK_BACKSPACE:
               if (m_Windows.back()->m_Name == "WPlayerNameInput")
               {
                  for (auto w: m_Windows.back()->m_Widgets)
                  {
                     if (w->m_Name == "PlayerNameInput")
                     {
                        w->backspace();
                        break;
                     }
                  }
               }

               break;
            case SDLK_LCTRL:
               m_SlipperGodmode = true;
               break;
         }
      }
      break;

      case SDL_KEYUP:
         if (event.key.keysym.sym == SDLK_LCTRL) m_SlipperGodmode = false;
         break;

      }
   }
   return false;
exit:
   return true;
}

void CEngine:: createWindow(std::string Name)
{
   CUIWindow* Window = new CUIWindow(Name);
   Window->initialize();
   if (!m_Windows.empty()) Window->m_Parent = m_Windows.back();
   m_Windows.push_back(Window);
}

void CEngine:: destroyWindow()
{
   m_ActiveObject = nullptr;
   CUIWindow* Window = m_Windows.back();
   m_Windows.pop_back();
   SAFE_DELETE(Window);
}

void CEngine:: loadRecords()
{
   std::fstream file ("records.dat", std::ios::in|std::ios::binary);
   if (file.is_open())
   {
//      file.read(reinterpret_cast<char*>(&AllTimeRecords[0]), AllTimeRecords.size() * sizeof(AllTimeRecords[0]));
      for (int i = 0; i < DEFAULTVECTORSIZE; i++) {
         file.read(reinterpret_cast<char*>(&AllTimeRecords[i].Name), sizeof AllTimeRecords[i].Name);
         file.read(reinterpret_cast<char*>(&AllTimeRecords[i].Score), sizeof AllTimeRecords[i].Score);
      }
      file.close();
   }
}

void CEngine:: saveRecords()
{
   std::fstream file ("records.dat", std::ios::out|std::ios::binary);
   if (file.is_open())
   {
//      file.write(reinterpret_cast<char*>(&AllTimeRecords[0]), AllTimeRecords.size() * sizeof(AllTimeRecords[0]));
      for (int i = 0; i < DEFAULTVECTORSIZE; i++)
      {
         file.write(reinterpret_cast<char*>(&AllTimeRecords[i].Name), sizeof AllTimeRecords[i].Name);
         file.write(reinterpret_cast<char*>(&AllTimeRecords[i].Score), sizeof AllTimeRecords[i].Score);
      }
      file.close();
   }
}

void CEngine:: writeRecordsToTextFile()
{
   std::fstream file ("records.txt", std::ios::out);
   if (file.is_open())
   {
      file << std::setw(MAXLENPLAYERNAME + 1) << std::left << "      Name" << std::setw(5) << "Score" << std::endl;
      for (int i = 0; i < DEFAULTVECTORSIZE; i++)
      {
         file << std::setw(MAXLENPLAYERNAME + 1) << std::left <<  AllTimeRecords[i].Name << std::setw(5) << std::right << AllTimeRecords[i].Score << std::endl;
      }
      file.close();
   }
}

void CEngine:: addNoteToRecords(std::string Name, int Score)
{
   addNote(AllTimeRecords, Name, Score);
   addNote(CurrentRecords, Name, Score);
}

template <typename Type>
void CEngine:: addNote(std::vector <Type> &Vector, std::string Name, int Score)
{
   for (int i = DEFAULTVECTORSIZE-1; i >= 0; i--)
   {
      if (Score <= Vector[i].Score) break;
      if (i == DEFAULTVECTORSIZE - 1)
      {
         snprintf(&(Vector[i].Name[0]), MAXLENPLAYERNAME, "%s", Name.c_str());
         Vector[i].Score = Score;
      }
      else
      {
         std::swap(Vector[i], Vector[i+1]);
      }
   }
}

