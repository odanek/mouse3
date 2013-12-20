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
Popis: Pocitacem ovladane postavy
*/

#include "mouse3.h"

/*
==================================================
Nahrani prisery ze souboru
==================================================
*/
void MON_Load (int cis)
{
    int     siz = m3Mon[cis].Faz * (m3Mon[cis].Vys * m3Mon[cis].Sir), ii;
    int     kk = 0;

    m3Data.spi[cis] = (myBYTE *) MY_Alloc (siz);
    if (cis < 1)
        kk = 0;
    else
        for (ii = 0; ii < cis; ii++)
            kk += m3Mon[ii].Faz * (m3Mon[ii].Vys * m3Mon[ii].Sir);
    MY_FLoadBlock (MOUSE3_MON, kk, siz, m3Data.spi[cis]);
}

/*
==================================================
Zkontroluje dotyk s hracem
==================================================
*/
void MON_CheckMeet (void)
{
    int gj;

    if (m3Mans < 2)
        return;

    for (gj = 1; gj < m3Mans; gj++)
        if ((m3Man[gj].X - m3Man[0].X <= m3Mon[0].Sir) &&
            (m3Man[0].X - m3Man[gj].X <= m3Mon[m3Man[gj].Bt].Sir) &&
            (m3Man[gj].Y - m3Man[0].Y <= m3Mon[0].Vys) &&
            (m3Man[0].Y - m3Man[gj].Y <= m3Mon[m3Man[gj].Bt].Vys) && !M3_M_ISFLAG(gj,M3_F_DIED))
        {
            if (m3Mon[m3Man[gj].Bt].D[3] == 2 && (m3MenuVal != 3 || !m3Opt[4]))   // zajistuje nesmrtelnost ******
            {
                m3Man[0].MS = m3Mon[0].MaxS;
                m3Man[0].SS = 0;
                m3Man[0].Push = 0;
                MON_SetFaz(0);
                return;
            }

            if ((m3Mon[m3Man[gj].Bt].D[3] == 1) && (!m3Man[0].Push))
            {
                if (m3Man[0].S == 1)
                    m3Man[0].S = 2;
                SOUND_Add (600,10);
                if (m3Man[0].X < m3Man[gj].X)
                    m3Man[0].Push = -13;
                else
                    m3Man[0].Push = 13;
            }
        }
}

/*
==================================================
Pridani prisery do urovne
==================================================
*/
void MON_Add (int X, int Y, int C)
{
    m3Man[m3Mans].X = X * 20;
    m3Man[m3Mans].Y = Y * 20 - m3Mon[C].Vys;
    m3Man[m3Mans].Bt = C;
    m3Man[m3Mans].O = 1;
    m3Man[m3Mans].F = 0;
    m3Man[m3Mans].St = 1;
    m3Man[m3Mans].Ff = 0;
    m3Man[m3Mans].Flags = M3_F_NONE;
    m3Man[m3Mans].MS = 0;
    m3Man[m3Mans].S = 2;
    m3Man[m3Mans].Si = 0;
    m3Man[m3Mans].SS = 0;
    m3Man[m3Mans].Ad = 1;
    m3Mans++;
}

/*
==================================================
Nastaveni faze
==================================================
*/
void MON_SetFaz (int m)
{
    int tp = 0, dd = 6, mC = m3Man[m].Bt;

    if (m3MenuVal == 3 && m3Opt[4] && !m)       // Zajistuje nesmrtelnost
    {
        M3_M_CLRFLAG(0, M3_F_DIED);
        m3Man[0].MS = 0;
    }

    if (!m3Man[m].S)
        tp = 0;

    if ((m3Man[m].S == 1) || (m3Man[m].S == 2))
        tp = 1;

    if (m3Man[m].SS > 0)
        tp = 2;

    if (M3_M_ISFLAG(m,M3_F_DIED) || m3Man[m].MS >= m3Mon[m3Man[m].Bt].MaxS)
        tp = 3;

    if (!m3Mon[mC].FD[tp][0])
        return;

    if (tp == 3)
    {
        m3Man[m].F = m3Mon[mC].FD[3][1];
        M3_M_ADDFLAG(m,M3_F_DIED);
        if (m3Man[m].MS + m3Man[m].SS > m3Mon[mC].MaxS + 30)
            m3Man[m].F++;
        else
            m3Man[m].SS++;
        return;
    }

    if (m3Mon[mC].FD[tp][2] == 1)
    {
        m3Man[m].F = m3Mon[mC].FD[tp][1];
        return;
    }

    m3Man[m].Ff++;

    if (tp == 2)
        dd = 18;

    if (m3Man[m].F <= m3Mon[mC].FD[tp][1])
    {
        m3Man[m].F = m3Mon[mC].FD[tp][1];
        m3Man[m].St = 1;
    }
    if (m3Man[m].F > m3Mon[mC].FD[tp][1] + m3Mon[mC].FD[tp][2] - 2)
    {
        m3Man[m].F = m3Mon[mC].FD[tp][1] + m3Mon[mC].FD[tp][2] - 1;
        m3Man[m].St = -1;
    }
    if (m3Man[m].Ff > dd)
    {
        m3Man[m].F += m3Man[m].St;
        m3Man[m].Ff = 0;
    }
}

