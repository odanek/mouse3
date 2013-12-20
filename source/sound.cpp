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
Popis: Zvuky
*/

#include "mouse3.h"

struct m3sound_s
{
    int     FQ;
    int     DE;
};

static m3sound_s    zvk[50];
static int          zvku, zvkuS;

/*
==================================================
Inicializace
==================================================
*/
void SOUND_Init (void)
{
    zvku = 0;
    zvkuS = 0;
}

/*
==================================================
Prehrani zvuku
==================================================
*/
void SOUND_Play (void)
{
    if (!zvku || !m3Opt[1])
        return;
    // TODO: Dodelat
    // my_Sound (zvk[zvkuS].FQ, zvk[zvkuS].DE);
    zvku--;
    if (++zvkuS > 49)
        zvkuS -= 50;
}

/*
==================================================
Pridani zvuku
==================================================
*/
void SOUND_Add (int fq, int de)
{
    int cZ = zvkuS + zvku;

    if (zvku > 49)
        return;
    if (cZ > 49)
        cZ -= 50;
    zvk[cZ].FQ = fq;
    zvk[cZ].DE = de;
    zvku++;
}
