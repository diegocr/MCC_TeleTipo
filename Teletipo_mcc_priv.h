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


#include <exec/types.h>
#include <exec/resident.h>
#include <exec/execbase.h>
#include <dos/dosextens.h>
#include <libraries/mui.h>
#include <exec/libraries.h>
#include <proto/exec.h>
#include <proto/muimaster.h>
#include "mcc_common.h"
#include <SDI_mui.h>

#define OMITVER
#include <dcrver.h>

#define VERSION		1
#define REVISION	0
#define VSTRING		"1.0"
#define LIB_DATE	"28.05.2006"
#define LIB_COPYRIGHT	"(C)2006 Diego Casorran"
#define CLASS		"Teletipo.mcc"
//#define SUPERCLASS	MUIC_Area
#define SUPERCLASS	MUIC_Virtgroup

#define INSTDATA	ClassData

#define UserLibID	"$VER: " CLASS " " VSTRING " " OnCPU " (" LIB_DATE ") " LIB_COPYRIGHT
#define MASTERVERSION	19

//#define USEDCLASSESP  used_classesP
//static const STRPTR used_classesP[] = { "Toolbar.mcp", NULL };
//#define USE_UTILITYBASE

#if 0
#define PreClassInit
#define PostClassExit

struct Library *DataTypesBase = NULL;
struct Library *KeymapBase    = NULL;
struct Library *DiskfontBase  = NULL;
#if defined(__amigaos4__)
struct DataTypesIFace *IDataTypes = NULL;
struct KeymapIFace *IKeymap       = NULL;
struct DiskfontIFace *IDiskfont   = NULL;
#endif

#ifdef USE_IMAGEPOOL
struct Library *ImagePoolBase = NULL;
#if defined(__amigaos4__)
struct ImagePoolIFace *IImagePool = NULL;
#endif
#endif

BOOL PreClassInitFunc(void)
{
  if((DataTypesBase = OpenLibrary("datatypes.library", 37L)) &&
     GETINTERFACE(IDataTypes, DataTypesBase))
  {
    if((KeymapBase = OpenLibrary("keymap.library", 37L)) &&
       GETINTERFACE(IKeymap, KeymapBase))
    {
      if((DiskfontBase = OpenLibrary("diskfont.library", 37L)) &&
         GETINTERFACE(IDiskfont, DiskfontBase))
      {
        #ifdef USE_IMAGEPOOL
        // ImagePool is no MUST
        if((ImagePoolBase = OpenLibrary("imagepool.library", 37L)) &&
           GETINTERFACE(IImagePool, ImagePoolBase)) { }
        #endif

        return TRUE;
      }

      DROPINTERFACE(IKeymap);
      CloseLibrary(KeymapBase);
      KeymapBase = NULL;
    }

    DROPINTERFACE(IDataTypes);
    CloseLibrary(DataTypesBase);
    DataTypesBase = NULL;
  }

  return FALSE;
}

VOID PostClassExitFunc(void)
{
  #ifdef USE_IMAGEPOOL
  if(ImagePoolBase) { DROPINTERFACE(IImagePool); CloseLibrary(ImagePoolBase); ImagePoolBase = NULL; }
  #endif

  if(DiskfontBase)  { DROPINTERFACE(IDiskfont);  CloseLibrary(DiskfontBase);  DiskfontBase  = NULL; }
  if(KeymapBase)    { DROPINTERFACE(IKeymap);    CloseLibrary(KeymapBase);    KeymapBase    = NULL; }
  if(DataTypesBase) { DROPINTERFACE(IDataTypes); CloseLibrary(DataTypesBase); DataTypesBase = NULL; }
}
#endif // if 0

/******************************************************************************/
/*                                                                            */
/* include the lib startup code for the mcc/mcp  (and muimaster inlines)      */
/*                                                                            */
/******************************************************************************/

#if 0
# include "Debug.h"
#elif defined(DEBUG)
# define D(f, s, vargs...)	kprintf( s, ## vargs )
#else
# define D(f, s, vargs...)	((void)0)
#endif

#define SetupDebug()	((void)0)

#include "mccheader.c"
