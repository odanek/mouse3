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
Popis: Psani fontem
*/

#include "mouse3.h"

static myBYTE       *m3FontTo = NULL;
static const myBYTE *m3Font = NULL;
static bool         m3FontMul = false;

/*
==================================================
Vykresleni jednoho znaku
==================================================
*/
static void FONT_DrawChar (int x, int y, int c, int col)
{
    const myBYTE    *frm;
    myWORD          i, j, b;

    frm = &m3Font[2 + c * m3Font[1]];

    for (i = 0; i < m3Font[1]; i++, frm++, y++)
    {
        b = *frm;

        for (j = 0; j < m3Font[0]; j++, x++)
            if (b & (1 << (7 - j)))
                if (x >= 0 && x < 320 && y >= 0 && y < 200)
                    m3FontTo[(y << 8) + (y << 6) + x] = (myBYTE) col;
        x -= m3Font[0];
    }
}

/*
==================================================
Vypsani retezce
==================================================
*/
void FONT_WriteS (int x, int y, int col, const char *str)
{
    int     i, l = (int) strlen (str);

    if (m3FontMul)
    {
        x *= m3Font[0];
        y *= m3Font[1];
    }

    for (i = 0; i < l; i++, x += m3Font[0])
        if (str[i] > 0 && str[i] != ' ')
            FONT_DrawChar (x, y, str[i], col);
}

/*
==================================================
Vypsani formatovaneho retezce
==================================================
*/
void FONT_WriteSF (int x, int y, int c, const char *str, ...)
{
    va_list argptr;
    char    f_str[200];

    va_start (argptr, str);
    vsprintf (f_str, str, argptr);
    va_end (argptr);

    FONT_WriteS (x, y, c, f_str);
}

/*
==================================================
Nastaveni psani fontem
==================================================
*/
void FONT_Set (myBYTE *to, bool mul)
{
    m3Font = CO_FontGet ();
    m3FontTo = to;
    m3FontMul = mul;
}
