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
Popis: Vykreslovani
*/

#include "mouse3.h"

// Promenne
static SDL_Color	m3CurPal[256];
static myBYTE       m3TmpOkno[64000];

/*
==================================================
Vykresleni postav/spritu
==================================================
*/
void DRAW_Man(int naX, int naY, int faz, int sir, int vys, int svys, myBYTE *po)
{
    myBYTE  *fromP, *toP;
    int     sr = sir, vs = vys, adS = 0, adV = 0, ii;

    if (naX < 0)
    {
        adS = -naX;
        sr += naX;
        naX = 0;
    }

    if (naY < 0)
    {
        adV = -naY;
        vs += naY;
        naY = 0;
    }

    if (naX + sir > 320)
        sr -= naX + sir - 320;

    if (naY + vys > 200)
        vs -= naY + vys - 200;

    adV *= sir;


    toP = &m3Data.scr[(naY << 8) + (naY << 6) + naX];
    fromP = &po[sir * svys * faz + adV + adS];

    while (vs-- > 0)
    {
        for (ii = 0; ii < sr; ii++, fromP++, toP++)
            if (*fromP)
                *toP = *fromP;
        toP += 320 - sr;
        fromP += sir - sr;
    }
}

/*
==================================================
Vykresli informacni okno
==================================================
*/
void DRAW_Okno (int x, int y, int zx, int zy, int sr, int vs, bool show)
{
    myBYTE  *fromP, *toP;
    int ii;

    LOAD_Ob5 (5, MOUSE3_PIC, m3TmpOkno, 61440);

    toP = &m3Data.scr[y * 320 + x];
    fromP = &m3TmpOkno[zy * 320 + zx];

    while (vs--)
    {
        for (ii = 0; ii < sr; ii++, fromP++, toP++)
            *toP = *fromP;

        toP += 320 - sr;
        fromP += 320 - sr;
    }

    if (show)
        DRAW_BlitScreen (0);
}

/*
==================================================
Vykresli statistiku hrace, pocet naboju, ...
==================================================
*/
void DRAW_Stats (void)
{
    int     i, j;

    FONT_Set (m3Data.scr, false);
    DRAW_Okno (69, 49, 58, 0, 182, 110, false);
    FONT_WriteSF (82, 60, 9, "%d", m3Game.points);
    FONT_WriteSF (164, 60, 9, "%d", m3Game.bullets);
    if (m3Game.life - 1)
        for (i = 1; i < m3Game.life; i++)
            DRAW_Man(75 + 27 * (i - 1), 82, 0, m3Mon[0].Sir, m3Mon[0].Vys, m3Mon[0].Vys, m3Data.spi[0]);
    j = 77;
    for (i = 0; i < 4; i++)
        if (m3Game.key[i])
        {
            DRAW_Man (j, 129, KLICE + i * 2, 20, 20, 20, m3Data.art);
            j += 21;
        }

    DRAW_BlitScreen (0);
    M3_WaitFor (SDLK_RETURN);
    FONT_Set (m3Data.scr, true);
}

/*
==================================================
Najeti obrazovky
==================================================
*/
void DRAW_Najed (void)
{
    int     i = 0, t;

    memcpy (m3TmpOkno, m3Data.scr, 64000);
    while (i < 192)
    {
        if ((t = CO_FpsSync ()) > 0)
        {
            memset (m3Data.scr, 0, (192 - i) * 320);
            memcpy (&m3Data.scr[(192 - i) * 320], m3TmpOkno, i * 320);
            DRAW_BlitScreen (0);
            i += t * 2;
        }
    }
    memcpy (m3Data.scr, m3TmpOkno, 192 * 320);
    DRAW_BlitScreen (0);
}

/*
==================================================
Zajeti obrazovky
==================================================
*/
void DRAW_Zajed (void)
{
    int     i = 192, t;

    memcpy (m3TmpOkno, m3Data.scr, 64000);
    while (i > 0)
    {
        if ((t = CO_FpsSync ()) > 0)
        {
            memset (m3Data.scr, 0, (192 - i) * 320);
            memcpy (&m3Data.scr[(192 - i) * 320], m3TmpOkno, i * 320);
            DRAW_BlitScreen (0);
            i -= t * 2;
        }
    }
    memset (m3Data.scr, 0, 192 * 320);
    DRAW_BlitScreen (0);
}

