#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <cstdint>
#include <cstdbool>

#define SAFE_DELETE(obj) \
   delete obj; \
   obj = nullptr

#define LOGFILENAME "roachez.log"
#define WINDOWTITLE "EmpreAllie's Epic GameWindow                                                 -='\\|  ROACHEZ  |/'=-"
#define DEFAULTFONTSIZE    36
#define DEFAULTFONTFILENAME "./Fonts/foxtail.ttf";
#define DEFAULTFRAMEWIDTH  100

#define GAMEWIDTH   800
#define GAMEHEIGHT  600

#define MOUSEPOINTSIZE 10

#define MARGIN 2

#define PLAYERNAME "<EMPTY>"

#define DEFAULTVECTORSIZE  10
#define MAXLENPLAYERNAME   16

#define BUTTONWIDTH   200
#define BUTTONHEIGHT  100

#define ALLTIME    60
#define DELTATIME  1000

#define ROACHCOUNT 10

typedef enum
{
   ROACH_STAND, ROACH_RUN, ROACH_SMASHED, ROACH_DEAD
} ERoachState;

typedef enum
{
   GAME_FROZEN, GAME_PAUSED, GAME_RUNNING
} EGameState;

typedef enum
{
   SLIPPER_DEFAULT, SLIPPER_PRESSED
} ESlipperState;

typedef enum
{
	SOUND_SLAP, SOUND_SMASH, SOUND_DING, SOUND_TYPING, SOUND_SMASH_GOLDEN, SOUND_SIZE
} ESound;

typedef enum
{
   LEVEL1, LEVEL2, LEVEL3, LEVEL4
} ELevel;

#include "UIObject.h"
#include "UIStaticBitmap.h"
#include "UIButton.h"
#include "UIStaticText.h"
#include "UIEditText.h"
#include "UIWindow.h"

#include "Object.h"
#include "Debug.h"
#include "Roach.h"
#include "Video.h"
#include "Sound.h"
#include "Game.h"
#include "Engine.h"

double degToRad(double Dir);
double radToDeg(double DirInRad);
double normalizeAngle(double direction);
int random(int Min, int Max);
void Log(const char * format, ...);
void Logi(const char * format, ...);
void Logw(const char * format, ...);
void Loge(const char * format, ...);
//void pop_back_utf8(std::string& utf8);

extern CEngine* Engine;