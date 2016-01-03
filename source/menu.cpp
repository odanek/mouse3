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
Popis: Menu
*/

#include "mouse3.h"

// Promenne
struct hvDat
{
    int X;
    int Y;
    int Spd;
    int Now;
};

struct meItem
{
    char name[30];
    char va1[30];
    int va2;
    int styl;
};

static hvDat   hvezda[900];
static meItem  mItem[10];

//*****************************HVEZDY**************************************

/*
==================================================
Pocatecni nastaveni hvezdy
==================================================
*/
static void HVEZDY_SetOne (int kt)
{
    hvezda[kt].X = 319;
    hvezda[kt].Y = rand () % 184 + 8;
    hvezda[kt].Spd = rand() % 63 + 1;
    hvezda[kt].Now = 0;
}

/*
==================================================
Inicializace hvezd
==================================================
*/
static void HVEZDY_Init (void)
{
    int     j;

    for (j = 0; j < 900; j++)
    {
        HVEZDY_SetOne (j);
        hvezda[j].X = rand () % 319;
    }
}

/*
==================================================
Posun hvezd
==================================================
*/
static void HVEZDY_Move (void)
{
    int     i;

    for (i = 0; i < 900; i++)
    {
        hvezda[i].Now += 5;

        if (hvezda[i].Now > hvezda[i].Spd)
        {
            hvezda[i].Now = 0;
            hvezda[i].X--;
        }

        if (hvezda[i].X < 0)
            HVEZDY_SetOne (i);
    }
}

/*
==================================================
Vykresleni hvezd
==================================================
*/
static void HVEZDY_Draw (void)
{
    int     i;

    memset (m3Data.scr, 0, 64000);
    for (i = 0; i < 900; i++)
        m3Data.scr[hvezda[i].Y * 320 + hvezda[i].X] = 244 - hvezda[i].Spd;
}

//************************************************************************

//*****************************MENU***************************************

/*
==================================================
Hi scores
==================================================
*/
static void MENU_HiScores (void)
{
    int     i, j, press = WAITTIME, t;

    do
    {
        CO_ProcessEvents ();

        if ((t = CO_FpsSync ()) > 0)
        {
            while (t-- > 0)
            {
                HVEZDY_Move ();
                if (press > 0)
                    press--;
            }

            HVEZDY_Draw ();
            FONT_Set (m3Data.scr, false);
            i = 128;
            FONT_WriteSF (i,25,10,"HISCORES");
            FONT_WriteSF (i,23,10,"HISCORES");
            FONT_WriteSF (i+1,24,10,"HISCORES");
            FONT_WriteSF (i-1,24,10,"HISCORES");
            FONT_WriteSF (i,24,2,"HISCORES");
            FONT_WriteSF (144, 176, 40, "Zpet");
            FONT_Set (m3Data.scr, true);
            for (j = 0; j < m3HiPoc; j++)
            {
                if (strlen (m3Hs[j].jmeno))
                FONT_WriteSF (5, j + 7, 31, m3Hs[j].jmeno);
                FONT_WriteSF (30, j + 7, 32, "%ld", m3Hs[j].bodu);
            }
            DRAW_BlitScreen (2560);
        }
    } while (!CO_IsKeyPressed(SDLK_RETURN) || press);
}

/*
==================================================
Vrati zmacknutou klavesu
==================================================
*/
static int MENU_GetKey (void)
{
    int         l;
    static int  pism[26] =
         { SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, SDLK_i,
           SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r,
           SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y, SDLK_z };

    for (l = 0; l < 26; l++)
        if (CO_IsKeyPressed(pism[l]))
            return l;
    return -1;
}

/*
==================================================
Vstupni pole pro zadani jmena
==================================================
*/
static void MENU_Input (int y, char col, char max, char *str)
{
    int  zmc, delka = 0, press = 1, t;

    FONT_Set (m3Data.scr, true);
    do
    {
        if ((t = CO_FpsSync ()) > 0)
        {
            while (t-- > 0)
            {
                CO_ProcessEvents ();

                if (!press)
                {
                    if (CO_IsKeyPressed(SDLK_BACKSPACE))
                    {
                        str[delka] = 0;
                        if (delka)
                            delka--;
                        press = WAITTIME;
                    }
                    if (CO_IsKeyPressed(SDLK_SPACE) && delka < max)
                    {
                        str[delka++] = ' ';
                        press = WAITTIME;
                    }
                    if ((zmc = MENU_GetKey()) > -1 && delka < max)
                    {
                        str[delka++] = zmc + 65;
                        press = WAITTIME;
                    }
                }

                HVEZDY_Move ();
                if (press > 0)
                    press--;

                str[delka] = '_'; str[ delka + 1 ] = 0;
            }

            HVEZDY_Draw ();
            FONT_WriteSF (13, 5, 32, "Zadej svoje jmeno");
            FONT_WriteSF (20 - delka / 2, y, col, &str[0]);
            DRAW_BlitScreen (2560);
        }
    } while (!CO_IsKeyPressed(SDLK_RETURN));
    str[delka] = 0;
}