/*
==================================================
Vykresleni horniho loga
==================================================
*/
void DRAW_Logo (void)
{
    int     x, y, p = 0;

    memset (m3Data.scr, 0, 64000);
    memset (m3Data.scr_gl, 0, 3 * 64000);

    DRAW_Man (135, 0, 73, 50, 8, 8, m3Data.art);

    for (y = 0; y < 8; y++)
    {
        myBYTE *scr = &m3Data.scr_gl[3 * 320 * (199 - y)]; 
        for (x = 0; x < 320; x++, p++)
        {
            *scr++ = m3CurPal[m3Data.scr[p]].r;
            *scr++ = m3CurPal[m3Data.scr[p]].g;
            *scr++ = m3CurPal[m3Data.scr[p]].b;
        }
    }
}

/*
==================================================
Vykresleni bufferu na obrazovku
==================================================
*/
void DRAW_BlitScreen (int start)
{
#ifndef M3_USE_OPENGL
    myBYTE *dst = &((myBYTE *)m3SdlMainSurface->pixels)[(8 * 320) * m3Screen.zoom * m3Screen.zoom];

    SDL_LockSurface (m3SdlMainSurface);

    for (size_t y = 0; y < 192; y++)
    {
        for (size_t zy = 0; zy < m3Screen.zoom; zy++)
        {
            myBYTE *src = &m3Data.scr[start + (y << 8) + (y << 6)];
            
            for (size_t x = 0; x < 320; x++, src++)
            {
                for (size_t zx = 0; zx < m3Screen.zoom; zx++)
                {
                    *dst++ = *src;
                }
            }
        }
    }

    SDL_UnlockSurface (m3SdlMainSurface);
    SDL_SetPalette (m3SdlMainSurface, SDL_PHYSPAL, m3CurPal, 0, 256);
    SDL_Flip (m3SdlMainSurface);

#else

    int     x, y;
    myBYTE  *p = &m3Data.scr[start];

    for (y = 0; y < 192; y++)
    {
        myBYTE *scr = &m3Data.scr_gl[3 * 320 * (191 - y)]; 
        for (x = 0; x < 320; x++, p++)
        {
            *scr++ = m3CurPal[*p].r;
            *scr++ = m3CurPal[*p].g;
            *scr++ = m3CurPal[*p].b;
        }
    }

    glRasterPos2f (0, 200);
    glPixelZoom (m3Screen.zoom, m3Screen.zoom);
    glDrawPixels (320, 200, GL_RGB, GL_UNSIGNED_BYTE, m3Data.scr_gl);

    VID_SwapBuffers ();
#endif
}

/*
==================================================
Nastaveni barev v palete
==================================================
*/
void DRAW_SetPal (int i, int r, int g, int b)
{
    m3CurPal[i].r = r;
    m3CurPal[i].g = g;
    m3CurPal[i].b = b;
}

/*
==================================================
Vycisteni obrazovky, obnoveni puvodni palety
==================================================
*/
void DRAW_Obnov (void)
{
    int     i;

    memset (m3Data.scr, 0, 64000);
    for (i = 180; i < 244; i++)
        DRAW_SetPal (i, m3Data.pal[i][0], m3Data.pal[i][1], m3Data.pal[i][2]);
}


////////////////////////////////////////////////////////////////////////////////
/*
==================================================
Vykresleni dveri
==================================================
*/
static void DRAW_Doors (void)
{
    int     i, rozX, rozY, mS;

    if (m3Door[0].A + m3Door[1].A + m3Door[2].A + m3Door[3].A == 0)
        return;

    for (i = 0; i < 4; i++)
        if ((m3Door[i].A > 0) && (m3Door[i].A < 40))
        {
            rozX = m3Door[i].X - m3Screen.X;
            mS = DVERE + m3Door[i].C * 2;
            if (m3Door[i].A < 20)
            {
                rozY = m3Door[i].Y + m3Door[i].A + 20 - m3Screen.Y;
                if ((rozX + 20 > 0) && (rozY + 20 - m3Door[i].A > 0) &&
                    (rozX < 320) && (rozY < 200))
                    DRAW_Man (rozX, rozY, mS + 1, 20, 20 - m3Door[i].A, 20, m3Data.art);
            }

            rozY = m3Door[i].Y + m3Door[i].A - m3Screen.Y;
            if ((rozX + 20 > 0) && (rozY + 40 - m3Door[i].A > 0) &&
                (rozX < 320) && (rozY < 200))
                DRAW_Man (rozX, rozY, mS, 20, 40 - m3Door[i].A, 20, m3Data.art);
        }
}