/*
==================================================
Pohne postavami
==================================================
*/
void MON_MoveAll (void)
{
    int nm = 0, oldX, mC, rozY, rozX, otocenK;

    if (m3Mans <= 1)
        return;

    while (++nm < m3Mans)
    {
        M3_M_CLRFLAG (nm, M3_F_NPRA);

        if (!m3Man[nm].S)
            KONTR_Kontrola (3,nm);
        if (m3Man[nm].S == 1)
            MOVE_Up (nm);
        if (m3Man[nm].S == 2)
            MOVE_Down (nm);

        oldX = m3Man[nm].X;
        mC = m3Man[nm].Bt;
        rozY = (m3Man[0].Y + m3Mon[0].Vys) - (m3Man[nm].Y + m3Mon[mC].Vys);
        rozX = m3Man[0].X - m3Man[nm].X;
        otocenK = ((rozX < 0) && (!m3Man[nm].O)) || ((rozX > 0) && (m3Man[nm].O));

        if (M3_M_ISFLAG(nm, M3_F_DIED))
        {
            MON_SetFaz (nm);
            continue;
        }

        if (m3Man[nm].SS)
        {
            m3Man[nm].SS++;
            if (m3Man[nm].SS > (m3Mon[mC].FD[2][0] * m3Mon[mC].FD[2][2]) * 18)
                m3Man[nm].SS = 0;
            MON_SetFaz (nm);
            if (m3Man[nm].SS)
                continue;
        }

        if (!m3Man[nm].S && M3_M_ISFLAG(nm, M3_F_NPRA) && !m3Mon[mC].D[1])
            m3Man[nm].O = 1 - m3Man[nm].O;

        if (m3Man[nm].O)
            MOVE_Side (nm, m3Man[nm].Ad);
        else
            MOVE_Side (nm, -m3Man[nm].Ad);

        if (!m3Man[nm].S && !m3Mon[mC].D[1] && M3_M_ISFLAG(nm, M3_F_NPRA))
        {
            m3Man[nm].SS = 1;
            continue;
        }

        if ((!m3Man[nm].S) && (m3Mon[mC].D[2]) && (!rozY))
            m3Man[nm].Ad = 3;
        else
            m3Man[nm].Ad = 1;

        if ((m3Man[nm].Ad == 1) && (rand () % 200 == 1) && (!m3Man[nm].S) && (m3Mon[mC].MaxJ))
        {
            m3Man[nm].S = 1;
            m3Man[nm].J = 0;
        }

        if (m3Mon[mC].NabInt)
        {
            if ((m3Mon[mC].D[4] == 1) && (!m3Man[nm].Si))
                SHOT_Shot (nm);
            if ((m3Mon[mC].D[4] == 2) && (!rozY) && (otocenK) && (!m3Man[nm].Si))
                SHOT_Shot (nm);
        }

        if ((rand () % 700 == 1) && (m3Mon[mC].D[0]) && (!((m3Mon[mC].D[5]) && (!rozY))))
            m3Man[nm].SS = 1;

        if ((m3Mon[mC].D[5]) && (!otocenK) && (!rozY))
            m3Man[nm].SS = 1;

        if (m3Man[nm].X == oldX)
        {
            m3Man[nm].O = 1 - m3Man[nm].O;
            m3Man[nm].SS = 0;
        }

        if (m3Man[nm].SS == 1)
            m3Man[nm].O = 1 - m3Man[nm].O;

        if ((m3Man[nm].SS > 0) && (m3Man[nm].S))
            m3Man[nm].SS = 0;
    }
}
