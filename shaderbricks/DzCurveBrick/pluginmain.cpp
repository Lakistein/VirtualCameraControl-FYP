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
#include "dzcurvebrick.h"

/*****************************
   Plugin Definition
*****************************/

DZ_PLUGIN_DEFINITION( "Curve Brick (SDK Example)" );
DZ_PLUGIN_AUTHOR( "DAZ 3D, Inc" );
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

DZ_PLUGIN_DESCRIPTION(
	"This plugin is an example of creating a shader brick for shader mixer.  It creates a shader designed to work on RiCurves."
);

DZ_PLUGIN_CLASS_GUID( DzCurveBrick, 66A3260B-7AB3-40DF-B807-82B7A35C0D3C );
