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
Popis: Hlavni hlavickovy soubor projektu
*/

#ifndef __MOUSE3_H
#define __MOUSE3_H

#include "core/co_core.h"

#define  M3_USE_OPENGL

#define  MOUSE3_ART  "data/mouse3.art"
#define  MOUSE3_ANM  "data/mouse3.anm"
#define  MOUSE3_FON  "data/mouse3.fon"
#define  MOUSE3_PIC  "data/mouse3.spo"
#define  MOUSE3_MON  "data/mouse3.mon"
#define  MOUSE3_PAL  "data/mouse3.pal"

#define  MOUSE3_DAT  "data/mouse3.dat"
#define  MOUSE3_SAV  "data/mouse3.sav"
#define  MOUSE3_INT  "data/intro.spo"

#define  MONSTERS    11

#define  STRELY      11
#define  DVERE       15
#define  KLICE       23
#define  BULLETS     147
#define  POINTS1     133
#define  POINTS2     135
#define  POINTS5     137
#define  POINTS10    139
#define  POINTS20    141
#define  POINTS50    143
#define  EXTRALIFE   145

#define  WAITTIME    13

// Uroven
struct m3lev_s
{
    int     Sir;
    int     Vys;
};

// Dvere
struct m3door_s
{
    int     X;
    int     Y;
    int     C;
    int     A;
};

// Hi-score
struct m3hs_s
{
    char    jmeno[14];
    int     bodu;
};

// Monstra
struct m3mon_s
{
    int     D[6];       // [0] - muze se otacet?   (0 - NE, 1 -ANO)
                        // [1] - muze do diry?
                        // [2] - muze zrychlit kdyz te vidi?
                        // [3] - typ monstra       (0 - Nic, 1 - zducha, 2 - zabiji)
                        // [4] - strelba           (0 - nikdy, 1 - pri nabiti, 2 - kdyz ses na musce)
                        // [5] - otaci se za tebou?
    int     NabInt;     // jak dlouho trva nabiti
    int     MaxS;       // maximalni vydrz (pocet zasahu)
    int     Sir;        // sirka kresby
    int     Vys;        // vyska kresby
    int     Faz;        // celkovy pocet fazi
    int     FP;         // misto ve kterem dochazi k otoceni orientace fazi
    int     MaxJ;       // Vyska skoku
    int     FD[4][3];   // [0] pohyb do stran, [1] skok, [2] otaceni, [3] pri zasahu
                        // [0] jsou faze obsazeny?, [1] zacatek pasu, [2] delka pasu
};

// Pozice obrazovky, pocitadlo animaci
struct m3scr_s
{
    int     X;
    int     Y;
    int     MAXX;
    int     MAXY;
    int     adXX;
    int     adYY;
    int     anmC;
    char    nowA;
#ifndef M3_USE_OPENGL
    size_t  zoom;
#else
    float   zoom;
#endif
};

// Vsechny postavy
struct m3man_s
{
    int     X;      // pozice (globalni)
    int     Y;
    int     O;      // orientace 0 - doleva, 1 - doprava
    int     F;      // faze pohybu
    int     J;      // faze skoku
    int     S;      // stav 0 - stoji na zemi, 1 - ve vyskoku, 2 - v padu
    int     SS;     // rozsireny stav
    int     MS;     // pocet obdrzenych zasahu strelou
    int     Si;     // stav nabiti
    int     St;     // zmena fazi 1 - nahoru, -1 - dolu
    int     Ff;     // pocitadlo pohybu
    int     Ad;     // posun o
    int     Bt;     // prislusnost ke kmenu mon
    int     Flags;  // flagy
    int     Push;   // postrceni
};

// Stav hry
struct m3game_s
{
    int     level;
    int     stage;
    int     life;
    int     points;
    int     exp;
    int     bullets;
    bool    key[4];
};

// Data
struct m3data_s
{
    myBYTE  *art;
    myBYTE  *anm;
    myBYTE  *lev;
    myBYTE  *bcg;
    myBYTE  *scr;
    myBYTE  *scr_gl;
    myBYTE  *spi[20];
    int     znak[256];
    myBYTE  pal[256][3];
};

// Quit kody
enum
{
    M3_QC_LOOP,
    M3_QC_NEXT,
    M3_QC_DIED,
    M3_QC_QUIT
};

