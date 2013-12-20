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
Popis: Nahravani, ukladani dat
*/

#include "mouse3.h"

/*
==================================================
Nahrani ob5 obrazku
==================================================
*/
void LOAD_Ob5 (int poz, const char *ces, myBYTE *poi, int limit)
{
    myBYTE      del, kol;
    int         posin;
    int         posnow = 0;
    myFile_s    *ff;

    ff = MY_FOpen (ces, (poz - 1) * 4, "rb", true);
    MY_FRead (&posin, 4, 1, ff);
    MY_FSeek (ff, posin * 2 - 2, SEEK_SET);

    while (posnow < limit)
    {
        MY_FRead (&del,1,1,ff);
        MY_FRead (&kol,1,1,ff);
        memset (&poi[posnow], kol, del);
        posnow += del;
    }
    MY_FClose(&ff);
}

/*
==================================================
Nahrani priser
==================================================
*/
static void LOAD_LevelMonsters (int cis)
{
    short       ddd, ddd2, ddd3, ddd4, ddd5;
    int         i;
    char        levst[30];
    myFile_s    *ff;

    m3Mans = 1;
    strcpy (levst, "stage_/mouse3.lev");
    levst[5] = m3Game.stage + 48;
    ff = MY_FOpen (levst, 210 * cis, "rb", true);
    MY_FRead (&ddd, 2, 1, ff);
    m3Man[0].X = ddd * 20;
    MY_FRead (&ddd, 2, 1, ff);
    m3Man[0].Y = ddd * 20 - m3Mon[0].Vys;
    MY_FRead (&ddd5, 2, 1, ff);
    MY_FRead (&ddd4, 2, 1, ff);
    for (i = 0; i < ddd4; i++)
    {
            MY_FRead (&ddd, 2, 1, ff);
            MY_FRead (&ddd2, 2, 1, ff);
            MY_FRead (&ddd3, 2, 1, ff);
            MON_Add (ddd, ddd2, ddd3);
    }
    MY_FClose (&ff);

    LOAD_Ob5 (ddd5, MOUSE3_PIC, m3Data.bcg, 61440);
}

/*
==================================================
Nahrani urovne
==================================================
*/
void LOAD_Level (int stg, int num)
{
    int         i, j, b, pos = 1;
    char        stage[30];
    myFile_s    *ff;

    strcpy (stage, "stage_/level_.lev");
    stage[5] = stg + 48;
    stage[12] = num + 48;

    ff = MY_FOpen (stage, 0, "rb", true);
    m3Lev.Vys = 0;
    m3Lev.Sir = 0;
    MY_FRead(&m3Lev.Vys, 2, 1, ff);
    MY_FRead (&m3Lev.Sir, 2, 1, ff);
    MY_FClose (&ff);

    m3Data.lev = (myBYTE *) MY_Alloc (m3Lev.Sir * m3Lev.Vys * 2);
    MY_FLoadBlock (stage, 4, -1, m3Data.lev);
    LOAD_LevelMonsters (num - 1);

    // Odstraneni bonusu
    if (m3MenuVal == 3)
    {
        for (j = 0; j < m3Lev.Vys; j++)
            for (i = 0; i < m3Lev.Sir; i++, pos += 2)
            {
                b = m3Data.lev[pos];
                if (b >= POINTS1 && b <= POINTS50)
                    m3Data.lev[pos] = 0;
            }
    }
}
