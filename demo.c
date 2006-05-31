/* ***** BEGIN LICENSE BLOCK *****
 * Version: MIT/X11 License
 * 
 * Copyright (c) 2006 Diego Casorran
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 * Contributor(s):
 *   Diego Casorran <dcasorran@gmail.com> (Original Author)
 * 
 * ***** END LICENSE BLOCK ***** */


#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/muimaster.h>
#include "Teletipo_mcc.h"

struct Library *MUIMasterBase = NULL;

#ifndef MAKE_ID
# define MAKE_ID(a,b,c,d)	\
	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

int main(int argc,char *argv[])
{
	Object * app, * window;

	if (!(MUIMasterBase = OpenLibrary(MUIMASTER_NAME,MUIMASTER_VMIN)))
	{
		PutStr("Cannot open muimaster.library\n");
		return 10;
	}

	app = ApplicationObject,
		MUIA_Application_Title      , "Teletipo",
		MUIA_Application_Version    , "$VER: Teletipo 1.0 (28.05.2006)",
		MUIA_Application_Copyright  , "©2006, Diego Casorran",
		MUIA_Application_Author     , "Diego Casorran",
		MUIA_Application_Description, "Teletipo.mcc demostration.",
		MUIA_Application_Base       , "TELETIPO",

		SubWindow, window = WindowObject,
			MUIA_Window_Title, "Teletipo.mcc demostration.",
			MUIA_Window_ID   , MAKE_ID('T','I','P','O'),

			WindowContents, TeletipoObject,

				Child, Label("This is just a quick demostration of what Teletipo.mcc is able to do... not much, I know and you see it, but should be funny to show ie RSS feeds and similar news items!... or that was the intention why I created it...  :-]"),
			End,
		End,
	End;

	if( ! app ) {
		PutStr("Failed to create Application.");
		goto done;
	}


	DoMethod(window,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,
		app,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

	set(window,MUIA_Window_Open,TRUE);

	{
		ULONG sigs = 0;

		while (DoMethod(app,MUIM_Application_NewInput,&sigs) != MUIV_Application_ReturnID_Quit)
		{
			if (sigs)
			{
				sigs = Wait(sigs | SIGBREAKF_CTRL_C);
				if (sigs & SIGBREAKF_CTRL_C) break;
			}
		}
	}

	set(window,MUIA_Window_Open,FALSE);


done:
	if (app)
		MUI_DisposeObject(app);
	if (MUIMasterBase)
		CloseLibrary(MUIMasterBase);
	
	return(0);
}