// Flagy
#define M3_M_ISFLAG(x,y)    (m3Man[x].Flags & (y))
#define M3_M_SETFLAG(x,y)   m3Man[x].Flags = y
#define M3_M_ADDFLAG(x,y)   m3Man[x].Flags |= y
#define M3_M_CLRFLAG(x,y)   m3Man[x].Flags &= ~y

enum
{
    M3_F_NONE = 0,
    M3_F_DIED = 1,
    M3_F_NEXT = 2,
    M3_F_ONICE = 4,
    M3_F_NPRA = 8
};

extern int      m3LoopQuitCode;
extern int      m3Mans;
extern int      m3HiPoc;
extern int      m3MenuVal;
extern int      m3Opt[8];
extern m3game_s m3Game;
extern m3data_s m3Data;
extern m3scr_s  m3Screen;
extern m3man_s  m3Man[40];
extern m3lev_s  m3Lev;
extern m3door_s m3Door[4];
extern m3hs_s   m3Hs[10];
extern m3mon_s  m3Mon[11];

extern SDL_Surface *m3SdlMainSurface;

/*
===============================================================================
                                    MOUSE3.CPP
===============================================================================
*/
void    M3_WaitFor          (SDL_Keycode key);
int     M3_DoLoop           (void (*move)(void), void (*draw)(void));

/*
===============================================================================
                                    DRAW.CPP
===============================================================================
*/
void    DRAW_Logo           (void);
void    DRAW_Stats          (void);
void    DRAW_SetPal         (int i, int r, int g, int b);
void    DRAW_Obnov          (void);
void    DRAW_Najed          (void);
void    DRAW_Zajed          (void);
void    DRAW_Okno           (int x, int y, int zx, int zy, int sr, int vs, bool show);
void    DRAW_Man            (int naX, int naY, int faz, int sir, int vys, int svys, myBYTE *po);
void    DRAW_BlitScreen     (int start);
void    DRAW_Screen         (bool show);

/*
===============================================================================
                                    FONT.CPP
===============================================================================
*/
void    FONT_Set            (myBYTE *to, bool mul);
void    FONT_WriteS         (int x, int y, int col, const char *str);
void    FONT_WriteSF        (int x, int y, int c, const char *str, ...);

/*
===============================================================================
                                    HLAVNI.CPP
===============================================================================
*/
void    HLAVNI_PlayGame     (void);

/*
===============================================================================
                                    MENU.CPP
===============================================================================
*/
void    MENU_AddHiScore     (void);

/*
===============================================================================
                                    MOVE.CPP
===============================================================================
*/
void    MOVE_Zduchacka      (void);
void    MOVE_Ice            (int poho);
void    MOVE_Up             (int cm);
void    MOVE_Down           (int cm);
void    MOVE_Side           (int cm, int poho);
void    MOVE_ScrXY          (void);

/*
===============================================================================
                                    KONTROLA.CPP
===============================================================================
*/
int     KONTR_Attr          (int x, int y, int kontr, bool ch_p);
void    KONTR_Kontrola      (int cis, int koho);

/*
===============================================================================
                                    MONSTER.CPP
===============================================================================
*/
void    MON_Load            (int cis);
void    MON_CheckMeet       (void);
void    MON_Add             (int X, int Y, int C);
void    MON_MoveAll         (void);
void    MON_SetFaz          (int m);

/*
===============================================================================
                                    SHOTS.CPP
===============================================================================
*/
void    SHOT_Init           (void);
void    SHOT_Shot           (int kdo);
void    SHOT_MoveAll        (void);
void    SHOT_DrawAll        (void);

/*
===============================================================================
                                    LOAD.CPP
===============================================================================
*/
void    LOAD_Ob5            (int poz, const char *ces, myBYTE *poi, int limit);
void    LOAD_Level          (int stg, int num);

/*
===============================================================================
                                    SOUND.CPP
===============================================================================
*/
void    SOUND_Init          (void);
void    SOUND_Play          (void);
void    SOUND_Add           (int fq, int de);

/*
===============================================================================
                                    SOUND.CPP
===============================================================================
*/
void    MISC_PlayAni        (const char *file, int plFrom, int plTo, int wait);
void    MISC_SaveCfg        (void);
void    MISC_LoadCfg        (void);
void    MISC_SaveGame       (void);
void    MISC_LoadGame       (void);

/*
===============================================================================
                                    END.CPP
===============================================================================
*/
void    END_Congratulate    (void);

#endif
