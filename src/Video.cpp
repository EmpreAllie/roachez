#include "roachez.h"

CVideo:: CVideo()
{
   m_Type = "CVideo";
   SDL_DisplayMode mode;
   if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
   {
      IsInitialized = false;
      Loge("%s", SDL_GetError());
   }
   Logi("Display resolution: %d x %d (%.2f), refresh rate: %d", mode.w, mode.h, (double)mode.w / mode.h, mode.refresh_rate);

   m_DisplayWidth = mode.w;
   m_DisplayHeight = mode.h;
   m_DisplayFPS = mode.refresh_rate;
   m_DisplayPixelFormat = mode.format;

   m_GameWidth = GAMEWIDTH;
   m_GameHeight = GAMEHEIGHT;

   SDL_Rect UsableBounds;
   SDL_GetDisplayUsableBounds(0, &UsableBounds);
   Logi("Display usable bounds:   X: %d, Y: %d - %d x %d (%.2f)", UsableBounds.x, UsableBounds.y, UsableBounds.w, UsableBounds.h, (double)UsableBounds.w / UsableBounds.h);

   while(m_GameWidth + GAMEWIDTH < UsableBounds.w && m_GameHeight + GAMEHEIGHT < UsableBounds.h)
   {
      m_GameWidth += GAMEWIDTH;
      m_GameHeight += GAMEHEIGHT;
   }

   Logi("Game resolution %d x %d (%.2f)", m_GameWidth, m_GameHeight, (double)m_GameWidth / m_GameHeight);
   int flags = 0;

   m_Window = SDL_CreateWindow(WINDOWTITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_GameWidth, m_GameHeight, flags | SDL_WINDOW_HIDDEN);

   SDL_Surface* IconSurface = nullptr;  
   IconSurface = IMG_Load("./Images/Icon.png");
	if (IconSurface)
	{
		SDL_SetWindowIcon(m_Window, IconSurface);
		SDL_FreeSurface(IconSurface);
	}

   int top, left, bottom, right;
   SDL_GetWindowBordersSize(m_Window, &top, &left, &bottom, &right);
   Logi("Window borders - top: %d left: %d, bottom: %d, right: %d", top, left, bottom, right);

   if (UsableBounds.w < m_GameWidth + left + right || UsableBounds.h < m_GameHeight + top + bottom)
   {
      m_GameWidth -= GAMEWIDTH;
      m_GameHeight -= GAMEHEIGHT;
      SDL_SetWindowSize(m_Window, m_GameWidth, m_GameHeight);
      SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
      Logi("After resize  game %d x %d (%.2f)", m_GameWidth, m_GameHeight, (double)m_GameWidth / m_GameHeight);
   }

   m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
   SDL_RenderSetIntegerScale(m_Renderer, SDL_TRUE);
   SDL_RenderSetLogicalSize(m_Renderer, GAMEWIDTH, GAMEHEIGHT);

   SDL_ShowWindow(m_Window);
}

CVideo:: ~CVideo()
{
   SDL_DestroyRenderer(m_Renderer);
   SDL_DestroyWindow(m_Window);
}

