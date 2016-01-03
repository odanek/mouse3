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
Popis: Pohyb postav a obrazovky
*/

#include "mouse3.h"

/*
==================================================
Pokud byla postava postrcena
==================================================
*/
void MOVE_Zduchacka (void)
{
    if (m3Man[0].Push < 0)
    {
        MOVE_Side (0, -2);
        m3Man[0].Push++;
    }
    if (m3Man[0].Push > 0)
    {
        MOVE_Side (0, 2);
        m3Man[0].Push--;
    }
}

/*
==================================================
Pohyb na lede
==================================================
*/
void MOVE_Ice (int poho)
{
    m3Man[0].Ad = 1;
    m3Man[0].X += poho;
    KONTR_Kontrola (4, 0);
    m3Man[0].Ad = 2;
}

/*
==================================================
Pohyb nahoru
==================================================
*/
void MOVE_Up (int cm)
{
    m3Man[cm].Y -= m3Man[cm].Ad;
    m3Man[cm].J += m3Man[cm].Ad;
    if (m3Man[cm].J >= m3Mon[m3Man[cm].Bt].MaxJ)
        m3Man[cm].S = 2;
    MON_SetFaz (cm);
    KONTR_Kontrola (1, cm);
}

/*
==================================================
Pohyb dolu
==================================================
*/
void MOVE_Down (int cm)
{
    m3Man[cm].Y += m3Man[cm].Ad;
    KONTR_Kontrola (2, cm);
    MON_SetFaz (cm);
}

/*
==================================================
Pohyb dostran
==================================================
*/
void MOVE_Side (int cm, int poho)
{
    MON_SetFaz (cm);
    m3Man[cm].X += poho;
    if (poho > 0)
        m3Man[cm].O = 1;
    else
        m3Man[cm].O = 0;
    KONTR_Kontrola (4, cm);
}

/*
==================================================
Pohyb obrazovkou pres Alt
==================================================
*/
void MOVE_ScrXY (void)
{
    if (CO_IsKeyPressed(SDLK_LALT) || CO_IsKeyPressed(SDLK_RALT))
    {
        if ((CO_IsKeyPressed(SDLK_RIGHT)) && (m3Screen.adXX < 110))
            m3Screen.adXX += 3;
        if ((CO_IsKeyPressed(SDLK_LEFT)) && (m3Screen.adXX > -110))
            m3Screen.adXX -= 3;
        if ((CO_IsKeyPressed(SDLK_UP)) && (m3Screen.adYY > -60))
            m3Screen.adYY -= 3;
        if ((CO_IsKeyPressed(SDLK_DOWN)) && (m3Screen.adYY < 60))
            m3Screen.adYY += 3;
    }
    else
        if ((m3Screen.adXX) || (m3Screen.adYY))
        {
            if (m3Screen.adXX > 0)
                m3Screen.adXX -= 3;
            if (m3Screen.adYY > 0)
                m3Screen.adYY -= 3;
            if (m3Screen.adXX < 0)
                m3Screen.adXX += 3;
            if (m3Screen.adYY < 0)
                m3Screen.adYY += 3;
        }
}
