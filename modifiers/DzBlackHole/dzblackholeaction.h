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

#ifndef DAZ_BLACK_HOLE_ACTION_H
#define DAZ_BLACK_HOLE_ACTION_H

/*****************************
   Include files
*****************************/
#include "dzaction.h"

class DzBlackHoleAction : public DzCreateItemAction {
	Q_OBJECT
public:
	DzBlackHoleAction();
protected:
	virtual void	executeAction();
};

#endif // DAZ_BLACK_HOLE_ACTION_H