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
Popis: Jadro - zakladni funkce, vstupni bod
*/

#include <unordered_set>
#include "co_core.h"

// Deklarace procedur ktere musi byt nekde implementovany
void    P_Init          (void);
void    P_DeInit        (void);
void    P_Main          (void);
void    P_KeyEvent      (int key);
void    P_ActiveEvent   (bool active);

// Deklarace globalnich promenych
app_s       g_app;
appVid_s    g_vid;
std::unordered_set<SDL_Keycode> pressedKeys;

bool CO_IsKeyPressed(SDL_Keycode keyCode)
{
	return pressedKeys.find(keyCode) != pressedKeys.end();
}

/*
==================================================
Zpracovani eventu
==================================================
*/
void CO_ProcessEvents (void)
{
    SDL_Event   event;

    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
		case SDL_KEYDOWN:
			pressedKeys.insert(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			pressedKeys.erase(event.key.keysym.sym);
			break;
        case SDL_QUIT:
            g_app.flags |= APP_FLAG_QUIT;
            break;
//        case SDL_ACTIVEEVENT:
//            g_app.active = event.active.gain == 1;
//            P_ActiveEvent (event.active.gain == 1);
//            break;
        }
    }
}

/*
==================================================
Deinicializace jadra
==================================================
*/
static void CO_DeInit (void)
{
    MY_DeInit ();
}

/*
==================================================
Ulozeni obsahu konzoly na disk a uklid po chybe
==================================================
*/
static void CO_ErrorHandler (const char *str)
{
    P_DeInit ();
    VID_Shutdown ();
    CO_DeInit ();
    SDL_Quit ();
}

/*
==================================================
Inicializace jadra
==================================================
*/
static void CO_Init (void)
{
    // Inicializace promenych jadra
    g_app.flags = APP_FLAG_NONE;

    // Inicializace me knihovny
    MY_Init ();
    MY_AddPck (APP_FILE_CORE_PCK);
    MY_ErrCallback (CO_ErrorHandler);

    // Inicializace SDL
    if (SDL_Init (SDL_INIT_VIDEO) != 0)
        MY_Err (MY_ErrDump ("%S: %s\n", MY_L("COSTR0006|Chyba pri inicializaci grafickeho modu"), SDL_GetError()));

    // Inicializace jadra
    CO_FontLoad (APP_FILE_FONT);
}

/*
==================================================
Vstupni bod aplikace
==================================================
*/
int main (int argc, char *argv[])
{
    // Inicializace
    CO_Init ();
    VID_Init ();
    P_Init ();

    // Predani kontroly aplikaci
    P_Main ();

    // Deinicializace
    P_DeInit ();
    VID_Shutdown ();
    CO_DeInit ();

    SDL_Quit ();

    return 0;
}
