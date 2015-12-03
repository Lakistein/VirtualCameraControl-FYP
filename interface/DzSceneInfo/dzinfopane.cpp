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
#include "dzinfopane.h"

#include <QtCore/QObject>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

#include "dzapp.h"
#include "dzbone.h"
#include "dzfacetmesh.h"
#include "dzhelpmgr.h"
#include "dzobject.h"
#include "dzscene.h"
#include "dzshape.h"
#include "dzskeleton.h"
#include "dzstyle.h"

static const int c_minWidth = 200;
static const int c_minHeight = 150;

/**
**/
DzSceneInfoPane::DzSceneInfoPane() : DzPane( "Scene Info" ), m_refreshBlocked( false ), 
	m_totalVerts( 0 ), m_totalTris( 0 ), m_totalQuads( 0 )
{
	// Declarations
	int margin = style()->pixelMetric( DZ_PM_GeneralMargin );

	// Define the layout for the pane
	QVBoxLayout *mainLyt = new QVBoxLayout();
	mainLyt->setMargin( margin );
	mainLyt->setSpacing( margin );

	// Use a text browser for the output window - this supports basic html/rtf formatting
	m_output = new QTextBrowser();
	m_output->setObjectName( "SceneInfoTxtBrwsr" );
	m_output->setMinimumSize( c_minWidth, c_minHeight );
	mainLyt->addWidget( m_output );

	// Set the layout for the pane
	setLayout( mainLyt );

	// Do an initial update of the scene stats
	refresh();

	// Listen for the node list in the scene to change so that we can update our information
	connect( dzScene, SIGNAL(nodeListChanged()), this, SLOT(refresh()) );

	// Listen for the primary selection to change so that we can update our information
	connect( dzScene, SIGNAL(primarySelectionChanged(DzNode*)), this, SLOT(refreshSelected(DzNode*)) );

	// Listen for file load operations so that we can block our refresh function from executing.
	// During a scene load, the node list will change many times - we only want to refresh
	// once at the end of the load, so that we do not slow down the file load.
	connect( dzScene, SIGNAL(sceneLoadStarting()), this, SLOT(blockRefresh()) );
	connect( dzScene, SIGNAL(sceneClearStarting()), this, SLOT(blockRefresh()) );
	connect( dzScene, SIGNAL(sceneLoaded()), this, SLOT(unblockRefresh()) );
	connect( dzScene, SIGNAL(sceneCleared()), this, SLOT(unblockRefresh()) );

	setMinimumSize( c_minWidth, c_minHeight );
}

/**
**/
DzSceneInfoPane::~DzSceneInfoPane()
{
}

/**
**/
void DzSceneInfoPane::refresh()
{
	if( m_refreshBlocked )
		// Ignore signals during scene load
		return;

	int					numVerts, numTris, numQuads;
	DzNodeListIterator	nodeIt( dzScene->nodeListIterator() );

	// Clear out any previous data
	m_output->clear();
	m_totalVerts = 0;
	m_totalTris = 0;
	m_totalQuads = 0;

	// Iterate over all the nodes in the scene and collect data for them
	while( nodeIt.hasNext() ){
		getNodeInfo( nodeIt.next(), numVerts, numTris, numQuads );
		m_totalVerts += numVerts;
		m_totalTris += numTris;
		m_totalQuads += numQuads;
	}

	// Collect data for the primary selection
	refreshSelected( dzScene->getPrimarySelection() );
}