/*
==================================================
Vykresleni postav
==================================================
*/
static void DRAW_Mans (void)
{
    int     i, rozX, rozY, mC, mS;

    for (i = 0; i < m3Mans; i++)
    {
        rozX = m3Man[i].X - m3Screen.X;
        rozY = m3Man[i].Y - m3Screen.Y;
        mC = m3Man[i].Bt;
        mS = m3Man[i].F;
        if (!m3Man[i].SS)
            mS += m3Man[i].O * m3Mon[mC].FP;
        if (!i && m3Man[0].Si && !M3_M_ISFLAG(i,M3_F_DIED))
            mS++;
        if ((rozX + m3Mon[mC].Sir > 0) && (rozY + m3Mon[mC].Vys > 0) &&
            (rozX < 320) && (rozY < 200))
            DRAW_Man (rozX, rozY, mS, m3Mon[mC].Sir, m3Mon[mC].Vys, m3Mon[mC].Vys, m3Data.spi[mC]);
    }
}

/*
==================================================
Vykresleni bloku (dlazdic)
==================================================
*/
static void DRAW_Art (int x, int y, int IC)
{
    int     i, j;
    myBYTE  *sprite_ptr;

    IC = IC + m3Screen.nowA % (m3Data.anm[IC] + 1);
    sprite_ptr = &m3Data.art[400 * IC];

    for (j = 0; j < 20; j++, y++)
    {
        for (i = 0; i < 20; i++, x++, sprite_ptr++)
            if (*sprite_ptr && x >= 0 && x < 320 && y >= 0 && y < 192)
                m3Data.scr[(y << 8) + (y << 6) + x] = *sprite_ptr;
        x -= 20;
    }
}

/*
==================================================
Vykresleni viditelne casti urovne
==================================================
*/
static void DRAW_Level (bool predni)
{
    int     x = 0, y = 0, mx, my, b_add, IC;

    mx = m3Screen.X % 20;
    my = m3Screen.Y % 20;

    b_add = !mx ? 16 : 17;

    IC = 2 * ((m3Screen.Y / 20) * m3Lev.Sir + m3Screen.X / 20);
    if (predni)
        IC++;

    for (y = -my; y < 192; y += 20, IC += 2 * (m3Lev.Sir - b_add))
        for (x = -mx; x < 320; x += 20, IC += 2)
            if (m3Data.lev[IC] > 1)
                DRAW_Art (x, y, m3Data.lev[IC]);
}

/*
==================================================
Nastaveni pozice obrazovky
==================================================
*/
static void DRAW_SetScrPos (void)
{
    int     rozX = m3Man[0].X + m3Screen.adXX - m3Screen.X,
            rozY = m3Man[0].Y + m3Screen.adYY - m3Screen.Y;

    if (rozX < 115)
        m3Screen.X = MY_Max (MY_Min (m3Screen.X + rozX - 115, m3Screen.MAXX), 0);
    else if (rozX > 185)
        m3Screen.X = MY_Max (MY_Min (m3Screen.X + rozX - 185, m3Screen.MAXX), 0);

    if (rozY < 50)
        m3Screen.Y = MY_Max (MY_Min (m3Screen.Y + rozY - 50, m3Screen.MAXY), 0);
    else if (rozY > 115)
        m3Screen.Y = MY_Max (MY_Min (m3Screen.Y + rozY - 115, m3Screen.MAXY), 0);
}

/*
==================================================
Vykresleni obrazovky
==================================================
*/
void DRAW_Screen (bool show)
{
    // Vykresleni pozadi
    memcpy (m3Data.scr, m3Data.bcg, 61440);

    DRAW_SetScrPos ();

    // Vykresleni popredi
    DRAW_Level (false);
    DRAW_Mans ();
    SHOT_DrawAll ();
    DRAW_Doors ();
    DRAW_Level (true);

    if (show)
        DRAW_BlitScreen (0);
}
