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
#pragma once

#include <QtGui/QColor>

#include <dzmaterial.h>
#include <dzassetextraobjectio.h>

class DzFloatProperty;

/*
 Test by creating a primitive, selecting it, and then executing the following in the script IDE
  Scene.getPrimarySelection().getObject().getCurrentShape().replaceMaterial(Scene.getPrimarySelection().getObject().getCurrentShape().getMaterial(0), new MyCustomMaterial());

*/

class MyCustomMaterial : public DzMaterial {
	friend class MyCustomNodeIO;
	Q_OBJECT
public:
    MyCustomMaterial();
    ~MyCustomMaterial();


	virtual void	    setDiffuseColor( const QColor &color ) {}
	virtual void	    setBaseOpacity( float val )            {}
	virtual void	    setColorMap( const DzTexture *img )    {}
	virtual void	    setOpacityMap( const DzTexture *img )  {}

	virtual QColor		getDiffuseColor() const            { return QColor(0, 0, 255, 255); }
	virtual double		getBaseOpacity() const             { return 1; }
	virtual bool		isColorMappable() const            { return false; }
	virtual bool		isOpacityMappable() const          { return false; }
	virtual DzTexture*	getColorMap() const                { return 0; }
	virtual DzTexture*	getOpacityMap() const              { return 0; }
	virtual QString		getMaterialName() const            { return "MyMaterialName"; }
	virtual bool		needsTangentSpaceParams() const    { return false; }
	virtual bool		shouldAlwaysEmbed() const          { return false; }
	virtual bool		allowsAutoBake() const             { return false; }

	virtual void	    setGLMat( const DzDrawStyle &style ) const;
	virtual void	    unsetGLMat( const DzDrawStyle &style ) const;
	virtual void	    setShader( const DzRenderSettings &settings ) const;
	virtual void	    setCoShader( const DzRenderSettings &settings, DzCoShaderInfo& info ) const;

	bool toggle() const     { return m_bool; }
	void setToggle(bool v)  { m_bool = v; }

private:
	DzFloatProperty* m_property;
	bool             m_bool;
};


class MyCustomMaterialIO : public DzExtraMaterialIO {
Q_OBJECT
public:
	MyCustomMaterialIO();
	~MyCustomMaterialIO();

	// DzExtraNodeIO
	virtual void               setIncludeProperties( DzPropertyList *includeProps );
	virtual DzMaterial*        createMaterial() const;
	virtual bool               needToReplaceMaterial(const DzMaterial* currentMaterial);
	virtual DzError            writeExtraDefinition( QObject *object, IDzJsonIO *io, const DzFileIOSettings *opts ) const;
	virtual DzError            writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzAssetJsonObject* startDefinitionRead( DzAssetJsonItem *parentItem );
	virtual DzAssetJsonObject* startInstanceRead( DzAssetJsonItem* parentItem );
	virtual DzError	           applyDefinitionToObject( QObject *object, const DzFileIOSettings *opts ) const;
	virtual DzError            applyInstanceToObject( QObject* object, const DzFileIOSettings* opts ) const;

	// for reading
	struct Context;
	Context* m_context;
};

struct MyCustomMaterialIO::Context {
	Context(DzAssetFile& file)
		: m_file(file), m_bool(false)
	{}

	DzAssetFile& m_file;
	bool         m_bool;
};


class ReadMyCustomMaterial : public DzAssetJsonObject {
public:
	ReadMyCustomMaterial(MyCustomMaterialIO::Context* context)
		: DzAssetJsonObject(context->m_file), m_context(context)
	{}

	virtual bool addMember( const QString &name, bool val );

	MyCustomMaterialIO::Context* m_context;
};
