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
#include "dzapp.h"

#include "version.h"
#include "mycustommaterial.h"
#include "mycustommodifier.h"
#include "mycustomnode.h"
#include "mycustomshape.h"

/*****************************
   Plugin Definition
*****************************/

/**
	This macro creates the plugin definition, and the functions that are needed
	for DAZ Studio to load our plugin.
**/
DZ_PLUGIN_DEFINITION( "Custom Element Loading Saving (SDK Example)" );

/**
	This macro sets the author string for the plugin
**/
DZ_PLUGIN_AUTHOR( "DAZ 3D, Inc" );

/**
	This macro sets the version number for the plugin
**/
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

/**
	This macro sets the description string for the plugin. This is a good place
	to provide specific information about the plugin, including an HTML link to
	any external documentation. Links are shown in the system default browser.
**/
DZ_PLUGIN_DESCRIPTION( "This plugin is a simple example provided with the DAZ Studio "
					  "SDK. It provides developers an example of the basics with "
					  "regard to loading and saving of custom nodes, modifiers, "
					  "materials, and shapes with a DSF scene." );

DZ_PLUGIN_CLASS_GUID( MyCustomMaterial,		98CF3008-AD44-4479-86FF-3A7E95469500 );
DZ_PLUGIN_CLASS_GUID( MyCustomMaterialIO,	98CF3008-AD44-4479-86FF-3A7E95469501 );
DZ_PLUGIN_REGISTER_MATERIAL_EXTRA_OBJECT_IO("my_material", MyCustomMaterialIO,  MyCustomMaterial);

DZ_PLUGIN_CLASS_GUID( MyCustomModifier,		98CF3008-AD44-4479-86FF-3A7E95469502 );
DZ_PLUGIN_CLASS_GUID( MyCustomModifierIO,	98CF3008-AD44-4479-86FF-3A7E95469503 );
DZ_PLUGIN_REGISTER_MODIFIER_EXTRA_OBJECT_IO("my_custom", MyCustomModifierIO,  MyCustomModifier);

DZ_PLUGIN_CLASS_GUID( MyCustomNode,     98CF3008-AD44-4479-86FF-3A7E95469504 );
DZ_PLUGIN_CLASS_GUID( MyCustomNodeIO,   98CF3008-AD44-4479-86FF-3A7E95469505 );
DZ_PLUGIN_REGISTER_NODE_EXTRA_OBJECT_IO("my_custom", MyCustomNodeIO,  MyCustomNode);

DZ_PLUGIN_CLASS_GUID( MyNurbShape,      98CF3008-AD44-4479-86FF-3A7E95469506 );
DZ_PLUGIN_CLASS_GUID( MyNurbGeometry,   98CF3008-AD44-4479-86FF-3A7E95469507 );
DZ_PLUGIN_CLASS_GUID( MyNurbGeometryIO, 98CF3008-AD44-4479-86FF-3A7E95469508 );
DZ_PLUGIN_REGISTER_GEOMETRY_EXTRA_OBJECT_IO("my_nurb", MyNurbGeometryIO,  MyNurbGeometry);