/**
**/
void DzSceneInfoPane::refreshSelected( DzNode *node )
{
	if( m_refreshBlocked )
		// Ignore signals during scene load
		return;

	int		tNumVerts = 0, tNumTris = 0, tNumQuads = 0;
	QString	html;

	// Write the html output for the scene-wide stats
	writeSceneStats();

	if( node ){
		// Get the stats for the primary selection
		getNodeInfo( node, tNumVerts, tNumTris, tNumQuads );

		if( node->inherits( "DzSkeleton" ) ){
			// If the primary selection is a figure, add the stats for all the bones also
			DzBoneList	bones;
			int			i, n, numVerts, numTris, numQuads;

			((DzSkeleton*)node)->getAllBones( bones );
			for( i = 0, n = bones.count(); i < n; i++ ){
				getNodeInfo( bones[i], numVerts, numTris, numQuads );
				tNumVerts += numVerts;
				tNumTris += numTris;
				tNumQuads += numQuads;
			}
		}

		// Write the html output for the primary selection stats
		html = "<b>Primary Selection : </b><br/><table>";
		html += QString( "<tr><td>Name : </td><td>%1</td></tr>" ).arg( node->objectName() );
		html += QString( "<tr><td>Label : </td><td>%1</td></tr>" ).arg( node->getLabel() );
		html += QString( "<tr><td>Class : </td><td>%1</td></tr>" ).arg( node->className() );
		html += QString( "<tr><td>Vertices : </td><td>%1</td></tr>" ).arg( tNumVerts );
		html += QString( "<tr><td>Triangles : </td><td>%1</td></tr>" ).arg( tNumTris );
		html += QString( "<tr><td>Quads : </td><td>%1</td></tr>" ).arg( tNumQuads );
		html += QString( "<tr><td>Total Faces : </td><td>%1</td></tr>" ).arg( tNumTris + tNumQuads );
	}
	else
		// No primary selection
		html = "<b>Primary Selection : </b>None<br/>";

	m_output->append( html );
}

/**
**/
void DzSceneInfoPane::writeSceneStats()
{
	QString	html;

	m_output->clear();
	
	// Generate the 'Scene Info' table:
	html = "<b>Scene Items : </b><br/><table>";
	html += QString( "<tr><td>Nodes : </td><td>%1</td></tr>" ).arg( dzScene->getNumNodes() );
	html += QString( "<tr><td>Lights : </td><td>%1</td></tr>" ).arg( dzScene->getNumLights() );
	html += QString( "<tr><td>Cameras : </td><td>%1</td></tr>" ).arg( dzScene->getNumCameras() );
	html += QString( "<tr><td>World-Space Modifiers : </td><td>%1</td></tr>" )
		.arg( dzScene->getNumWSModifiers() );
	html += "</table><br/><br/>";

	// Generate the 'Scene Geometry' table:
	html += "<b>Scene Geometry : </b><br/><table>";
	html += QString( "<tr><td>Total Vertices : </td><td>%1</td></tr>" ).arg( m_totalVerts );
	html += QString( "<tr><td>Total Triangles : </td><td>%1</td></tr>" ).arg( m_totalTris );
	html += QString( "<tr><td>Total Quads : </td><td>%1</td></tr>" ).arg( m_totalQuads );
	html += QString( "<tr><td>Total Faces : </td><td>%1</td></tr>" ).arg( m_totalTris + m_totalQuads );
	html += "</table><br/><br/>";

	m_output->append( html );
}

/**
**/
void DzSceneInfoPane::blockRefresh()
{
	// A scene file is being loaded - block the refresh functions
	m_refreshBlocked = true;
}

/**
**/
void DzSceneInfoPane::unblockRefresh()
{
	// A scene file just finished loading - unblock the refresh functions, 
	// and force an update of our data.
	m_refreshBlocked = false;
	refresh();
}

/**
**/
void DzSceneInfoPane::getNodeInfo( const DzNode *node, int &numVerts, int &numTris, int &numQuads )
{
	// Collect information about the node's geometry
	DzObject	*obj = node->getObject();
	DzShape		*shape = obj ? obj->getCurrentShape() : NULL;
	DzGeometry	*geom = shape ? shape->getGeometry() : NULL;

	numVerts = 0;
	numTris = 0;
	numQuads = 0;

	if( shape == NULL || geom == NULL )
		return;	// No geometry for the node

	numVerts = geom->getNumVertices();

	DzFacetMesh	*mesh = qobject_cast<DzFacetMesh*>( geom );
	if( mesh ){
		int		i, nFacets = mesh->getNumFacets();
		DzFacet	*facets = mesh->getFacetsPtr();

		for( i = 0; i < nFacets; i++ ){
			if( facets[i].isQuad() )
				numQuads++;
			else
				numTris++;
		}
	}
}
