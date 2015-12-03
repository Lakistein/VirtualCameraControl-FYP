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
#include "dzscene.h"
#include "dzfacetmesh.h"
#include "dzobject.h"
#include "dzinfile.h"
#include "dzoutfile.h"

#include "dzblackholenode.h"
#include "dzblackholemod.h"

/*****************************
   File Section IDs
*****************************/
#define DZ_BLACK_HOLE_NODE_SECTION		0x0200
#define DZ_BLACK_HOLE_RADIUS_SECTION	0x0201

///////////////////////////////////////////////////////////////////////
// DzBlackHoleMod
///////////////////////////////////////////////////////////////////////

/**
**/
DzBlackHoleMod::DzBlackHoleMod() : m_radius( 1.0f ), m_node( NULL )
{
}

/**
**/
DzBlackHoleMod::~DzBlackHoleMod()
{
}

/**
**/
void DzBlackHoleMod::loadSection( DzInFile *file, short sectionID )
{
	switch( sectionID ){
	case DZ_BLACK_HOLE_NODE_SECTION:
		file->readPointer();
		break;
	case DZ_BLACK_HOLE_RADIUS_SECTION:
		file->readFloat( m_radius );
		break;
	default:
		DzWSModifier::loadSection( file, sectionID );
		break;
	}
}

/**
**/
void DzBlackHoleMod::setPointer( const DzInFile *file, short sectionID, short pointerID, DzBase *ptr )
{
	if( sectionID == DZ_BLACK_HOLE_NODE_SECTION )
		setNode( DZ_ASSERT_CAST( ptr, DzBlackHoleNode ) );
	else
		DzWSModifier::setPointer( file, sectionID, pointerID, ptr );
}

/**
**/
DzError DzBlackHoleMod::apply( DzVertexMesh &geom, DzNode &node )
{
	int			i, nVerts = geom.getNumVertices();
	DzPnt3		*verts = geom.getVerticesPtr();
	DzVec3		vec, wsPos;
	DzMatrix3	wsScale;
	DzQuat		wsRot;
	float		dist, radius = m_radius;

	m_node->getWSTransform( wsPos, wsRot, wsScale );
	wsRot.invert();
	wsScale = wsScale.inverse();

	for( i = 0; i < nVerts; i++, verts++ ){
		vec = *verts;

		vec -= wsPos;
		vec = wsRot.multVec( vec );
		vec *= wsScale;

		dist = vec.length() / radius;

		if( dist < 1.0f ){
			vec = *verts;
			vec = wsPos - vec;
			vec *= (1.0f - dist);
			(*verts)[0] += vec.m_x;
			(*verts)[1] += vec.m_y;
			(*verts)[2] += vec.m_z;
		}
	}

	return DZ_NO_ERROR;
}

/**
**/
DzError DzBlackHoleMod::applyInverse( DzVertexMesh &geom, DzNode &node )
{
	// needs to be implemented
	return DZ_NO_ERROR;
}

/**
**/
void DzBlackHoleMod::setNode( DzBlackHoleNode *node )
{
	if( m_node == node )
		return;

	if( m_node ){
		disconnect( m_node, SIGNAL( transformChanged() ), this, SLOT( updateTransform() ) );
		disconnect( m_node, SIGNAL( radiusChanged() ), this, SLOT( updateRadius() ) );
	}

	m_node = node;

	if( m_node ){
		connect( m_node, SIGNAL( transformChanged() ), this, SLOT( updateTransform() ) );
		connect( m_node, SIGNAL( radiusChanged() ), this, SLOT( updateRadius() ) );

		m_radius = m_node->getRadius();
	}
}


/**
**/
void DzBlackHoleMod::updateTransform()
{
	DzObject			*obj;
	DzNodeListIterator	nodeIt( dzScene->nodeListIterator() );

	while( nodeIt.hasNext() ){
		obj = nodeIt.next()->getObject();

		if( obj )
			obj->invalidateCache();
	}
}

/**
**/
void DzBlackHoleMod::updateRadius()
{
	if( m_node )
		m_radius = m_node->getRadius();

	updateTransform();
}
