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
Popis: Definice promenych
*/

int         m3LoopQuitCode;
int         m3Mans;
int         m3HiPoc = 0;
int         m3MenuVal;
int         m3Opt[8] = { 3, 1, 1, 5, 0, 1, 0, 1 };
m3game_s    m3Game;
m3data_s    m3Data;
m3scr_s     m3Screen;
m3man_s     m3Man[40];
m3lev_s     m3Lev;
m3door_s    m3Door[4];
m3hs_s      m3Hs[10];
SDL_Surface *m3SdlMainSurface = NULL, *m3SdlBuffer = NULL;
m3mon_s     m3Mon[11] =
         //   D           NabInt MaxS  Sir Vys Faz FP maxJ  FD
         { { {0,0,0,0,0,0}, 40,  1,    26, 34, 21, 9, 78, { {1,4,5}, {1,2,1}, {4,19,2}, {1,18,1} } },
           { {1,1,0,1,0,0}, 0,   2,    16, 26, 11, 3, 25, { {1,0,2}, {1,2,1}, {2,6,3},  {1,9,1}  } },
           { {0,0,0,1,0,0}, 0,   1000, 16, 16, 5,  2, 0,  { {1,0,2}, {0,0,0}, {8,4,1},  {0,0,0}  } },
           { {1,0,0,1,2,1}, 150, 1000, 35, 45, 5,  2, 0,  { {1,0,2}, {0,0,0}, {8,4,1},  {0,0,0}  } },
           { {1,1,1,2,0,1}, 0,   3,    20, 35, 10, 3, 50, { {1,0,3}, {1,1,1}, {4,6,2},  {1,8,1}  } },
           { {1,1,1,2,2,1}, 30,  1,    20, 30, 13, 4, 32, { {1,0,3}, {1,3,1}, {2,8,3},  {1,11,1} } },
           { {0,0,0,2,0,0}, 0,   1000, 30, 30, 7,  3, 0,  { {1,0,3}, {0,0,0}, {8,6,1},  {0,0,0}  } },
           { {0,0,0,1,1,0}, 200, 1000, 20, 25, 8,  3, 0,  { {1,0,3}, {0,0,0}, {4,6,2},  {0,0,0}  } },
           { {1,1,0,2,2,1}, 20,  4,    20, 30, 11, 4, 60, { {1,0,3}, {1,3,1}, {8,8,1},  {1,9,1}  } },
           { {0,0,1,2,0,1}, 0,   1,    20, 10, 10, 3, 0,  { {1,0,3}, {0,0,0}, {4,6,2},  {1,8,1}  } },
           { {1,0,1,2,0,1}, 0,   5,    35, 50, 11, 3, 0,  { {1,0,3}, {0,0,0}, {3,6,3},  {1,9,1}  } } };
