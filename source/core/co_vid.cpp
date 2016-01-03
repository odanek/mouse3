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
Popis: Obsluha grafickeho rezimu
*/

#include "co_core.h"

/*
==================================================
Swap buffers - prohodi buffery
==================================================
*/
void VID_SwapBuffers (void)
{
    if (!g_vid.inited)
        return;

	SDL_GL_SwapWindow(g_vid.window);
}

/*
==================================================
Inicializace videa
==================================================
*/
void VID_Init (void)
{
    g_vid.inited = false;
}

/*
==================================================
Nastaveni konkretniho grafickeho modu
==================================================
*/
void VID_SetMode (int w, int h, int bpp, int aa, bool full)
{
	int flags = 0;

	g_vid.cl_width = w;
	g_vid.cl_height = h;
	g_vid.gl_aspect = (float) g_vid.cl_width / (float) g_vid.cl_height;

	flags = SDL_WINDOW_OPENGL;
	if (full)
		flags |= SDL_WINDOW_FULLSCREEN;

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, bpp);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	if (aa > 0 && aa < 8 && aa % 2 == 0)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, aa);
	}

	// Nastav novy graficky rezim
	g_vid.window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_vid.cl_width, g_vid.cl_height, SDL_WINDOW_INPUT_GRABBED | flags);
	if (g_vid.window == NULL)
		MY_Err (MY_ErrDump ("%s: %s\n", MY_L("COSTR0009|Nelze nastavit graficky rezim"), SDL_GetError()));

	SDL_SetWindowTitle (g_vid.window, APP_NAME);
	SDL_SetWindowIcon (g_vid.window, SDL_LoadBMP (APP_FILE_ICON));

	g_vid.glContext = SDL_GL_CreateContext(g_vid.window);
	if (g_vid.glContext == NULL)
		MY_Err (MY_ErrDump ("%s: %s\n", MY_L("COSTR0009|Nelze ziskat OpenGL context"), SDL_GetError()));

	SDL_GL_SetSwapInterval(1);
	SDL_ShowCursor (SDL_DISABLE);

	g_vid.inited = true;
}

/*
==================================================
Ukonceni grafickeho rezimu
==================================================
*/
void VID_Shutdown (void)
{
    g_vid.inited = false;
}
