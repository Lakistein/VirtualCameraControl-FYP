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
#include <math.h>

#include <QtGui/QPixmap>

#include <dzopengl.h>
#include <dzfloatproperty.h>
#include <dzdrawstyle.h>
#include <dzinfile.h>
#include <dzoutfile.h>
#include <dzmatrix4.h>
#include <dzscene.h>

#include "dzblackholemod.h"
#include "dzblackholenode.h"

/*****************************
   Definitions
*****************************/
// File Section IDs
#define DZ_BLACK_HOLE_RADIUS_SECTION	0x0200

///////////////////////////////////////////////////////////////////////
// DzBlackHoleNode
///////////////////////////////////////////////////////////////////////

/**
**/
DzBlackHoleNode::DzBlackHoleNode()
{
	// Calling DzElement::addProperty() adds the property to the element, which takes ownership
	// of the property (DzElement is in charge of deleting this object). DzElement also is
	// responsible for saving and loading properties via the native file format. In this case, 
	// when this class is loaded from a file, it's constructor will be called, creating a new 
	// radius property. Then, when DzElement loads the properties from the file, it will delete 
	// the existing radius property and replace it with the new one. We are responsible for updating
	// our local pointer when that happens.
	m_radius = new DzFloatProperty( "Radius", true, false, 100.0f );
	addProperty( m_radius );

	connect( m_radius, SIGNAL( currentValueChanged() ), this, SIGNAL( radiusChanged() ) );
	connect( m_radius, SIGNAL( currentValueChanged() ), this, SIGNAL( drawnDataChanged() ) );
}

/**
**/
DzBlackHoleNode::~DzBlackHoleNode()
{
}

/**
**/
void DzBlackHoleNode::loadSection( DzInFile *file, short sectionID )
{
	if( sectionID == DZ_BLACK_HOLE_RADIUS_SECTION )
		file->readPointer();
	else
		DzNode::loadSection( file, sectionID );
}

/**
**/
void DzBlackHoleNode::setPointer( const DzInFile *file, short sectionID, short pointerID, DzBase *ptr )
{
	if( sectionID == DZ_BLACK_HOLE_RADIUS_SECTION ){
		// Here we match up our local radius pointer with the one read in from the file
		// by DzElement (DzElement has already deleted the one we created, see the comments
		// in the constructor). In this case, we chose to save a reference to the pointer in
		// the file, and then read it back in since this is a very fool-proof method.
		// Another approach would be to reimplement the postLoadFile() method in DzBlackHoleNode, 
		// and search the property list to update the m_radius pointer - this method is completely 
		// reasonable, and would even cut out a dozen bytes from the file footprint of the DzBlackHoleNode.
		m_radius = DZ_ASSERT_CAST( ptr, DzFloatProperty );
		connect( m_radius, SIGNAL( currentValueChanged() ), this, SIGNAL( radiusChanged() ) );
		connect( m_radius, SIGNAL( currentValueChanged() ), this, SIGNAL( drawnDataChanged() ) );
	}
	else
		DzNode::setPointer( file, sectionID, pointerID, ptr );
}

/**
**/
void DzBlackHoleNode::draw( const DzDrawStyle &style ) const
{
	// don't draw anything for renders, etc
	if( style.drawObjectsOnly() )
		return;

	// draw our interactive representation
	int			sides = 12;
	int			segments = 6;
	int			i, j, numVerts = segments - 1, centerSegs = segments - 2;
	float		cosVal, x, y, z, angle;
	float		radius = m_radius->getValue();
	DzVec3		wsPos, axis;
	DzMatrix3	wsScale;
	DzQuat		wsRot;

	glPushAttrib( GL_LIGHTING_BIT );
	glDisable( GL_LIGHTING );

	glPushMatrix();

	getWSTransform( wsPos, wsRot, wsScale );
	glTranslatef( wsPos[0], wsPos[1], wsPos[2] );

	wsRot.getValue( axis, angle );
	angle *= DZ_FLT_RAD_TO_DEG;
	glRotatef( -angle, axis[0], axis[1], axis[2] );

	glMultMatrixf( (const float*) wsScale.matrix4() );

	glColor3f( 0.0f, 0.0f, 0.0f );

	glBegin( GL_LINES );

	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, -radius, 0.0f );

	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, radius, 0.0f );

	for( i = 0; i < sides; i++ ){
		angle = DZ_FLT_2_PI * i / sides;
		x = sin( angle ) * radius;
		z = cos( angle ) * radius;
		for( j = 0; j < numVerts; j++ ){
			angle = DZ_FLT_PI * (j + 1) / segments;
			y = -cos( angle ) * radius;
			cosVal = sin( angle );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( x * cosVal, y, z * cosVal );
		}
	}

	glEnd();

	glPopMatrix();
	glPopAttrib();
}

/**
**/
void DzBlackHoleNode::render( const DzRenderSettings &settings ) const
{
	// we never render anything
}

/**
**/
float DzBlackHoleNode::getRadius() const
{
	return m_radius->getValue();
}






DzBlackHoleNodeIO::DzBlackHoleNodeIO()
{
}

DzBlackHoleNodeIO::~DzBlackHoleNodeIO()
{
}

DzNode* DzBlackHoleNodeIO::createNode() const
{
	return new DzBlackHoleNode();
}

DzError DzBlackHoleNodeIO::writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzError DzBlackHoleNodeIO::writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzAssetJsonObject* DzBlackHoleNodeIO::startDefinitionRead( DzAssetJsonItem* parentItem )
{
	return 0;
}

DzAssetJsonObject* DzBlackHoleNodeIO::startInstanceRead( DzAssetJsonItem* parentItem )
{
	return 0;
}

DzError DzBlackHoleNodeIO::applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzError DzBlackHoleNodeIO::applyInstanceToObject( QObject* object, const DzFileIOSettings* opts )const
{
	DzBlackHoleNode* node = qobject_cast<DzBlackHoleNode*>(object);

	DzBlackHoleMod	*mod = new DzBlackHoleMod;
	mod->setNode(node );
	dzScene->addWSModifier( mod, 0 );

	return DZ_NO_ERROR;
}

void DzBlackHoleNodeIO::getOwnedAssets(QObject* object, QList< IDzSceneAsset* >& assets)
{
}
