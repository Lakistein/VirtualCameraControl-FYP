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

#ifndef DAZ_BLACK_HOLE_NODE_H
#define DAZ_BLACK_HOLE_NODE_H

/*****************************
   Include files
*****************************/
#include <dznode.h>
#include <dzassetextraobjectio.h>

#include <QtGui/QPixmap>

/****************************
   Forward declarations
****************************/

class DzBlackHoleMod;
class DzFloatProperty;

/*****************************
   Class definitions
*****************************/
/**
	
**/
class DzBlackHoleNode : public DzNode {
	Q_OBJECT
public:
    //
    // CREATORS/DESTROYERS
    //

    /**
	 *	Default Constructor
	 */
    DzBlackHoleNode();

	// Destructor
	~DzBlackHoleNode();

	//////////////////////////////
	// keep these for backwards compatiblity
	virtual void	loadSection( DzInFile *file, short sectionID );
	virtual void	setPointer( const DzInFile *file, short sectionID, short pointerID, DzBase *ptr );


	////////////////////////////
	// from DzNode
	virtual void	draw( const DzDrawStyle &style ) const;
	virtual void	render( const DzRenderSettings &settings ) const;

	float			getRadius() const;

signals:
	void			radiusChanged();

private:
	DzFloatProperty	*m_radius;

};


// all we really need is the createNode, radius is saved by the system
class DzBlackHoleNodeIO : public DzExtraNodeIO {
Q_OBJECT
public:
	DzBlackHoleNodeIO();
	~DzBlackHoleNodeIO();

	// DzExtraNodeIO
	virtual DzNode*				createNode() const;
	virtual DzError				writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzError				writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzAssetJsonObject*	startDefinitionRead( DzAssetJsonItem* parentItem );
	virtual DzAssetJsonObject*	startInstanceRead( DzAssetJsonItem* parentItem );
	virtual DzError				applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts ) const;
	virtual DzError				applyInstanceToObject( QObject* object, const DzFileIOSettings* opts ) const;

	virtual void				getOwnedAssets( QObject* object, QList< IDzSceneAsset* >& assets );

};

#endif // DAZ_BLACK_HOLE_NODE_H
