#pragma once

class CGame : public CObject
{
public:
   CGame();
   ~CGame();
   void updateState(void);
   void render(void);
   void clearRoaches(void);

   EGameState m_State;

   std::string m_PlayerName;
   int m_Score;
   ELevel m_Level;

   int32_t m_AllTime;
   uint32_t m_Timeout;
   uint32_t m_SlipperTimer;

   ESlipperState m_SlipperState;
   std::vector <SDL_Texture*> m_SlipperTexture {2, nullptr};
   SDL_Rect m_SlipperRect;

   std::vector <CRoach*> m_Roaches {ROACHCOUNT, nullptr};
   std::vector <int> m_Scores {10, 16, 26, 42};

};