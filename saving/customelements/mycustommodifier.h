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
#pragma once

#include <dzmodifier.h>
#include <idzsceneasset.h>
#include <dzassetextraobjectio.h>

/* test on a figure by executing the following in the Script IDE 

	Scene.getPrimarySelection().getObject().addModifier(new MyCustomModifier());
*/


class MyCustomModifier : public DzModifier, public DzSceneAsset {
	Q_OBJECT
	Q_INTERFACES( IDzSceneAsset )
	friend class MyCustomModifierIO;
public:
	MyCustomModifier();
	~MyCustomModifier();

	// DzModifier
	virtual DzError			apply( DzVertexMesh &geom, DzNode &node );
	virtual DzError			applyInverse( DzVertexMesh &geom, DzNode &node );

	// DzSceneAsset
	virtual const QObject*	toQObject() const;
	virtual QObject*		toQObject();
	virtual AssetType		getAssetType() const;

signals:
	void	assetModified();
	void	assetWasSaved();

private:
	int m_seed;
};


class MyCustomModifierIO : public DzExtraModifierIO {
Q_OBJECT
public:
	MyCustomModifierIO();
	~MyCustomModifierIO();

	// DzExtraNodeIO
	virtual DzModifier*			createModifier( const QString& name, DzObject* tgtObject ) const;
	virtual DzError				writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzError				writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzAssetJsonObject*	startDefinitionRead( DzAssetJsonItem* parentItem );
	virtual DzAssetJsonObject*	startInstanceRead( DzAssetJsonItem* parentItem );
	virtual DzError				applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts ) const;
	virtual DzError				applyInstanceToObject( QObject* object, const DzFileIOSettings* opts ) const;

	// for reading
	struct Context;
	Context* m_context;
};

struct MyCustomModifierIO::Context
{
	Context(DzAssetFile& file)
		: m_file(file), m_seed(-1)
	{}

	DzAssetFile& m_file;
	int          m_seed;
};


class ReadMyCustomModifier : public DzAssetJsonObject
{
public:
	ReadMyCustomModifier(MyCustomModifierIO::Context* context)
		: DzAssetJsonObject(context->m_file), m_context(context)
	{}

	virtual bool addMember( const QString &name, double val );

	MyCustomModifierIO::Context* m_context;
};
