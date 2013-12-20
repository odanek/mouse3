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
Projekt: Sablona aplikace
Popis: Fps - vypocet a synchronizace
*/

#include <time.h>
#include "co_core.h"

/*
==================================================
Synchronizuje dve procedury, tak za
move je volana prave APP_FPS_SPEED krat za
vterinu a draw je volana co nejcasteji
==================================================
*/
void CO_FpsSyncLoops (void (*move) (void), void (*draw) (void))
{
    static float    start_time = 0, last_time;
    static float    cur_time = 0;
    static int      moves_done = 0;
    int             i, diff;

    last_time = cur_time;
    cur_time = clock () / (float) CLOCKS_PER_SEC;

    // Restart synchronizace
    if (cur_time - last_time > 0.35f)
    {
        start_time = cur_time;
        moves_done = 0;
    }

    // Synchronizace
    diff = int(APP_FPS_SPEED * (cur_time - start_time)) - moves_done;

    for (i = 0; i < diff; i++, moves_done++)
        move ();

    if (diff > 0)
        draw ();
}

/*
==================================================
Vraci true kolikrat by se mely pohnout casovace a 
prekreslit se obrazovka
==================================================
*/
int CO_FpsSync (void)
{
    static float    start_time = 0, last_time;
    static float    cur_time = 0;
    static int      moves_done = 0;
    int             diff;

    last_time = cur_time;
    cur_time = clock () / (float) CLOCKS_PER_SEC;

    // Restart synchronizace
    if (cur_time - last_time > 0.35f)
    {
        start_time = cur_time;
        moves_done = 0;
    }

    // Synchronizace
    diff = int(APP_FPS_SPEED * (cur_time - start_time)) - moves_done;
    if (diff > 0)
        moves_done += diff;

    return diff;
}
