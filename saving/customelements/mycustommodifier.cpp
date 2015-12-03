/**********************************************************************
	Copyright (C) 2011 DAZ 3D, Inc. All Rights Reserved.

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

#include "mycustommodifier.h"
#include <dzvertexmesh.h>
#include <idzjsonio.h>
#include <dzobject.h>

MyCustomModifier::MyCustomModifier()
	: m_seed(qrand())
{
	setObjectName("MyCustomModifier");
}

MyCustomModifier::~MyCustomModifier()
{
}

DzError	MyCustomModifier::apply( DzVertexMesh &geom, DzNode &node )
{
	int preSeed = qrand();
	qsrand(m_seed);

	DzPnt3* verts = geom.getVerticesPtr();
	for (int i = 0, n = geom.getNumVertices(); i < n ; i++)
	{
		verts[i][0] += (qrand() % 100) * 0.005f;
		verts[i][1] += (qrand() % 100) * 0.005f;
		verts[i][2] += (qrand() % 100) * 0.005f;
	}

	qsrand(preSeed);
	return DZ_NO_ERROR;
}

DzError	MyCustomModifier::applyInverse( DzVertexMesh &geom, DzNode &node )
{
	return DZ_NO_ERROR;
}

const QObject* MyCustomModifier::toQObject() const
{
	return this;
}

QObject* MyCustomModifier::toQObject()
{
	return this;
}

IDzSceneAsset::AssetType MyCustomModifier::getAssetType() const
{
	return ModifierAsset;
}



MyCustomModifierIO::MyCustomModifierIO()
	: m_context(0)
{
}

MyCustomModifierIO::~MyCustomModifierIO()
{
	delete m_context;
}

DzModifier* MyCustomModifierIO::createModifier(const QString& name, DzObject* tgtObject) const
{
	DzModifier* modifier = new MyCustomModifier();
	tgtObject->addModifier(modifier);
	return modifier;
}

DzError MyCustomModifierIO::writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	io->addMember("seed", 1);
	return DZ_NO_ERROR;
}

DzError MyCustomModifierIO::writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	MyCustomModifier* modifier = qobject_cast<MyCustomModifier*>(object);

	io->addMember("seed", modifier->m_seed);
	
	return DZ_NO_ERROR;
}

DzAssetJsonObject* MyCustomModifierIO::startDefinitionRead( DzAssetJsonItem* parentItem )
{
	return 0;
}

DzAssetJsonObject* MyCustomModifierIO::startInstanceRead( DzAssetJsonItem* parentItem )
{
	m_context = new Context(parentItem->getFile());
	return new ReadMyCustomModifier(m_context);
}

DzError MyCustomModifierIO::applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzError MyCustomModifierIO::applyInstanceToObject( QObject* object, const DzFileIOSettings* opts )const
{
	MyCustomModifier* modifier = qobject_cast<MyCustomModifier*>(object);

	modifier->m_seed = m_context->m_seed;

	return DZ_NO_ERROR;
}


bool ReadMyCustomModifier::addMember( const QString &name, double val )
{
	if (name == "seed") {
		m_context->m_seed = val;
	}
	else {
		return DzAssetJsonObject::addMember(name, val);
	}
	return true;
}
