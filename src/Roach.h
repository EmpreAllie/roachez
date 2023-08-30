#pragma once

#include <string>
#include <cmath>

#define ROACHSIZE    64
#define DELTAANGLE   60

#define MAXDELAY     60 // 30
#define MINDELAY     30 // 15
#define SCAREDDELAY  10
#define PANICDELAY   5
#define DELAYSTEP    4

#define RADIUSSAFE         200
#define RADIUSSLIPPERSLAP  110
#define RADIUSSLIPPERSMASH 50
#define RADIUSSLIPPERMOVE  140
#define RUNTEXTURECOUNT    2
#define SPRITESTEP         5
#define AGONYTIME          2000

class CRoach
{
public:
   CRoach(void);
   ~CRoach(void);

   std::string m_Type;
   std::string m_Name;

   ERoachState m_State;

   SDL_Texture* m_TextureStand;
   SDL_Texture* m_TextureSmashed;
   std::vector <SDL_Texture*> m_TextureRun {RUNTEXTURECOUNT, nullptr};
   SDL_Rect m_DstRect;

   double m_Dir;

   int m_SpriteDelay;
   int m_Scale;
   int m_SpriteIndex;
   uint32_t m_StandTimer;
   uint32_t m_RunTimer;
   uint32_t m_SmashedTimer;
   uint32_t m_SpriteTimer;
   double m_X, m_Y;
   bool m_IsInRadius;
   bool m_IsChangeDir;
   bool m_IsGolden;

   void update(void);
   void render(SDL_Renderer* Renderer);
   void changeDir(void);
};

