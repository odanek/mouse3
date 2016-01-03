/*
* Copyright (c) 2006, Ondrej Danek (www.ondrej-danek.net)
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Ondrej Danek nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
Projekt: Sablona aplikace
Popis: Hlavni hlavickovy soubor jadra
*/

#ifndef __CO_CORE_H
#define __CO_CORE_H

/* SDL */
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/* Moje standardni knihovny */
#include "../mylib/mylib.h"

/* Makra a definice */
#define APP_VERSION         "2.0.0"
#define APP_NAME            "The Mouse 3"
#define APP_FILE_ICON       "data/mouse3_icon.bmp"
#define APP_FILE_CORE_PCK   "data/mouse3.pck"
#define APP_FILE_FONT       "data/mouse3.fon"
#define APP_FPS_SPEED       60

/* Flagy */
#define APP_FLAG_NONE       0
#define APP_FLAG_QUIT       1

/*
==================================================
Datove struktury
==================================================
*/
struct app_s
{
    float       fps;                // Aktualni pocet obrazku za sekundu
    bool        flags;              // Flagy aplikace
    bool        active;             // Je aplikace prave aktivni (ma focus?)
    SDL_Surface *screen_surf;       // SDL_surface pro obrazovku
    myBYTE      *font;              // Ukazatel na font
};

struct appVid_s
{
    bool        inited;             // Video inited?
	SDL_Window *window;				// SDL Window
	SDL_GLContext glContext; 		// OpenGL context

    int         cl_width;           // Client width
    int         cl_height;          // Client height

    float       gl_farclip;         // OpenGL far clipping plane
    float       gl_nearclip;        // OpenGL far clipping plane
    float       gl_fov;             // OpenGL field of view
    float       gl_aspect;          // OpenGL aspect - pokud je 0 tak dopocitat jako width/height
    float       gl_cdepth;          // OpenGL color depth - 0 je default
    float       gl_zdepth;          // OpenGL z-buffer depth
};

extern app_s    g_app;
extern appVid_s g_vid;

/*
==================================================
co_main.cpp
==================================================
*/
void            CO_ProcessEvents    (void);
bool 			CO_IsKeyPressed		(SDL_Keycode keyCode);

/*
==================================================
co_font.cpp
==================================================
*/
void            CO_FontLoad         (const char *fontFile);
void            CO_FontFree         (void);
const myBYTE    *CO_FontGet         (void);

/*
==================================================
co_fps.cpp
==================================================
*/
void            CO_FpsSyncLoops     (void (*move)(void), void (*draw)(void));
int             CO_FpsSync          (void);

/*
==================================================
co_vid.cpp
==================================================
*/
void            VID_SwapBuffers     (void);
void            VID_Init            (void);
void            VID_SetMode         (int w, int h, int bpp, int aa, bool full);
void            VID_Shutdown        (void);

#endif
