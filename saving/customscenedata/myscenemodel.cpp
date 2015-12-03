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
#include "myscenemodel.h"
#include <dzscene.h>
#include <idzjsonio.h>

MySceneModel::MySceneModel()
	: DzSceneData("MySceneModel", true)
{
}

MySceneModel::~MySceneModel()
{
}

bool MySceneModel::isEmpty()
{
	return m_coolText == "";
}

const QString& MySceneModel::coolText()
{
	return m_coolText;
}

void MySceneModel:: setCoolText(const QString& value)
{
	if (m_coolText != value)
	{
		m_coolText = value;
		emit coolTextChanged(m_coolText);
	}
}




// MySceneModelIO
MySceneModelIO::MySceneModelIO()
	: m_context(0)
{
}

MySceneModelIO::~MySceneModelIO()
{
	delete m_context;
}

DzSceneData* MySceneModelIO::createDataItem( const DzFileIOSettings *opts ) const
{
	// DzSceneData is a singleton(meaning there is only every one of this type in the scene). 
	//   We already added it in MyScenePane::sceneCleared
	return dzScene->findDataItem("MySceneModel");
}

bool MySceneModelIO::shouldWrite( QObject* object, const DzFileIOSettings* opts )const
{
	MySceneModel* model = qobject_cast<MySceneModel*>(object);

	return !model->isEmpty();
}

DzError MySceneModelIO::writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	MySceneModel* model = qobject_cast<MySceneModel*>(object);

	// we start in an JSON object, this is the simplest case
	io->addMember("cool_text", model->coolText());

	return DZ_NO_ERROR;
}

DzAssetJsonObject* MySceneModelIO::startInstanceRead( DzAssetJsonItem* parentItem )
{
	m_context = new MyReadContext(parentItem->getFile());
	return new ReadMySceneModel(m_context);
}

DzError MySceneModelIO::applyInstanceToObject( QObject* object, const DzFileIOSettings* opts )const
{
	MySceneModel* model = qobject_cast<MySceneModel*>(object);

	model->setCoolText(m_context->m_coolText);

	return DZ_NO_ERROR;
}

DzError MySceneModelIO::resolveInstance( QObject* object, const DzFileIOSettings* opts )const
{
	MySceneModel* model = qobject_cast<MySceneModel*>(object);

	// this is where we would resolve node instances


	return DZ_NO_ERROR;
}


// ReadMySceneModel

bool ReadMySceneModel::addMember( const QString &name, const QString &val )
{
	if (name == "cool_text") {
		m_context->m_coolText = val;
	}
	else {
		return DzAssetJsonObject::addMember(name, val);
	}
	return true;
}
