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
Popis: Ruzne funkce
*/

#include "mouse3.h"

/*
==================================================
Ulozi konfiguraci a hi-score
==================================================
*/
void MISC_SaveCfg (void)
{
    int         j;
    myFile_s    *ff;

    ff = MY_FOpen (MOUSE3_DAT, 0, "wb", true);
    MY_FWrite (m3Opt, 1, sizeof (m3Opt), ff);
    MY_FWrite (&m3HiPoc, 4, 1, ff);
    for (j = 0; j < 10; j++)
    {
        MY_FWrite (&m3Hs[j].jmeno, 14, 1, ff);
        MY_FWrite (&m3Hs[j].bodu, 4, 1, ff);
    }
    MY_FClose (&ff);
}

/*
==================================================
Nahraje konfiguraci a hi-score
==================================================
*/
void MISC_LoadCfg (void)
{
    int         i, j;
    myFile_s    *ff;

    if (MY_FSize (MOUSE3_DAT) != 216)
    {
        for (i = 0; i < 10; i++)
        {
            memset (m3Hs[i].jmeno, 0, 14);
            m3Hs[i].bodu = 0;
        }
        printf ("Creating main data file\n");
        MISC_SaveCfg ();
    }

    ff = MY_FOpen (MOUSE3_DAT, 0, "rb", true);
    MY_FRead (m3Opt, 1, sizeof (m3Opt), ff);
    MY_FRead (&m3HiPoc, 4, 1, ff);
    for (j = 0; j < 10; j++)
    {
        MY_FRead (&m3Hs[j].jmeno, 14, 1, ff);
        MY_FRead (&m3Hs[j].bodu, 4, 1, ff);
    }
    MY_FClose (&ff);
}

/*
==================================================
Ulozi hru
==================================================
*/
void MISC_SaveGame (void)
{
    myFile_s *f;

    f = MY_FOpen (MOUSE3_SAV, 0, "wb", true);
    MY_FWrite (&m3Game, sizeof (m3game_s), 1, f);
    MY_FClose (&f);
}

/*
==================================================
Nahraje hru
==================================================
*/
void MISC_LoadGame (void)
{
    myFile_s *f;

    f = MY_FOpen (MOUSE3_SAV, 0, "rb", true);
    MY_FRead (&m3Game, sizeof (m3game_s), 1, f);
    MY_FClose (&f);
}

/*
==================================================
Prehraje animaci
==================================================
*/
void MISC_PlayAni (const char *file, int plFrom, int plTo, int wait)
{
    int     i, w = 0, t;

    memset (m3Data.scr, 0, 64000);
    i = plFrom;

    while (i <= plTo)
    {
        if ((t = CO_FpsSync ()) > 0)
        {
            if (w < 1)
            {
                LOAD_Ob5 (i, file, m3Data.scr, 64000);
                i++;
                w = wait;
            }
            else
                w -= t;
            DRAW_BlitScreen (0);
        }
    }
}
