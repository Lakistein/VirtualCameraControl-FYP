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
#include "dzblackholemod.h"
#include "dzblackholeaction.h"
#include "dzblackholenode.h"

/*****************************
   Plugin Definition
*****************************/

DZ_PLUGIN_DEFINITION( "Black Hole WS Modifier (SDK Example)" );
DZ_PLUGIN_AUTHOR( "DAZ 3D, Inc" );
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

DZ_PLUGIN_DESCRIPTION(
	"This plugin is an example of creating a world-space modifier that can deform geometry in "
	"the scene. As the name implies, world-space modifiers act in the world-space of the scene. "
	"Unlike local modifiers, a world-space modifier can deform the geometry of any object in "
	"the scene. This example implements a world space modifier that has a speherical area of "
	"influence in the scene. Any geometry that is within the area of influence is stretched "
	"toward the center of the sphere, sort of like a - dare I say it? - Black Hole. While it is "
	"unlikely to find a place in your daily work flow, this example shows implementations of "
	"several important DAZ Studio SDK concepts."
	);

DZ_PLUGIN_CLASS_GUID( DzBlackHoleMod, F6726BB3-09DD-4198-A229-44831A151AFD );
DZ_PLUGIN_CLASS_GUID( DzBlackHoleAction, 00751BBA-A9E3-430f-9D8D-7E49AD6797A1 );
DZ_PLUGIN_CLASS_GUID( DzBlackHoleNode, 4F8BB012-18D5-487f-A324-9E7897FAFB7D );
DZ_PLUGIN_CLASS_GUID( DzBlackHoleNodeIO, 54287CB8-7F59-40CC-9B8D-1D1922316018 );
DZ_PLUGIN_REGISTER_NODE_EXTRA_OBJECT_IO("dz_black_hole", DzBlackHoleNodeIO,  DzBlackHoleNode);
