/**********************************************************************
	Copyright (C) 2002-2012 DAZ 3D, Inc. All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of DAZ 3D, Inc;
	the contents of this file may not be disclosed to third parties,
	copied or duplicated in any form, in whole or in part, without the
	prior written permission of DAZ 3D, Inc.
**********************************************************************/
#ifndef DZ_DzWeaveBrick_BRICK_H
#define DZ_DzWeaveBrick_BRICK_H
/*****************************
	Include files
*****************************/
#include "dzshaderbrick.h"


class DzCurveBrick : public DzShaderBrick
{
	Q_OBJECT
public:
	DzCurveBrick();
	virtual ~DzCurveBrick();

public slots:
	virtual DzBrickSet::ShaderType	type() const {return DzBrickSet::None;};
	virtual QString		getPreferredName() const {return "Curve Hair";};
	virtual QString		getDefaultCategory() const {return "Lighting/Special/";};

protected:
	virtual DzShaderString*	getBrickClassShaderCode() const;
	virtual DzShaderString*	getShaderCodeCall(const QStringList& varCallNames,
										const QStringList& varArgNames) const;


private:
	struct Data;
	Data* m_data;

};
#endif // DZ_DzWeaveBrick_BRICK_H
