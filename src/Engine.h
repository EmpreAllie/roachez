#pragma once

#include <vector>

class CEngine: public CObject
{
public:
   CEngine();
   ~CEngine();

   CDebug* m_Debug = nullptr;
   CVideo* m_Video = nullptr;
   CSound* m_Sound = nullptr;
   CGame* m_Game = nullptr;
 
   CUIStaticBitmap* m_UIStaticBitmap = nullptr;

   bool initialize(void);
   void loop(void);
   bool handleEvent(void);

   std::vector<CUIWindow*> m_Windows;
   void createWindow(std::string Name);
   void destroyWindow(void);

   typedef struct
   {
      char Name[MAXLENPLAYERNAME] = {0};
      int Score = 0;
   } recordsCurrent_t;

   typedef struct
   {
      char Name[MAXLENPLAYERNAME] = {0};
      int Score = 0;
   } recordsAllTime_t;

   std::vector <recordsCurrent_t> CurrentRecords {DEFAULTVECTORSIZE, {PLAYERNAME, 0}};
   std::vector <recordsAllTime_t> AllTimeRecords {DEFAULTVECTORSIZE, {PLAYERNAME, 0}};

   std::string m_PlayerName;
   void loadRecords(void);
   void saveRecords(void);
   void writeRecordsToTextFile(void);
   void addNoteToRecords(std::string Name, int Score);
   template <typename Type>
   void addNote(std::vector<Type>&, std::string, int);

   SDL_Rect m_MouseRect {0, 0, MOUSEPOINTSIZE, MOUSEPOINTSIZE};
   CUIObject* m_ActiveObject;

   bool m_SlipperGodmode;
};

