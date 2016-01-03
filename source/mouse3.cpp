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
Popis: Hlavni soubor
*/

#include "mouse3.h"
#include "declare.h"

/*
==================================================
Reakce na stisk klavesy
==================================================
*/
void P_KeyEvent (int key)
{

}

/*
==================================================
Reakce na aktivaci/deaktivaci okna
==================================================
*/
void P_ActiveEvent (bool active)
{
    if (active)
        DRAW_Logo ();
}

/*
==================================================
Cekani na stisk dane klavesy
==================================================
*/
void M3_WaitFor (SDL_Keycode key)
{
    do
    {
        CO_ProcessEvents ();
    } while (!CO_IsKeyPressed(key));
}

/*
==================================================
Smycka - vykresleni, zpracovani eventu
==================================================
*/
int M3_DoLoop (void (*move)(void), void (*draw)(void))
{
    m3LoopQuitCode = 0;

    while (!(g_app.flags & APP_FLAG_QUIT) && !m3LoopQuitCode)
    {
        CO_ProcessEvents ();
        CO_FpsSyncLoops (move, draw);
    }

    return m3LoopQuitCode;
}
