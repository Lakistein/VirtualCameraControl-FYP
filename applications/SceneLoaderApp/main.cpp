/**********************************************************************
	Copyright (C) 2002-2012 DAZ 3D, Inc. All Rights Reserved.

	This file is part of the DAZ Studio SDK.

	This file may be used only in accordance with the DAZ Studio SDK
	license provided with the DAZ Studio SDK.

	The contents of this file may not be disclosed to third parties,
	copied or duplicated in any form, in whole or in part, without the
	prior written permission of DAZ 3D, Inc, except as explicitly
	allowed in the DAZ Studio SDK license.

	See http://www.daz3d.com to contact DAZ 3D or for more
	information about the DAZ Studio SDK.
**********************************************************************/

/*****************************
   Include files
*****************************/
#include <stdlib.h>

#include "dzapp.h"

#include "dzsceneloader.h"

void printUsage();

/**
   Program entry point. Creates and initializes DzApp with no interface and 
   loads the specified scene.
**/
int main( int argc, char **argv )
{
	if( argc < 2 || argc > 2 ){
		printf( "No filename specified.\n" );
		printUsage();
		return 1;
	}

	int myArgc = 1;
	QString			filename( argv[1] );
	DzApp			app( myArgc, argv, false, true );
	DzSceneLoader	loader;

	if( !app.init( DzApp::NoInterface ) )
		return 1;

	if( !loader.loadScene( filename ) )
		return 2;

	return 0;
}

/**
	Prints the usage message to the console.
**/
void printUsage()
{
	printf( "Usage:\n" );
	printf( "   SceneLoaderApp filename.daz\n" );
}
