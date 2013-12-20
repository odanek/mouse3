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
Popis: Kontrola pohybu
*/

#include "mouse3.h"

/*
==================================================
Konec urovne - kdyz se dotkne bloku vychodu
==================================================
*/
static void KONTR_LevelEnd (int x, int y)
{
    int         pos;

    if (m3Man[0].X % 20 > 3)
        return;

    M3_M_SETFLAG (0, M3_F_NEXT);

    // Presunuti bloku z pozadi do popredi
    pos = 2 * (m3Lev.Sir * (y - 1) + x + 2);
    m3Data.lev[pos + 1] = m3Data.lev[pos];
    m3Data.lev[pos + 3] = m3Data.lev[pos + 2];
    pos += 2 * m3Lev.Sir;
    m3Data.lev[pos + 1] = m3Data.lev[pos];
    m3Data.lev[pos + 3] = m3Data.lev[pos + 2];

    // Odpochodovani do vychodu
    m3Man[0].Push = 53;
}

/*
==================================================
Otevreni dveri
==================================================
*/
static void KONTR_OpenDoor (int x, int y, int &b, int pos)
{
    int cK = b - (DVERE + (b - DVERE) / 2 * 2),
        cD = (b - DVERE) / 2;

    if (!m3Game.key[cD])
        return;
    if (cK)
    {
        pos -= 2 * m3Lev.Sir;
        y--;
    }

    m3Data.lev[pos] = 0;
    m3Data.lev[pos + 2 * m3Lev.Sir] = 0;

    m3Door[cD].A = 1;
    m3Door[cD].X = x * 20;
    m3Door[cD].Y = y * 20;
    m3Door[cD].C = cD;
    b = 0;
}

/*
==================================================

==================================================
*/
static void KONTR_CheckBlock (int x, int y, int &b, int pos, int kontr)
{
    int     *znak = m3Data.znak, i;

    // Zkontroluj bloky ktere zabiji zespodu
    if ((znak[b] == 3) && (kontr == 1))
    {
        m3Man[0].MS = m3Mon[0].MaxS;
        return;
    }

    // Zkontroluj bloky ktere zabiji
    if ((znak[b] == 4) && (kontr != 3))
    {
        m3Man[0].MS = m3Mon[0].MaxS;
        return;
    }

    // Zkontroluj zda nevesel do vychodu z urovne
    if (znak[b] == 6 && !m3Man[0].S)
    {
        KONTR_LevelEnd (x, y);
        return;
    }

    // Zkontroluj zda nestoji na lede
    if ((znak[b] == 21) && (kontr == 3))
    {
        M3_M_ADDFLAG(0, M3_F_ONICE);
        return;
    }

    // Zkontroluj zda se nedotkl dveri a pripadne je otevri
    if (znak[b] == 22)
    {
        KONTR_OpenDoor (x, y, b, pos);
        return;
    }

    // Pokud se nejedna o bonus nebo klic tak skonci
    if (znak[b] != 5)
        return;

    for (i = 1; i < 7; i++)
        SOUND_Add (400 + i * 100, 5);

    // Odstran bonus
    m3Data.lev[pos] = 0;

    if ((b - KLICE >= 0) && (b - KLICE < 8))
        m3Game.key[(b - KLICE) / 2] = true;

    if (b == BULLETS) m3Game.bullets += 3;
    if (b == POINTS1) m3Game.points += 100;
    if (b == POINTS2) m3Game.points += 200;
    if (b == POINTS5) m3Game.points += 500;
    if (b == POINTS10) m3Game.points += 1000;
    if (b == POINTS20) m3Game.points += 2000;
    if (b == POINTS50) m3Game.points += 5000;
    if (b == EXTRALIFE || m3Game.points >= m3Game.exp)
    {
        for (i = 1; i < 21; i++)
            SOUND_Add (600 + (i / 2) * 100, 5);

        if (m3Game.life < 5)
            m3Game.life++;
        if (m3Game.points >= m3Game.exp)
            m3Game.exp += 50000;
    }
}

/*
==================================================
Vraci atribut bloku na dane pozici
Pokud je ch_p = true provadi i pripadne sbirani bonusu
==================================================
*/
int KONTR_Attr (int x, int y, int kontr, bool ch_p)
{
    int     b, pos;

    if (x < 0 || x >= m3Lev.Sir || y >= m3Lev.Vys)
        return 1;

    if (y < 0)
        return 0;

    pos = (m3Lev.Sir * y + x) << 1;
    b = m3Data.lev[pos];

    if (!m3Data.znak[b])
        b = m3Data.lev[++pos];

    if (ch_p && !M3_M_ISFLAG(0, M3_F_NEXT))
        KONTR_CheckBlock (x, y, b, pos, kontr);

    if (m3Data.znak[b] > 19)
        return 1;
    else
        b = m3Data.znak[b];

    if ((b == 3) && ((kontr == 2) || (kontr == 3)))
        return 2;

    return b;
}

