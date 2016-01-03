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
Projekt: The Mouse 3
Popis: Uvodni nastaveni
*/

#include <time.h>
#include <SDL2/SDL_video.h>
#include "mouse3.h"

/*
==================================================
Nastaveni grafickeho rezimu
==================================================
*/
static void SET_Graphics (void)
{
#ifndef M3_USE_OPENGL
    m3Screen.zoom = 3;

    m3SdlMainSurface = SDL_SetVideoMode (m3Screen.zoom * 320, m3Screen.zoom * 200, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (m3SdlMainSurface == NULL)
    {
        MY_Err ("Unable to create video memory SDL surface");
    }

#else

    // Zjisteni aktualniho rozliseni
	SDL_DisplayMode currentVideoMode;
	SDL_GetCurrentDisplayMode(0, &currentVideoMode);

    // Nastaveni grafickeho modu
    printf ("Opening graphics mode %dx%dx32\n", currentVideoMode.w, currentVideoMode.h);
    SDL_Delay (3000);
    VID_SetMode (currentVideoMode.w, currentVideoMode.h, 32, 0, true);

    // Spocitame zoom, abychom zabrali co nejvetsi cast obrazovky pri zachovani aspect ratia
    if (g_vid.cl_width / 320.0f > g_vid.cl_height / 200.0f)
    {
        m3Screen.zoom = g_vid.cl_height / 200.0f;
    }
    else
    {
        m3Screen.zoom = g_vid.cl_width / 320.0f;
    }

    glViewport (g_vid.cl_width / 2 - int(160 * m3Screen.zoom), g_vid.cl_height / 2 - int(100 * m3Screen.zoom), int(320 * m3Screen.zoom), int(200 * m3Screen.zoom));

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, 320, 200, 0, 0, 1);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    // Clear both buffers
    glDrawBuffer (GL_FRONT_AND_BACK);
    glClear (GL_COLOR_BUFFER_BIT);
    glDrawBuffer (GL_BACK);
#endif
}

/*
==================================================
Nahrani dat
==================================================
*/
static void SET_LoadData (void)
{
    int         i, j;
    myFile_s    *f;

    // Nahrani a nastaveni palety
    printf ("Loading palette : %s\n", MOUSE3_ART);
    MY_FLoad (MOUSE3_PAL, (myBYTE *) m3Data.pal);
    for (i = 0; i < 256; i++)
        DRAW_SetPal (i, m3Data.pal[i][0], m3Data.pal[i][1], m3Data.pal[i][2]);

    // Nahrani grafiky
    printf ("Loading art : %s\n", MOUSE3_ART);
    MY_FLoad (MOUSE3_ART, m3Data.art);

    // Nahrani priser
    printf ("Loading monsters (");
    for (i = 0; i < MONSTERS; i++)
    {
        MON_Load (i);
        printf (".");
    }

    // Nahrani animacnich dat
    printf (")\nLoading anm : %s\n", MOUSE3_ANM);
    j = (int) MY_FSize (MOUSE3_ANM);
    f = MY_FOpen (MOUSE3_ANM, 0, "rb", true);

    for (i = 0; i < j; i += 2)
    {
        MY_FRead (&m3Data.anm[i / 2], 1, 1, f);
        MY_FRead (&m3Data.znak[i / 2], 1, 1, f);
    }

    MY_FClose (&f);
    memset (m3Data.scr, 0, 64000);

    // Nahrani konfigurace
    printf ("Loading config file\n");
    MISC_LoadCfg ();
}

/*
==================================================
Nastaveni ukazatelu, alokace pameti
==================================================
*/
static void SET_Pointers (void)
{
    int     i;

    printf ("Loading memory\n");
    m3Data.art = (myBYTE *) MY_Alloc(MY_FSize(MOUSE3_ART));
    m3Data.anm = (myBYTE *) MY_Alloc(MY_FSize(MOUSE3_ANM) / 2);
    m3Data.scr = (myBYTE *) MY_Alloc(64000);
    m3Data.scr_gl = (myBYTE *) MY_Alloc(3 * 64000);
    m3Data.bcg = (myBYTE *) MY_Alloc(61445);
    for (i = 0; i < 20; i++)
        m3Data.spi[i] = NULL;
}

/*
==================================================
Deinit
==================================================
*/
void P_DeInit (void)
{
    int i;

    MISC_SaveCfg ();
    MY_Free (m3Data.art);
    MY_Free (m3Data.anm);
    MY_Free (m3Data.scr);
    MY_Free (m3Data.bcg);
    for (i = 0; i < MONSTERS; i++)
        MY_Free (m3Data.spi[i]);
}

/*
==================================================
Inicializacni procedura
==================================================
*/
void P_Init (void)
{
    srand ((unsigned) time (NULL));

    printf ("==== The Mouse 3 ====\nBy O.Danek (c) 2006\n");
    printf ("Starting TEEM 4 engine\n");
    printf ("Starting PCK engine\n");
    printf ("Main graphics file : %s\n", MOUSE3_PCK);
    printf ("Monsters data : %d\n", MONSTERS);
    printf ("Starting keyboard handler\n");
    printf ("Main data file : %s\n", MOUSE3_DAT);

    SET_Pointers ();
    SET_LoadData ();
    SET_Graphics ();

    if (m3Opt[7])
    {
        MISC_PlayAni (MOUSE3_INT, 1, 65, 7);
        M3_WaitFor (SDLK_RETURN);
    }

    DRAW_Logo ();
    memset (m3Data.scr, 0, 64000);
}
