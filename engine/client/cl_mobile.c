/*
cl_mobile.c - common mobile interface
Copyright (C) 2015 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common.h"
#include "client.h"
#include "mobility_int.h"
#include "events.h"
#include "library.h"
#include "gl_local.h"
#include "touch.h"
#if defined(__ANDROID__) && !defined(DEDICATED)

//#include "platform/android/android-gameif.h"
#include "SDL_system.h"
#endif

// we don't have our controls at this time
mobile_engfuncs_t *gMobileEngfuncs;

void Android_Vibrate( float life, char flags );

static void pfnVibrate( float life, char flags )
{
	// here goes platform-specific backends
	MsgDev( D_NOTE, "Vibrate: %f %d", life, flags );
#if defined (__ANDROID__) && !defined(DEDICATED)
	Android_Vibrate(life, flags);
#endif
}

static void pfnEnableTextInput( int enable )
{
#if defined(XASH_SDL)
	SDLash_EnableTextInput(enable);
#endif
}

static void Touch_Disable( qboolean disable )
{

}

static mobile_engfuncs_t gpMobileEngfuncs =
{
	MOBILITY_API_VERSION,
	pfnVibrate,
	pfnEnableTextInput,
	IN_TouchAddClientButton,
	IN_TouchAddDefaultButton,
	IN_TouchHideButtons,
	IN_TouchRemoveButton,
	IN_TouchSetClientOnly,
	IN_TouchResetDefaultButtons
};

void Mobile_Init( void )
{
	// find a mobility interface
	pfnMobilityInterface ExportToClient = Com_GetProcAddress(clgame.hInstance, MOBILITY_CLIENT_EXPORT);

	gMobileEngfuncs = &gpMobileEngfuncs;

	if( ExportToClient )
	{
		ExportToClient( gMobileEngfuncs );
	}
	else
	{
		MsgDev( D_INFO, "Mobility interface not found\n");
	}
}

void Mobile_Destroy( void )
{

}