//***************************************************************************

static int  rX, rY, rS, rV, kt;

/*
==================================================
Kontrola pri vyskoku
==================================================
*/
static void KONTR_Kontr1 (void)
{
    int obX, obY, obC, olMS = m3Man[kt].MS;

    obY = rY / 20;
    for (obC = 0; obC <= (rS - 1) / 20; obC++)
    {
            obX = (rX + obC * 20) / 20;
            if (KONTR_Attr (obX, obY, 1, !kt) == 1)
            {
                m3Man[kt].S = 2;
                m3Man[kt].MS = olMS;
                m3Man[kt].Y += m3Man[kt].Ad;
                return;
            }
    }
    obX = (rX + rS - 1) / 20;
    if (KONTR_Attr (obX, obY, 1, !kt) == 1)
    {
            m3Man[kt].S = 2;
            m3Man[kt].MS = olMS;
            m3Man[kt].Y += m3Man[kt].Ad;
    }
}

/*
==================================================
Kontrola pri padu
==================================================
*/
static void KONTR_Kontr2 (void)
{
    int obX, obY, obC, obA, obP, olMS = m3Man[kt].MS;

    obY = (rY + rV - 1) / 20;
    obP = (rY + rV - m3Man[kt].Ad);
    obP = obP - (obP / 20) * 20;
    for (obC = 0; obC <= (rS - 1) / 20; obC++)
    {
            obX = (rX + obC * 20) / 20;
            obA = KONTR_Attr (obX, obY, 2, !kt);
            if ((obA == 1) || ((obA == 2) && (obP == 0)))
            {
                m3Man[kt].MS = olMS;
                m3Man[kt].S = 0;
                m3Man[kt].Y -= m3Man[kt].Ad;
                return;
            }
    }
    obX = (rX + rS - 1) / 20;
    obA = KONTR_Attr (obX, obY, 2, !kt);
    if ((obA == 1) || ((obA == 2) && (obP == 0)))
    {
            m3Man[kt].MS = olMS;
            m3Man[kt].S = 0;
            m3Man[kt].Y -= m3Man[kt].Ad;
    }
}

/*
==================================================
Kontrola zda stoji na pevnem
==================================================
*/
static void KONTR_Kontr3 (void)
{
    int     obX, obY, obC, obA, obP;
    bool    pra = true, npra = false;

    obY = (rY + rV) / 20;
    obP = (rY + rV);
    obP = obP - (obP / 20) * 20;

    for (obC = 0; obC <= (rS - 1) / 20; obC++)
    {
            obX = (rX + obC * 20) / 20;
            obA = KONTR_Attr (obX, obY, 3, !kt);
            if ((obA == 1) || ((obA == 2) && (obP == 0)))
                pra = false;
            else
                npra = true;
    }

    obX = (rX + rS - 1) / 20;
    obA = KONTR_Attr (obX, obY, 3, !kt);

    if ((obA == 1) || ((obA == 2) && (obP == 0)))
        pra = false;
    else
        npra = true;

    if (pra)
        m3Man[kt].S = 2;
    if (npra)
        M3_M_ADDFLAG (kt, M3_F_NPRA);
}

/*
==================================================
Kontrola pri pohybu do stran
==================================================
*/
static void KONTR_Kontr4 (void)
{
    int obX1, obX2, obY, obC;

    obX1 = rX / 20;
    obX2 = (rX + rS - 1) / 20;
    for (obC = 0; obC <= (rV - 1) / 20; obC++)
    {
            obY = (rY + obC * 20) / 20;
            if (rX < 0 || KONTR_Attr (obX1, obY, 4, !kt) == 1)
            {
                m3Man[kt].X += m3Man[kt].Ad;
                return;
            }
            if (KONTR_Attr (obX2, obY, 4, !kt) == 1)
            {
                m3Man[kt].X -= m3Man[kt].Ad;
                return;
            }
    }
    obY = (rY + rV - 1) / 20;
    if (KONTR_Attr (obX1, obY, 4, !kt) == 1)
    {
        m3Man[kt].X += m3Man[kt].Ad;
        return;
    }
    if (KONTR_Attr (obX2, obY, 4, !kt) == 1)
    {
        m3Man[kt].X -= m3Man[kt].Ad;
        return;
    }
}

/*
==================================================
Kontroly - vstupni bod
==================================================
*/
void KONTR_Kontrola (int cis, int koho)
{
    rX = m3Man[koho].X;
    rY = m3Man[koho].Y;
    rS = m3Mon[m3Man[koho].Bt].Sir;
    rV = m3Mon[m3Man[koho].Bt].Vys;
    kt = koho;

    if (cis == 1)
        KONTR_Kontr1 ();
    else if (cis == 2)
        KONTR_Kontr2 ();
    else if (cis == 3)
        KONTR_Kontr3 ();
    else if (cis == 4)
        KONTR_Kontr4 ();
}
