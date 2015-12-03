/**********************************************************************
	Copyright (C) 2012 DAZ 3D, Inc. All Rights Reserved.

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
#include "mycustommaterial.h"
#include <dzopengl.h>
#include <dzfloatproperty.h>
#include <idzjsonio.h>


MyCustomMaterial::MyCustomMaterial()
{
	// this custom bool value is shown only for completeness.  Almost for sure you will want to use
	// properties as shown below.
	m_bool = (rand() & 0x1) == 1;

	// create a property and add it to the scene,
	//  no furthur IO work is needed as DzMaterial handles any properties we add
	m_property = new DzFloatProperty("MyCustomFloat", false, false, 0);
	m_property->setLabel("Custom Float");
	m_property->setPath("General/Custom");
	m_property->setIsMappable(false);
	m_property->setMinMax(0, 1);
	m_property->setIsClamped(true);

	addProperty(m_property);
}

MyCustomMaterial::~MyCustomMaterial()
{
}

void MyCustomMaterial::setGLMat( const DzDrawStyle &style ) const
{
	float v = m_property->getValue();

	// purple to blue in this case, black to yellow in the else
	if (m_bool)
		glColor4f(1.0f - v, 0, v, 1);
	else
		glColor4f(0, v, 0, 1);
}

void MyCustomMaterial::unsetGLMat( const DzDrawStyle &style ) const
{
	// if we had turned on anything like glEnable(GL_TEXTURE), we would want to clean that up.
}

void MyCustomMaterial::setShader( const DzRenderSettings &settings ) const
{
}

void MyCustomMaterial::setCoShader( const DzRenderSettings &settings, DzCoShaderInfo& info ) const
{
}



MyCustomMaterialIO::MyCustomMaterialIO()
	: m_context(0)
{
}

MyCustomMaterialIO::~MyCustomMaterialIO()
{
	delete m_context;
}

void MyCustomMaterialIO::setIncludeProperties( DzPropertyList *includeProps )
{
}

DzMaterial*	MyCustomMaterialIO::createMaterial() const
{
	return new MyCustomMaterial();
}

bool MyCustomMaterialIO::needToReplaceMaterial(const DzMaterial* currentMaterial)
{
	return DzExtraMaterialIO::needToReplaceMaterial(currentMaterial);
}


DzError MyCustomMaterialIO::writeExtraDefinition( QObject *object, IDzJsonIO *io, const DzFileIOSettings *opts ) const
{
	return DZ_NO_ERROR;
}

DzError MyCustomMaterialIO::writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	MyCustomMaterial* material = qobject_cast<MyCustomMaterial*>(object);

	io->addMember("my_toggle", material->toggle());

	return DZ_NO_ERROR;
}

DzAssetJsonObject* MyCustomMaterialIO::startDefinitionRead( DzAssetJsonItem *parentItem )
{
	return 0;
}

DzAssetJsonObject* MyCustomMaterialIO::startInstanceRead( DzAssetJsonItem* parentItem )
{
	m_context = new Context(parentItem->getFile());
	return new ReadMyCustomMaterial(m_context);
}


DzError MyCustomMaterialIO::applyDefinitionToObject( QObject *object, const DzFileIOSettings *opts ) const
{
	return DZ_NO_ERROR;
}

DzError MyCustomMaterialIO::applyInstanceToObject( QObject* object, const DzFileIOSettings* opts )const
{
	MyCustomMaterial* material = qobject_cast<MyCustomMaterial*>(object);

	material->setToggle(m_context->m_bool);

	return DZ_NO_ERROR;
}

bool ReadMyCustomMaterial::addMember( const QString &name, bool val )
{
	if (name == "my_toggle") {
		m_context->m_bool = val;
	}
	else {
		return DzAssetJsonObject::addMember(name, val);
	}
	return true;
}
