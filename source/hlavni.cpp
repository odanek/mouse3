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
Popis: Hlavni herni smycka
*/

#include "mouse3.h"

/*
==================================================
Posune vsechny intervaly (animace, strely, ...)
==================================================
*/
static void HLAVNI_UpravIntervaly (void)
{
    int     i;

    m3Screen.anmC++;
    if (m3Screen.anmC > 20)
    {
        m3Screen.anmC = 0;
        m3Screen.nowA++;
        if (m3Screen.nowA > 11)
            m3Screen.nowA = 0;
    }

    for (i = 0; i < m3Mans; i++)
        if ((m3Man[i].Si) && (!m3Man[i].SS))
            if (++m3Man[i].Si > m3Mon[m3Man[i].Bt].NabInt)
                m3Man[i].Si = 0;

    for (i = 0; i < 4; i++)
        if ((m3Door[i].A > 0) && (m3Door[i].A < 40))
        {
            m3Door[i].A++;
            SOUND_Add (700 - m3Door[i].A * 10, 2);
        }
}

/*
==================================================
Reakce na stisknute klavesy, posun postavy
==================================================
*/
static void HLAVNI_MovePlayer (void)
{
    bool    has_moved = false;

    M3_M_CLRFLAG(0, M3_F_ONICE);

    // TODO: if (g_inp.key[SDLK_p]) M3_TGAScreenShot ();

    if (CO_IsKeyPressed(SDLK_SPACE))
        DRAW_Stats ();

    // ZKontroluje zda ma pevnou pudu pod nohama
    if (!m3Man[0].S)
        KONTR_Kontrola (3,0);

    // Posuny obrazovky pres Alt
    MOVE_ScrXY ();

    if (m3Man[0].Push)
    {
        MOVE_Zduchacka ();
        has_moved = true;
    }
    else
    {
        if (M3_M_ISFLAG(0, M3_F_ONICE))
            MOVE_Ice (m3Man[0].O * 2 - 1);

        if (!CO_IsKeyPressed(SDLK_LALT) && !CO_IsKeyPressed(SDLK_RALT))
        {
            if (!m3Man[0].Si || m3Man[0].S)
            {
                if ((m3Man[0].S == 1) && (m3Man[0].J > 17) && (CO_IsKeyPressed(SDLK_DOWN)))
                    m3Man[0].S = 2;

                if (!M3_M_ISFLAG(0, M3_F_ONICE))
                {
                    if (CO_IsKeyPressed(SDLK_RIGHT))
                    {
                        MOVE_Side (0, m3Man[0].Ad);
                        has_moved = true;
                    }
                    if (CO_IsKeyPressed(SDLK_LEFT))
                    {
                        MOVE_Side (0, -m3Man[0].Ad);
                        has_moved = true;
                    }
                }
                if ((CO_IsKeyPressed(SDLK_UP)) && (!m3Man[0].S))
                {
                    m3Man[0].S = 1;
                    m3Man[0].J = 0;
                }
                if (CO_IsKeyPressed(SDLK_LCTRL) || CO_IsKeyPressed(SDLK_RCTRL))
                    SHOT_Shot (0);
            }
        }
    }

    // Ve vyskoku
    if (m3Man[0].S == 1)
    {
        MOVE_Up (0);
        has_moved = true;
    }

    // V padu
    if (m3Man[0].S == 2)
    {
        MOVE_Down (0);
        has_moved = true;
    }

    // Pokud se nepohl tak faze na 0
    if (!has_moved)
        m3Man[0].F = 0;

    // Preskakovani urovni
    if ((CO_IsKeyPressed(SDLK_m)) && (CO_IsKeyPressed(SDLK_y)) && (CO_IsKeyPressed(SDLK_s)) && (m3MenuVal == 3))
        M3_M_SETFLAG(0, M3_F_NEXT);
}

/*
==================================================
Hlavni update procedura pri hre, aktualizace vseho
==================================================
*/
static void HLAVNI_Update (void)
{
    if (!M3_M_ISFLAG (0, M3_F_NEXT))
    {
        HLAVNI_MovePlayer ();
        MON_CheckMeet ();
    }
    else
    {
        // Odpochodovani na konci
        if (--m3Man[0].Push < 0)
            m3LoopQuitCode = M3_QC_NEXT;
        MOVE_Side (0, 1);
        // TODO: SOUND_Add (300 + ll * 20, 5);
    }

    SHOT_MoveAll ();
    MON_MoveAll ();
    HLAVNI_UpravIntervaly ();
    SOUND_Play ();

    if (M3_M_ISFLAG(0, M3_F_DIED))
        m3LoopQuitCode = M3_QC_DIED;
    if (CO_IsKeyPressed(SDLK_ESCAPE))
        m3LoopQuitCode = M3_QC_QUIT;
}

