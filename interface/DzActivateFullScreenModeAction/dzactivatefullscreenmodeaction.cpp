/**********************************************************************
	Copyright (C) 2002-2012 DAZ 3D, Inc. All Rights Reserved.

	This file is part of the DAZ Studio SDK.

	This file may be used only in accordance with the DAZ Studio SDK 
	license provided with the DAZ Studio SDK.

	The contents of this file may not be disclosed to third parties, 
	copied or duplicated in any form, in whole or in part, without the 
	prior written permission of DAZ 3D, Inc, except as explicitly
	allowed in the DAZ Studio SDK license.

	See http://www.daz3d.com to contact DAZ 3D, Inc or for more 
	information about the DAZ Studio SDK.
**********************************************************************/

/*****************************
   Include files
*****************************/
#include "dzapp.h"
#include "dzmainwindow.h"

#include "dzactivatefullscreenmodeaction.h"

///////////////////////////////////////////////////////////////////////
// DzActivateFullScreenModeAction
///////////////////////////////////////////////////////////////////////

/**
**/
DzActivateFullScreenModeAction::DzActivateFullScreenModeAction() : DzAction( tr( "&Activate Full Screen Mode" ),
	tr("Cause the active viewport to go into full screen mode. Use the ESC key to exit full screen mode." ) )
{
}

/**
**/
void DzActivateFullScreenModeAction::executeAction()
{
		DzMainWindow *mw = dzApp->getInterface();
		if( !mw )
			return;

		mw->goFullScreen();
}
