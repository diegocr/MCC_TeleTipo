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


/** $Id: Teletipo_mcc.c,v 0.1 2006/02/24 06:42:46 diegocr Exp $
 **/

int __dummy_startup( void ) {
	
	return(130130);
}

#include <proto/exec.h>
#include <proto/utility.h> /* NextTagItem */
#include <proto/intuition.h>
#include <SDI_mui.h>

struct ClassData
{
	struct MUI_EventHandlerNode ehn;
	LONG ticker;
};

#include "Teletipo_mcc.h"
#include "Teletipo_mcc_priv.h"

#define TELETIPO_INITIAL_DELAY 9

/***************************************************************************/
#if !defined(__PPC__)
ULONG STDARGS DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
	return(DoSuperMethod(cl, obj, OM_NEW, &tag1, NULL));
}
#endif

INLINE ULONG Teletipo_new(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct ClassData *data;
	
	obj = (Object *)DoSuperNew(cl,obj,
		MUIA_Group_Horiz, TRUE,
		MUIA_Virtgroup_Input, TRUE,
		TAG_MORE, msg->ops_AttrList);
	
	if(!obj) return(0);
	
	if(!(data = INST_DATA(cl,obj)))
		return 0;
	
	data->ehn.ehn_Events   = 0;
	data->ehn.ehn_Priority = 0;
	data->ehn.ehn_Flags    = 0;
	data->ehn.ehn_Object   = obj;
	data->ehn.ehn_Class    = cl;
	
	
	return((ULONG)obj);
}

/***************************************************************************/

INLINE ULONG Teletipo_dispose(struct IClass *cl,Object *obj,Msg msg)
{
	struct ClassData *data = INST_DATA(cl,obj);
	
	return(DoSuperMethodA(cl, obj, msg));
}

/***************************************************************************/

INLINE ULONG Teletipo_set(struct IClass *cl, Object *obj, Msg msg)
{
	register struct ClassData * data = INST_DATA(cl,obj);
	struct TagItem *tag, *tags = (((struct opSet *)msg)->ops_AttrList);
	
	while(( tag = (struct TagItem *) NextTagItem( &tags )))
	{
		switch (tag->ti_Tag)
		{
/*			case :
				
				data-> = (Object *) tag->ti_Data;
				
				return TRUE;
*/		}
	}
	
	return(DoSuperMethodA(cl, obj, msg));
}

/***************************************************************************/

INLINE ULONG Teletipo_get(struct IClass *cl, Object *obj, struct opGet * msg)
{
	ULONG *store = msg->opg_Storage;
	struct ClassData *data = INST_DATA( cl, obj);
	
	switch(msg->opg_AttrID)
	{
/*		case :
			
			*store = (ULONG) data-> ;
			
			return TRUE;
*/	}
	return(DoSuperMethodA(cl, obj, (Msg)msg));
}

/***************************************************************************/

INLINE ULONG Teletipo_setup(struct IClass *cl, Object *obj, Msg msg)
{
	register struct ClassData * data = INST_DATA(cl,obj);
	
	if(!DoSuperMethodA(cl, obj, msg))
		return FALSE;
	
	data->ticker = TELETIPO_INITIAL_DELAY;
	
	return TRUE;
}

/***************************************************************************/

INLINE ULONG Teletipo_cleanup(struct IClass *cl, Object *obj, Msg msg)
{
	register struct ClassData * data = INST_DATA(cl,obj);
	
	return(DoSuperMethodA(cl, obj, msg));
}

/***************************************************************************/

INLINE ULONG Teletipo_show(struct IClass *cl, Object *obj, struct MUIP_Show *msg)
{
	register struct ClassData * data = INST_DATA(cl,obj);
	ULONG retval;
	
	retval = DoSuperMethodA(cl, obj, (Msg)msg);
	
	data->ehn.ehn_Events |= IDCMP_INTUITICKS;    
	DoMethod(_win(obj), MUIM_Window_AddEventHandler, (ULONG)&data->ehn);
	
	return retval;
}

/***************************************************************************/

INLINE ULONG Teletipo_hide(struct IClass *cl, Object *obj, struct MUIP_Hide *msg)
{
	register struct ClassData * data = INST_DATA(cl,obj);
	
	DoMethod(_win(obj), MUIM_Window_RemEventHandler, (ULONG)&data->ehn);
	data->ehn.ehn_Events &= ~IDCMP_INTUITICKS;
	
	return DoSuperMethodA(cl, obj, (Msg)msg);
}

/***************************************************************************/

INLINE ULONG Teletipo_HandleEvent(struct IClass *cl, Object *obj, struct MUIP_HandleEvent *msg)
{
	if(msg->imsg)
	{
		if (msg->imsg->Class == IDCMP_INTUITICKS)
		{
			register struct ClassData * data = INST_DATA(cl,obj);
		
			if (data->ticker) data->ticker--;
			
			if (data->ticker == 0)
			{
				ULONG virty, virth;
				
				get(obj, MUIA_Virtgroup_Left, &virty);
				get(obj, MUIA_Virtgroup_Width, &virth);
		
				if (virth >= _mwidth(obj))
				{
					virty++;
					if (virty + _mwidth(obj) > virth) virty = 0;
					
					set(obj, MUIA_Virtgroup_Left, virty);
				}
			}
		}
	}
	
	return 0;
}

/***************************************************************************/

DISPATCHERPROTO(_Dispatcher)
{
	switch(msg->MethodID) {
		
		case OM_NEW:     return( Teletipo_new     (cl, obj, (APTR)msg));
		case OM_DISPOSE: return( Teletipo_dispose (cl, obj, (APTR)msg));
		case OM_SET:     return( Teletipo_set     (cl, obj, (APTR)msg));
		case OM_GET:     return( Teletipo_get     (cl, obj, (APTR)msg));
		case MUIM_Show:  return( Teletipo_show    (cl, obj, (APTR)msg));
		case MUIM_Hide:  return( Teletipo_hide    (cl, obj, (APTR)msg));
		
		case MUIM_Setup:
			return( Teletipo_setup            (cl, obj, (APTR)msg));
		
		case MUIM_Cleanup:
			return( Teletipo_cleanup          (cl, obj, (APTR)msg));
		
		case MUIM_HandleEvent:
			return( Teletipo_HandleEvent      (cl, obj, (APTR)msg));
		
	}
	return(DoSuperMethodA(cl,obj,msg));
}
