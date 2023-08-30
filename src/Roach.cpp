#include "roachez.h"

CRoach:: CRoach()
{
   m_Type = "CRoach";
   m_Name = "Roach";

   m_State = ROACH_STAND;
   m_StandTimer = random(350, 650) + SDL_GetTicks();
   m_RunTimer = m_SmashedTimer = 0;
   m_SpriteTimer = SDL_GetTicks();
   m_Scale = random(ROACHSIZE - 10, ROACHSIZE + 20);
   m_IsInRadius = false;
   m_IsChangeDir = false;
   m_IsGolden = random(1, 100) == 1 ? true : false;

   char buf[256];
   snprintf(buf, 256, "./Images/Roach%sStand.png", m_IsGolden ? "Golden" : "");
   m_TextureStand = IMG_LoadTexture(Engine->m_Video->m_Renderer, buf);

   snprintf(buf, 256, "./Images/Roach%sSmashed.png", m_IsGolden ? "Golden" : "");
   m_TextureSmashed = IMG_LoadTexture(Engine->m_Video->m_Renderer, buf);

   for (size_t i = 0; i < m_TextureRun.size(); i++)
   {
      snprintf(buf, 256, "./Images/Roach%sRun%u.png", m_IsGolden ? "Golden" : "", i);
      m_TextureRun[i] = IMG_LoadTexture(Engine->m_Video->m_Renderer, buf);
   }
   m_SpriteIndex = 0;
   m_SpriteDelay = random(MINDELAY - Engine->m_Game->m_Level * DELAYSTEP, MAXDELAY - Engine->m_Game->m_Level * DELAYSTEP);

   m_X = m_Y = 0;
   int n = random(0, 3);
   if (n == 0)
   {
      m_X = random(0, GAMEWIDTH);
      m_Y = -ROACHSIZE;
      m_Dir = random(180 - DELTAANGLE, 180 + DELTAANGLE);
   }
   else if (n == 1)
   {
      m_X = GAMEWIDTH + ROACHSIZE;
      m_Y = random(0, GAMEHEIGHT);
      m_Dir = random(270 - DELTAANGLE, 270 + DELTAANGLE);
   }
   else if (n == 2)
   {
      m_X = random(0, GAMEWIDTH);
      m_Y = GAMEHEIGHT + ROACHSIZE;
      m_Dir = random(0 - DELTAANGLE, 0 + DELTAANGLE);
   }
   else if (n == 3)
   {
      m_X = -ROACHSIZE;
      m_Y = random(0, GAMEHEIGHT);
      m_Dir = random(90 - DELTAANGLE, 90 + DELTAANGLE);
   }
   m_Dir = normalizeAngle(m_Dir);
   m_DstRect = {lround(m_X), lround(m_Y), m_Scale, m_Scale};
}

CRoach:: ~CRoach()
{
   SDL_DestroyTexture(m_TextureStand);
   SDL_DestroyTexture(m_TextureSmashed);
   for (size_t i = 0; i < m_TextureRun.size(); i++)
   {
      SDL_DestroyTexture(m_TextureRun[i]);
   }
}

