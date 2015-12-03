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
#include "dzplugin.h"

#include "version.h"
#include "dzinfopane.h"

/*****************************
   Plugin Definition
*****************************/

DZ_PLUGIN_DEFINITION( "Scene Info Pane (SDK Example)" );
DZ_PLUGIN_AUTHOR( "DAZ 3D, Inc" );
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

DZ_PLUGIN_DESCRIPTION(
	"This plugin is a very simple example of extending the DAZ Studio interface by inheriting from the "
	"DzPane class. Plugins that need a persistent interface where the user is still able to interact "
	"with the rest of the DAZ Studio interface should implement their interface as a DzPane subclass."
	"<br><br>"
	"In this example, we provide a simple HTML-based display of general information about the scene "
	"contents and the current selection. This pane also updates itself whenever the scene or primary "
	"selection is changed so that the display reflects the contents of the scene in \'real time\'."
	);

DZ_PLUGIN_CLASS_GUID( DzSceneInfoPane, B833CC13-F88D-4fd2-8C95-F82D0BF3A5D0 );
DZ_PLUGIN_CLASS_GUID( DzSceneInfoPaneAction, 295A67A3-0442-42d0-9F03-F675799D7573 );
