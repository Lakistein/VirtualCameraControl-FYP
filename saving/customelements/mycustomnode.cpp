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
#include "mycustomnode.h"

#include <dzfloatproperty.h>
#include <idzjsonio.h>

static int s_ticket = 0;

MyCustomNode::MyCustomNode()
{
	// this keeps track of which number this custom node was created since launch
	//  for something to save that is not a property.
	m_myInt = ++s_ticket;

	// create a property and add it to the scene
	m_property = new DzFloatProperty("MyCustomFloat", true, false, 0);
	m_property->setLabel("Custom Float");
	m_property->setPath("General/Custom");
	m_property->setIsMappable(true);

	addProperty(m_property);
}

MyCustomNode::~MyCustomNode()
{
}


MyCustomNodeIO::MyCustomNodeIO()
	: m_context(0)
{
}

MyCustomNodeIO::~MyCustomNodeIO()
{
	delete m_context;
}

DzNode* MyCustomNodeIO::createNode() const
{
	return new MyCustomNode();
}

DzError MyCustomNodeIO::writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzError MyCustomNodeIO::writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	MyCustomNode* node = qobject_cast<MyCustomNode*>(object);

	io->addMember("my_int", node->m_myInt);
	
	return DZ_NO_ERROR;
}

DzAssetJsonObject* MyCustomNodeIO::startDefinitionRead( DzAssetJsonItem* parentItem )
{
	return 0;
}

DzAssetJsonObject* MyCustomNodeIO::startInstanceRead( DzAssetJsonItem* parentItem )
{
	m_context = new Context(parentItem->getFile());
	return new ReadMyCustomNode(m_context);
}

DzError MyCustomNodeIO::applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzError MyCustomNodeIO::applyInstanceToObject( QObject* object, const DzFileIOSettings* opts )const
{
	MyCustomNode* node = qobject_cast<MyCustomNode*>(object);

	node->m_myInt = m_context->m_myInt;

	return DZ_NO_ERROR;
}

void MyCustomNodeIO::getOwnedAssets(QObject* object, QList< IDzSceneAsset* >& assets)
{
	MyCustomNode* node = qobject_cast<MyCustomNode*>(object);

	// we don't need to do this since we added our property to the node.
	//  this is shown as an example for when you have assets that are not known to the system.
	//assets.push_back(node->m_property);
}

bool ReadMyCustomNode::addMember( const QString &name, double val )
{
	if (name == "my_int") {
		m_context->m_myInt = val;
	}
	else {
		return DzAssetJsonObject::addMember(name, val);
	}
	return true;
}
