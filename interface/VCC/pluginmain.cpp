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
#include "VCCpane.h"

/*****************************
   Plugin Definition
*****************************/

DZ_PLUGIN_DEFINITION( "Virtual Camera Control" );
DZ_PLUGIN_AUTHOR( "Lazar Lazarevic" );
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

DZ_PLUGIN_DESCRIPTION(
	"Virtual Camera Control is a plugin for automatic camera positioing for taking various types of shot including extreme close-up, face close-up, medium body shot, full body shot, bird's eye view, worm's  eye view and high angle."
	);

DZ_PLUGIN_CLASS_GUID( VCCPane, 74F1805A-D409-469B-8018-4FB6E33F6CD2);
DZ_PLUGIN_CLASS_GUID( VCCPaneAction, 295A67A3-0442-42d0-9F03-F675799D7573 );
