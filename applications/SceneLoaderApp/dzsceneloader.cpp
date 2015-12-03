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

#include "dzscene.h"
#include "dznode.h"
#include "dzobject.h"
#include "dzshape.h"
#include "dzfacetmesh.h"

#include "dzsceneloader.h"

///////////////////////////////////////////////////////////////////////
// DzSceneLoader
///////////////////////////////////////////////////////////////////////

/**
	Default Constructor.
**/
DzSceneLoader::DzSceneLoader()
{
}

/**
	Loads the scene from the given file, and prints file statistics to the console.
	@param	filename	An absolute path to the scene file to load.
**/
bool DzSceneLoader::loadScene( const QString &filename )
{
	DzError		err;
	QByteArray	arr = filename.toLatin1();

	printf( "Loading Scene %s...\n", arr.data() );

	err = dzScene->loadScene( filename, DzScene::OpenNew );
	
	if( err != DZ_NO_ERROR ){
		arr = getErrorMessage( err ).toLatin1();
		printf( "Error Loading Scene: %s\n", arr.data() );
		return false;
	}

	printf( "Scene loaded successfully.\n" );

	printSceneStats();

	return true;
}

/**
	Prints the statistics of the current scene to the console.
**/
void DzSceneLoader::printSceneStats()
{
	int	nVerts, nFacets;

	getGeometryStats( nVerts, nFacets );

	printf( "Scene Statistics:\n" );
	printf( "\tTotal Number of Nodes in Scene: %d\n", dzScene->getNumNodes() );
	printf( "\tFigures in Scene: %d\n", dzScene->getNumSkeletons() );
	printf( "\tCameras in Scene: %d\n", dzScene->getNumCameras() );
	printf( "\tLights in Scene: %d\n", dzScene->getNumLights() );
	printf( "\tTotal Vertices in Scene: %d\n", nVerts );
	printf( "\tTotal Facets in Scene: %d\n", nFacets );
}

/**
	Collects geometry statistics from the nodes in the current scene.
	@param	nVerts		Set to the total number of vertices in all geometries in the scene.
	@param	nFacets		Set to the total number of facets in all geometries in the scene.
**/
void DzSceneLoader::getGeometryStats( int &nVerts, int &nFacets )
{
	DzNodeListIterator	nodeIt( dzScene->nodeListIterator() );
	DzNode				*node;

	nVerts = 0;
	nFacets = 0;

	while( nodeIt.hasNext() ){
		int nodeVerts, nodeFacets;

		node = nodeIt.next();
		getGeometryStats( node, nodeVerts, nodeFacets );

		nVerts += nodeVerts;
		nFacets += nodeFacets;
	}
}

/**
	Collects geometry statistics for the given node.
	@param	node		The node to get the geometry statistics for.
	@param	nVerts		Set to the number of vertices for any geometry on the node.
	@param	nFacets		Set to the number of facets for any geometry on the node.
**/
void DzSceneLoader::getGeometryStats( DzNode *node, int &nVerts, int &nFacets )
{
	nVerts = 0;
	nFacets = 0;

	assert( node );
	if( !node )
		return;

	DzObject	*obj = node->getObject();

	if( !obj )
		return;

	DzShape	*shape = obj->getCurrentShape();

	if( !shape )
		return;

	DzGeometry	*geom = shape->getGeometry();

	if( !geom )
		return;

	nVerts = geom->getNumVertices();

	if( DzFacetMesh *mesh = qobject_cast<DzFacetMesh*>( geom ) ){
		nFacets = mesh->getNumFacets();
	}
}