/*
==================================================
Hlavni vykreslovaci procedura
==================================================
*/
static void HLAVNI_Draw (void)
{
    DRAW_Screen (true);
}

/*
==================================================
Hrej zvolenou uroven
==================================================
*/
static int HLAVNI_PlayLevel ()
{
    int     i, qc;

    LOAD_Ob5 (m3Game.stage + 9, MOUSE3_PIC, m3Data.scr, 61440);
    FONT_Set (m3Data.scr, false);
    FONT_WriteSF (174, 139, 32, "%d", m3Game.level);
    DRAW_Najed ();
    M3_WaitFor (SDLK_RETURN);
    DRAW_Zajed ();

    LOAD_Level (m3Game.stage, m3Game.level);

    // Nastaveni promenych
    m3Screen.nowA = 0;
    m3Screen.anmC = 0;
    m3Screen.MAXX = m3Lev.Sir * 20 - 320;
    m3Screen.MAXY = m3Lev.Vys * 20 - 200;
    m3Screen.X = 0;
    m3Screen.Y = 0;
    m3Screen.adXX = 0;
    m3Screen.adYY = 0;

    // Inicilizace hrace
    m3Man[0].Ad = 2;
    m3Man[0].Bt = 0;
    m3Man[0].O = ((m3Man[0].X > m3Lev.Sir * 10) ? 0 : 1);
    m3Man[0].S = 0;
    m3Man[0].Flags = M3_F_NONE;
    m3Man[0].F = 0;
    m3Man[0].MS = 0;
    m3Man[0].Si = 0;
    m3Man[0].SS = 0;
    m3Man[0].Push = 0;

    for (i = 0; i < 4; i++)
    {
        m3Door[i].A = 0;
        m3Game.key[i] = 0;
    }

    SHOT_Init ();
    SOUND_Init ();

    KONTR_Kontrola (3, 0);
    if (m3Man[0].S)
        m3Man[0].F = m3Mon[0].FD[1][1];

    DRAW_Screen (false);
    DRAW_Najed ();

    qc = M3_DoLoop (HLAVNI_Update, HLAVNI_Draw);

    if (qc == M3_QC_DIED)
    {
        HLAVNI_Draw ();
        if (m3Game.life > 1)
            DRAW_Okno (121, 79, 241, 0, 79, 43, true);
        else
            DRAW_Okno (121, 80, 241, 46, 79, 42, true);
    }

    if (qc == M3_QC_NEXT)
        DRAW_Okno (116, 73, 215, 136, 89, 56, true);
    if (qc == M3_QC_NEXT || qc == M3_QC_DIED)
        M3_WaitFor (SDLK_RETURN);

    DRAW_Screen (true);
    DRAW_Zajed ();

    MY_Free (m3Data.lev);

    return qc;
}

/*
==================================================
Zepta se na ulozeni hry a pripadne ulozi
==================================================
*/
static void HLAVNI_AskForSave (void)
{
    if (m3MenuVal == 3)
        return;
    if (m3Opt[5])
    {
        MISC_SaveGame ();
        return;
    }

    FONT_Set (m3Data.scr, true);
    FONT_WriteS (10, 12, 32, "Chces hru ulozit? (A/N)");
    DRAW_BlitScreen (0);

    do
    {
        CO_ProcessEvents ();
        if (CO_IsKeyPressed(SDLK_a))
            MISC_SaveGame ();
    } while (!CO_IsKeyPressed(SDLK_n) && !CO_IsKeyPressed(SDLK_a));
}

/*
==================================================
Hrej hru
==================================================
*/
void HLAVNI_PlayGame (void)
{
    int     qc;

    while (m3Game.life > 0)
    {
        qc = HLAVNI_PlayLevel ();

        if (qc != M3_QC_DIED && qc != M3_QC_NEXT)
            return;

        if (qc == M3_QC_NEXT)
            if (++m3Game.level > 4)
            {
                if (m3MenuVal == 3)
                    return;
                if (++m3Game.stage > 4)
                {
                    END_Congratulate ();
                    return;
                }
                m3Game.level = 1;
            }

        if (qc == M3_QC_DIED)
            m3Game.life--;
        else if (m3MenuVal < 3)
            HLAVNI_AskForSave ();
    }
}
