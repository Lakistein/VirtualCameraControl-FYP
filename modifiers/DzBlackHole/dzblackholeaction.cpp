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

#include "dzblackholemod.h"
#include "dzblackholenode.h"
#include "dzblackholeaction.h"

///////////////////////////////////////////////////////////////////////
// DzBlackHoleAction
///////////////////////////////////////////////////////////////////////

/**
**/
DzBlackHoleAction::DzBlackHoleAction() : DzCreateItemAction( "New BlackHole", "Create a new BlackHole" )
{
}

/**
**/
void DzBlackHoleAction::executeAction()
{
	QString		name = "BlackHole";
	DzNode		*node = dzScene->findNode( name );

	if( node ){
		int		blackHoleCount = 2; 
		name = QString( "BlackHole %1" ).arg( blackHoleCount );
		while( dzScene->findNode( name ) ){
			blackHoleCount++;
			name = QString( "BlackHole %1" ).arg( blackHoleCount );
		}
	}

	node = new DzBlackHoleNode;

	DzBlackHoleMod	*mod = new DzBlackHoleMod;

	node->setName( name.toAscii() );
	//node->setLabel( "Black Hole" );

	mod->setNode( (DzBlackHoleNode*)node );

	dzScene->addNode( node );
	dzScene->addWSModifier( mod, 0 );
}
