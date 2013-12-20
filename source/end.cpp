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
Popis: Zaver hry, blahoprani
*/

#include "mouse3.h"

/*
==================================================
Vypise barevny text blahopreji
==================================================
*/
static void END_DrawFancyText (int psn)
{
    char    text[15] = "BLAHOPREJI";
    int     l;

    FONT_Set (m3Data.scr, false);
    FONT_WriteS (126, 65, 0, text);
    for (l = 0; l < 10; l++)
        FONT_WriteSF ((16 + l) * 8 - 3, 64, 32 + psn + l, "%c", text[l]);
}

/*
==================================================
Zaverecna dekovacka
==================================================
*/
void END_Congratulate (void)
{
    int     mC, bf_wait = 0, bf_psn = 0, t;

    m3Opt[6] = 1;

    memset (m3Data.scr, 26, 61440);
    DRAW_Najed ();

    M3_M_SETFLAG(0, M3_F_NONE);
    m3Man[0].MS = 0;
    m3Man[0].S = 0;

    mC = 0;
    m3Man[0].X = 320;
    m3Man[0].Y = 100 - m3Mon[mC].Vys / 2;
    MON_SetFaz (0);

    while (mC < MONSTERS)
    {
        if ((t = CO_FpsSync ()) > 0)
        {
            // Move
            while (t-- > 0)
            {
                if (m3Man[0].X + m3Mon[mC].Sir <= 0)
                {
                    mC++;
                    m3Man[0].X = 320;
                    m3Man[0].Y = 100 - m3Mon[mC].Vys / 2;
                    m3Man[0].Bt = mC;
                }
                else
                {
                    if (!bf_wait)
                        m3Man[0].X--;
                    else
                    {
                        bf_wait--;
                        if (bf_wait % 2)
                            if (++bf_psn > 48)
                                bf_psn = 0;
                    }

                    if (m3Man[0].X == 160 - m3Mon[mC].Sir / 2 && !m3Man[0].SS)
                    {
                        m3Man[0].SS = 1;
                        bf_wait = 144;
                    }

                    if (bf_wait <= 0)
                        m3Man[0].SS = 0;

                    MON_SetFaz (0);
                }
            }

            // Draw
            memset (m3Data.scr, 26, 61440);
            if (m3Man[0].X + m3Mon[mC].Sir > 0)
                DRAW_Man (m3Man[0].X, m3Man[0].Y, m3Man[0].F, m3Mon[mC].Sir, m3Mon[mC].Vys, m3Mon[mC].Vys, m3Data.spi[mC]);
            if (m3Man[0].SS)
                END_DrawFancyText (bf_psn);
            DRAW_BlitScreen (0);
        }
    }

    m3Man[0].Bt = 0;
    FONT_Set (m3Data.scr, true);
    FONT_WriteSF (18, 19, 32, "ENTER");
    DRAW_BlitScreen (0);
    FONT_Set (m3Data.scr, false);
    M3_WaitFor (SDLK_RETURN);
    DRAW_Zajed ();

    MENU_AddHiScore ();
}
