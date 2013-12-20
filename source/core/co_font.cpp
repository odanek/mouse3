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
Popis: Prace s fonty, psani na obrazovku
*/

#include "co_core.h"

static  myBYTE  *fontData = NULL;
static  int     fontSZX, fontSZY;

/*
==================================================
Nahrani fontu ze souboru
==================================================
*/
void CO_FontLoad (const char *fontFile)
{
    MY_Free (fontData);

    if (MY_FSize (fontFile) < 50)
        MY_Err (MY_ErrDump(MY_L("COSTR0001|Nepodarilo se nahrat soubor s fontem %s"), fontFile));

    fontData = (myBYTE *) MY_Alloc (MY_FSize (fontFile) - 50);
    MY_FLoadBlock (fontFile, 50, -1, (void *) fontData);
    fontSZX = (int) fontData[0];
    fontSZY = (int) fontData[1];
}

/*
==================================================
Uvolneni fontu z pameti
==================================================
*/
void CO_FontFree (void)
{
    MY_Free (fontData);
}

/*
==================================================
Vraci ukazatel na nahrany font
==================================================
*/
const myBYTE *CO_FontGet (void)
{
    return fontData;
}