void CRoach:: update()
{
// сброс тапка в дефолтное состояние
   if (SDL_GetTicks() > Engine->m_Game->m_SlipperTimer && Engine->m_Game->m_SlipperState == SLIPPER_PRESSED && !Engine->m_SlipperGodmode)
   {
      Engine->m_Game->m_SlipperState = SLIPPER_DEFAULT;
   }

   double d = sqrt( pow(m_X - Engine->m_MouseRect.x, 2) + pow(m_Y - Engine->m_MouseRect.y, 2) );
// поведение тараканов на втором уровне и выше
   if (Engine->m_Game->m_Level >= LEVEL2 && Engine->m_Game->m_SlipperState == SLIPPER_PRESSED && d <= RADIUSSLIPPERSLAP && m_State != ROACH_SMASHED)
   {
      changeDir();
      m_SpriteDelay = PANICDELAY;
   }

// последний уровень
   else if (Engine->m_Game->m_Level >= LEVEL4 && m_SpriteDelay != PANICDELAY && m_SpriteDelay != SCAREDDELAY && d <= RADIUSSLIPPERMOVE && m_State != ROACH_SMASHED)
   {
      changeDir();
      m_SpriteDelay = SCAREDDELAY;
   }

// если таракан убежал от тапка на достаточное расстояние
   else if (Engine->m_Game->m_Level >= LEVEL2 && (m_SpriteDelay == SCAREDDELAY || m_SpriteDelay == PANICDELAY) && d > RADIUSSAFE)
   {
      m_SpriteDelay = random(MINDELAY - Engine->m_Game->m_Level * DELAYSTEP, MAXDELAY - Engine->m_Game->m_Level * DELAYSTEP);
   }

   if (m_IsGolden && ((Engine->m_Game->m_SlipperState == SLIPPER_PRESSED && d <= RADIUSSLIPPERSLAP) || d <= RADIUSSLIPPERMOVE) && m_State != ROACH_SMASHED)
   {
      changeDir();
   }

   if (m_IsGolden)
   {
      m_SpriteDelay = SCAREDDELAY;
   }

// если попали тапком по таракану
   if (Engine->m_Game->m_SlipperState == SLIPPER_PRESSED && d < RADIUSSLIPPERSMASH && (m_State == ROACH_STAND || m_State == ROACH_RUN))
   {
   	m_IsGolden ? Engine->m_Sound->playSound(ESound::SOUND_SMASH_GOLDEN) : Engine->m_Sound->playSound(ESound::SOUND_SMASH);
      m_State = ROACH_SMASHED;
      m_SmashedTimer = SDL_GetTicks() + AGONYTIME;
      Engine->m_Game->m_Score += m_IsGolden ? 1000 : Engine->m_Game->m_Scores[Engine->m_Game->m_Level];
   }

// запуск из стоячего положения
   else if (m_State == ROACH_STAND && SDL_GetTicks() > m_StandTimer)
   {
      m_State = ROACH_RUN;
      m_RunTimer = m_IsGolden ? random(800, 1600) + SDL_GetTicks() : random(1600, 3600) + SDL_GetTicks();
      m_SpriteIndex = 0;
   }

// изменение направления движения
   else if ((Engine->m_Game->m_Level >= LEVEL3 || m_IsGolden) && m_State == ROACH_RUN && SDL_GetTicks() > m_RunTimer)
   {
      m_State = ROACH_STAND;
      m_StandTimer = (m_IsGolden ? random(200, 400) : random(350, 650)) + SDL_GetTicks();
      m_IsChangeDir = true;
   }

// смерть таракана
   else if (m_State == ROACH_SMASHED && SDL_GetTicks() > m_SmashedTimer)
   {
      m_State = ROACH_DEAD;
   }

// описание бега таракана
   if (m_State == ROACH_RUN)
   {
      if (m_IsChangeDir)
      {
         m_IsChangeDir = false;
         m_Dir = normalizeAngle(random(0, 360));
      }

      if (SDL_GetTicks() > m_SpriteTimer)
      {
         int q = (int)m_Dir / 90;
         if (q == 0)
         {
            m_X += fabs(sin(degToRad(m_Dir))) * SPRITESTEP;
            m_Y -= fabs(cos(degToRad(m_Dir))) * SPRITESTEP;
         }
         else if (q == 1)
         {
            m_X += fabs(cos(degToRad(m_Dir))) * SPRITESTEP;
            m_Y += fabs(sin(degToRad(m_Dir))) * SPRITESTEP;
         }
         else if (q == 2)
         {
            m_X -= fabs(sin(degToRad(m_Dir))) * SPRITESTEP;
            m_Y += fabs(cos(degToRad(m_Dir))) * SPRITESTEP;
         }
         else if (q == 3)
         {
            m_X -= fabs(cos(degToRad(m_Dir))) * SPRITESTEP;
            m_Y -= fabs(sin(degToRad(m_Dir))) * SPRITESTEP;
         }
         m_DstRect = {lround(m_X), lround(m_Y), m_Scale, m_Scale};

         ++m_SpriteIndex;
         m_SpriteIndex %= m_TextureRun.size();
         m_SpriteTimer = SDL_GetTicks() + m_SpriteDelay;
      }
   }
}

void CRoach:: render(SDL_Renderer* Renderer)
{
   SDL_Texture* Texture = nullptr;
   if (m_State == ROACH_STAND)
   {
      Texture = m_TextureStand;
   }
   else if (m_State == ROACH_SMASHED)
   {
      static uint32_t timeout = SDL_GetTicks();

      if (SDL_GetTicks() >= timeout)
      {
         uint8_t alpha = 0;
         if (SDL_GetTextureAlphaMod(m_TextureSmashed, &alpha) == 0)
         {
            if (alpha >= 2)
            {
               alpha -= 2;
               SDL_SetTextureAlphaMod(m_TextureSmashed, alpha);
            }
         }
         timeout += AGONYTIME / alpha;
      }
      Texture = m_TextureSmashed;
   }
   else if (m_State == ROACH_RUN)
   {
      Texture = m_TextureRun[m_SpriteIndex];
   }
   if (Texture) SDL_RenderCopyEx(Renderer, Texture, NULL, &m_DstRect, m_Dir, NULL, SDL_FLIP_NONE);
}

void CRoach:: changeDir()
{
   m_Dir = radToDeg(acos((Engine->m_MouseRect.y - m_Y) / sqrt( pow(Engine->m_MouseRect.x - m_X, 2) + pow(Engine->m_MouseRect.y - m_Y, 2))));

   /// because acos() returns value in range [0; Pi]
   if (Engine->m_MouseRect.x > m_X)
   {
      m_Dir = normalizeAngle(m_Dir * -1);
   }
}