#pragma once

class CVideo: public CObject
{
public:
   CVideo();
   ~CVideo();
   SDL_Window* m_Window = nullptr;
   int m_DisplayWidth, m_DisplayHeight, m_DisplayFPS;
   int m_GameWidth, m_GameHeight;
   uint32_t m_DisplayPixelFormat;
   SDL_Renderer* m_Renderer = nullptr;
};

