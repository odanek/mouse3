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
Popis: Strely
*/

#include "mouse3.h"

struct shotDef
{
    int X, Y, Sm, Od, St;
};

static shotDef  shot[200];
static int      shots;

/*
==================================================
Inicializace
==================================================
*/
void SHOT_Init (void)
{
    shots = 0;
}

/*
==================================================
Kontrola zda strela nekam nenarazila
==================================================
*/
static void SHOT_Kontr (int num)
{
    int obX1, obX2, obY;

    shot[num].St = 1;

    if ((shot[num].X < 0) || (shot[num].Y < 0))   //**** Y lze odstranit
        return;
    if (shot[num].X > m3Lev.Sir * 20)
        return;
    if (shot[num].Y > m3Lev.Vys * 20) //********* Y lze odstranit
        return;
    obX1 = shot[num].X / 20;
    obX2 = (shot[num].X + 15) / 20;
    obY = shot[num].Y / 20;

    if (KONTR_Attr (obX1, obY, 5, false) == 1)
        return;
    if (KONTR_Attr (obX2, obY, 5, false) == 1)
        return;

    obY = (shot[num].Y + 9) / 20;

    if (KONTR_Attr (obX1, obY, 5, false) == 1)
        return;
    if (KONTR_Attr (obX2, obY, 5, false) == 1)
        return;

    shot[num].St = 0;
}

/*
==================================================
Zkontroluje zasahy
==================================================
*/
static void SHOT_CheckFrags (void)
{
    int     ss, m, S, V;

    for (ss = 0; ss < shots; ss++)
        if (!shot[ss].St)
            for (m = 0; m < m3Mans; m++)
            {
                S = m3Mon[m3Man[m].Bt].Sir; V = m3Mon[m3Man[m].Bt].Vys;
                if ((shot[ss].X+16 > m3Man[m].X) && (shot[ss].X < m3Man[m].X + S) &&
                    (shot[ss].Y+10 > m3Man[m].Y) && (shot[ss].Y < m3Man[m].Y + V) &&
                    !M3_M_ISFLAG(m,M3_F_DIED | M3_F_NEXT))
                {
                    if (m || (m3MenuVal != 3 || !m3Opt[4]))
                    {
                        m3Man[m].MS++;
                        m3Man[m].SS = 0;
                        MON_SetFaz (m);
                    }

                    shot[ss].St = 1;
                    if ((abs (shot[ss].X - m3Screen.X) < 350) &&
                        (abs (shot[ss].Y - m3Screen.Y) < 250))
                    {
                        if (!shot[ss].Od)
                            SOUND_Add (300,10);
                        else
                            SOUND_Add (200,10);
                    }
                }
            }
}

/*
==================================================
Vystreli danou postavou
==================================================
*/
void SHOT_Shot (int kdo)
{
    if (m3Man[kdo].Si)
        return;
    if (shots >= 200)
        return;

    m3Man[kdo].Si = 1;

    if (!kdo)
    {
        if (!m3Game.bullets)
        {
            SOUND_Add (200,4);
            SOUND_Add (100,4);
            return;
        }
        else
            m3Game.bullets--;
    }

    shot[shots].X = (m3Man[kdo].X + m3Man[kdo].O * m3Mon[m3Man[kdo].Bt].Sir) +
                     m3Man[kdo].Ad + (m3Man[kdo].O - 1) * (16 + 2 * m3Man[kdo].Ad);
    shot[shots].Y = m3Man[kdo].Y + (m3Mon[m3Man[kdo].Bt].Vys / 2) - 5;
    shot[shots].Sm = m3Man[kdo].O;
    shot[shots].Od = !kdo ? 0 : 1;
    shot[shots].St = 0;
    shots++;
}

/*
==================================================
Odstrani strelu
==================================================
*/
static void SHOT_Remove (int ks)
{
    int     j;

    shots--;
    if (ks == shots)
        return;

    for (j = ks; j < shots; j++)
        memcpy (&shot[j], &shot[j + 1], sizeof (shotDef));
}

/*
==================================================
Vykresleni strel
==================================================
*/
void SHOT_DrawAll (void)
{
    int     i, rozX, rozY, mS;

    if (!shots)
        return;

    for (i = 0; i < shots; i++)
    {
        rozX = shot[i].X - m3Screen.X;
        rozY = shot[i].Y - m3Screen.Y;
        mS = STRELY + shot[i].Od;
        if (shot[i].St > 0)
            mS += 2;
        if ((rozX + 16 > 0) && (rozY + 10 > 0) && (rozX < 320) && (rozY < 200))
            DRAW_Man (rozX, rozY, mS, 20, 20, 20, m3Data.art);
    }
}

/*
==================================================
Pohne strelami
==================================================
*/
void SHOT_MoveAll (void)
{
    int s;

    if (!shots)
        return;

    SHOT_CheckFrags ();

    for (s = 0; s < shots; s++)
    {
        if (!shot[s].St)
        {
            if (!shot[s].Sm)
                shot[s].X -= 5;
            else
                shot[s].X += 5;

            SHOT_Kontr (s);

            if (shot[s].St)
                if ((abs (shot[s].X - m3Screen.X) < 350) &&
                    (abs (shot[s].Y - m3Screen.Y) < 250))
                {
                    if (!shot[s].Od)
                        SOUND_Add (300,10);
                    else
                        SOUND_Add (200,10);
                }
            }

            if (shot[s].St)
                if (++shot[s].St > 20)
                    SHOT_Remove (s);
    }
}