/*
==================================================
Pridani noveho nejlepsiho vysledku
==================================================
*/
void MENU_AddHiScore (void)
{
    int  j, naPoz = 10;

    for (j = 1; j < 64; j++)
        DRAW_SetPal (180+j, j * 255 / 63, j * 255 / 63, j * 255 / 63);
    HVEZDY_Move ();
    DRAW_Najed ();

    for (j = 9; j >= 0; j--)
        if (m3Game.points > m3Hs[j].bodu)
            naPoz = j;
        else
            break;

    if (naPoz == 10) return;
    if (naPoz < 9) for (j = 8; j >= naPoz; j--)
        memcpy (&m3Hs[j + 1], &m3Hs[j], sizeof (m3hs_s));
    MENU_Input (12, 31, 12, &m3Hs[naPoz].jmeno[0]);
    m3Hs[naPoz].bodu = m3Game.points;
    if (m3HiPoc < 10)
        m3HiPoc++;
    MISC_SaveCfg ();
    MENU_HiScores ();
}

/*
==================================================
Vykresleni polozek menu
==================================================
*/
void MENU_DrawItems (int max, int cur_item, int x_pos)
{
    int     i, j;

    FONT_Set (m3Data.scr, false);
    i = (20 - (int)strlen(mItem[0].name) / 2) * 8;
    FONT_WriteSF (i, 57, 10, mItem[0].name);
    FONT_WriteSF (i, 55, 10, mItem[0].name);
    FONT_WriteSF (i+1, 56, 10, mItem[0].name);
    FONT_WriteSF (i-1, 56, 10, mItem[0].name);
    FONT_WriteSF (i, 56, 2, mItem[0].name);
    FONT_Set (m3Data.scr, true);

    for (i = 1; i <= max; i++)
    {
        if (i == cur_item) j = 40; else j = 30;
        if (mItem[i].styl == 3) FONT_WriteSF (x_pos, 10 + i, j, mItem[i].name);
        if (mItem[i].styl == 2) FONT_WriteSF (x_pos, 10 + i, j, "%s%d", mItem[i].name, mItem[i].va2);
        if (mItem[i].styl == 1) FONT_WriteSF (x_pos, 10 + i, j, "%s%s", mItem[i].name, mItem[i].va1);
    }
}

/*
==================================================
Pohyb v menu
==================================================
*/
void MENU_Control (int max, int &cur_item, int x_pos)
{
    int     press = 2 * WAITTIME, t;

    do
    {        
        if ((t = CO_FpsSync()) > 0)
        {
            while (t-- > 0)
            {
                CO_ProcessEvents ();

                HVEZDY_Move ();
                if (press > 0)
                    press--;

                if (!press)
                {
                    if ((CO_IsKeyPressed(SDLK_UP)) && (cur_item > 1))
                    {
                        cur_item--;
                        press = WAITTIME;
                    }
                    if ((CO_IsKeyPressed(SDLK_DOWN)) && (cur_item < max))
                    {
                        cur_item++;
                        press = WAITTIME;
                    }
                }
            }

            HVEZDY_Draw ();
            MENU_DrawItems (max, cur_item, x_pos);
            DRAW_BlitScreen (2560);
        }
    } while (!CO_IsKeyPressed(SDLK_RETURN) || press);
}

/*
==================================================
Volby
==================================================
*/
static void MENU_Options (void)
{
    int     i, sselect;
    const char *vAN[2] = { "Ne", "Ano" };

    for (i = 0; i < 10; i++)
        mItem[i].styl = 4;
    sselect = 1;

    while (sselect != 8)
    {
        m3Opt[2] = 0;
        strcpy (mItem[0].name, "VOLBY"); mItem[0].styl = 3;
        strcpy (mItem[1].name, "Pocet zivotu : "); mItem[1].va2 = m3Opt[0]; mItem[1].styl = 2;
        strcpy (mItem[2].name, "Zvuky        : "); strcpy (mItem[2].va1, vAN[m3Opt[1]]); mItem[2].styl = 1;
        strcpy (mItem[3].name, "Hudba        : "); strcpy (mItem[3].va1, vAN[m3Opt[2]]); mItem[3].styl = 1;
        strcpy (mItem[4].name, "Pocet strel  : "); mItem[4].va2 = m3Opt[3]; mItem[4].styl = 2;
        strcpy (mItem[5].name, "Nesmrtelnost : "); strcpy (mItem[5].va1, vAN[m3Opt[4]]); mItem[5].styl = 1;
        strcpy (mItem[6].name, "Auto save    : "); strcpy (mItem[6].va1, vAN[m3Opt[5]]); mItem[6].styl = 1;
        strcpy (mItem[7].name, "Intro        : "); strcpy (mItem[7].va1, vAN[m3Opt[7]]); mItem[7].styl = 1;
        strcpy (mItem[8].name, "OK"); mItem[8].styl = 3;

        MENU_Control (8, sselect, 14);

        if (sselect == 1) if (++m3Opt[0] > 4)  m3Opt[0] = 1;
        if (sselect == 4) if (++m3Opt[3] > 10) m3Opt[3] = 1;
        if (sselect > 1 && sselect < 7 && sselect !=4) m3Opt[sselect - 1] = 1 - m3Opt[sselect - 1];
        if (sselect == 7) m3Opt[7] = 1 - m3Opt[7];
        if (!m3Opt[6]) m3Opt[4] = 0;
        if (sselect == 8) return;
    }
}

/*
==================================================
Vyber kapitoly
==================================================
*/
static void MENU_Chapter (void)
{
    int     i, sselect;

    m3MenuVal = 3;
    for (i = 0; i < 10; i++)
        mItem[i].styl = 4;
    sselect = 1;

    strcpy (mItem[0].name, "HRAT KAPITOLU"); mItem[0].styl = 3;
    strcpy (mItem[1].name, "Tajuplny les"); mItem[1].styl = 3;
    strcpy (mItem[2].name, "Katakomby");  mItem[2].styl = 3;
    strcpy (mItem[3].name, "Caliguluv zamek"); mItem[3].styl = 3;
    strcpy (mItem[4].name, "Chladny svet"); mItem[4].styl = 3;
    strcpy (mItem[5].name, "Zrus"); mItem[5].styl = 3;

    MENU_Control (5, sselect, 16);

    if (sselect == 5)
        return;

    m3Game.points = 0;
    m3Game.exp = 50000;
    m3Game.life = m3Opt[0] + 1;
    m3Game.bullets = m3Opt[3];
    m3Game.level = 1;
    m3Game.stage = sselect;

    DRAW_Zajed ();
    DRAW_Obnov ();
    DRAW_BlitScreen (0);
    HLAVNI_PlayGame ();
}

/*
==================================================
Arkada
==================================================
*/
static void MENU_Arcade (void)
{
    int     i, sselect;

    for (i = 0; i < 10; i++)
        mItem[i].styl = 4;

    sselect = 1;

    strcpy (mItem[0].name, "ARKADA"); mItem[0].styl = 3;
    strcpy (mItem[1].name, "Nova hra"); mItem[1].styl = 3;
    strcpy (mItem[2].name, "Pokracovat"); mItem[2].styl = 3;
    strcpy (mItem[3].name, "Zrus"); mItem[3].styl = 3;

    ARKADA1:

    MENU_Control (3, sselect, 17);

    if (sselect == 1)
    {
        m3Game.points = 0;
        m3Game.exp = 50000;
        m3Game.life = m3Opt[0] + 1;
        m3Game.bullets = m3Opt[3];
        m3Game.level = 1;
        m3Game.stage = 1;

        DRAW_Zajed ();
        DRAW_Obnov ();
        DRAW_BlitScreen (0);
        m3MenuVal = 1;
        HLAVNI_PlayGame ();
    }

    if (sselect == 2)
    {
        if (MY_FSize (MOUSE3_SAV) > 15)
        {
            MISC_LoadGame ();
            DRAW_Zajed ();
            DRAW_Obnov ();
            DRAW_BlitScreen (0);
            m3MenuVal = 2;
            HLAVNI_PlayGame ();
        }
        else
            goto ARKADA1;
    }
}

/*
==================================================
Credits
==================================================
*/
static void MENU_Credits (void)
{
    typedef struct { const char *tex; int col; } lineDef;
    lineDef lin[33] =
          { { "THE MOUSE 3", 32 },
            { " ", 0           },
            { "PROGRAM", 44 },
            { "Ondrej Danek", 31 },
            { " ", 0 },
            { "GRAFIKA A ANIMACE", 44 },
            { "Ondrej Danek", 31 },
            { " ", 0 },
            { "INTRO", 44 },
            { "Ondrej Danek", 31 },
            { " ", 0 },
            { "ZVUKY", 44 },
            { "Ondrej Danek", 31 },
            { " ", 0 },
            { "DESIGN LEVELU", 44 },
            { "Kapitola 1 - Ondrej Danek", 31 },
            { "Kapitola 2 - Ondrej Danek", 31 },
            { "Kapitola 3 - Ondrej Danek", 31 },
            { "Kapitola 4 - Ondrej Danek", 31 },
        { " ", 0 },
        { "...proste cely jsem to udelal ja", 46 },
            { " ", 0 },
            { "PODEKOVANI", 44 },
            { "ID Soft - Commander Keen", 31},
            { "Dement - Programovani", 31},
        { "DJ Delorie - DJGPP", 31 },
            { " ", 0 },
            { " ", 0 },
            { "Ondrej Danek", 40 },
            { "(c) 2008", 40 } };
    int     lines = 30, zcL = 0, psN = 0, naR = 24, k = 0, l, t;

    FONT_Set (m3Data.scr, false);

    while (k < 24 + lines)
    {
        if ((t = CO_FpsSync()) > 0)
        {
            while (t-- > 0)
            {
                HVEZDY_Move ();
                if (++psN > 7)
                {
                    psN = 0;
                    k++;
                    if (--naR < 1)
                    {
                        naR = 1;
                        zcL++;
                    }
                }
            }

            HVEZDY_Draw ();
            for (l = zcL; l < lines; l++)
                if (naR + l - zcL < 25)
                    FONT_WriteSF ((20 - (int)strlen (lin[l].tex) / 2) * 8, (naR + l - zcL) * 8 - psN, lin[l].col, lin[l].tex);
            memset (&m3Data.scr[61440], 0, 2560);
            DRAW_BlitScreen (2560);
        }
    }
}

/*
==================================================
Vstupni bod menu i celeho programu
==================================================
*/
void P_Main (void)
{
    int     i, j, sselect;
    bool    first = true;

    HVEZDY_Init ();

    sselect = 1;

    m3Game.points = 0;
    m3Game.life = 4;
    m3Game.exp = 50000;
    m3Game.bullets = 5;
    m3Game.level = 1;
    m3Game.stage = 1;

    while (sselect != 6)
    {
        for (i = 0; i < 10; i++)
            mItem[i].styl = 4;

        strcpy (mItem[0].name, "HLAVNI MENU"); mItem[0].styl = 3;
        strcpy (mItem[1].name, "Arkada"); mItem[1].styl = 3;
        strcpy (mItem[2].name, "Hrat kapitolu"); mItem[2].styl = 3;
        strcpy (mItem[3].name, "Volby"); mItem[3].styl = 3;
        strcpy (mItem[4].name, "Credits"); mItem[4].styl = 3;
        strcpy (mItem[5].name, "Hiscores"); mItem[5].styl = 3;
        strcpy (mItem[6].name, "Konec"); mItem[6].styl = 3;

        for (j = 1; j < 64; j++)
            DRAW_SetPal (180+j, j * 255 / 63, j * 255 / 63, j * 255 / 63);

        if (first)
        {
            HVEZDY_Draw ();
            MENU_DrawItems (6, sselect, 17);
            DRAW_Najed ();
            first = false;
        }

        MENU_Control (6, sselect, 17);

        if (sselect == 1) { MENU_Arcade (); sselect = 1; }
        if (sselect == 2) { MENU_Chapter (); sselect = 2; }
        if (sselect == 3) { MENU_Options (); sselect = 3; }
        if (sselect == 4) { MENU_Credits (); sselect = 4; }
        if (sselect == 5) { MENU_HiScores (); sselect = 5; }
        if (sselect == 6) { DRAW_Zajed (); }
    }
}
