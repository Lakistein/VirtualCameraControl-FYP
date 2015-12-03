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

#include <dznode.h>
#include <dzassetextraobjectio.h>

class DzFloatProperty;

class MyCustomNode : public DzNode {
	friend class MyCustomNodeIO;
	Q_OBJECT
public:
    MyCustomNode();
    ~MyCustomNode();

private:
	DzFloatProperty* m_property;
	int              m_myInt;
};


class MyCustomNodeIO : public DzExtraNodeIO {
Q_OBJECT
public:
	MyCustomNodeIO();
	~MyCustomNodeIO();

	// DzExtraNodeIO
	virtual DzNode*				createNode() const;
	virtual DzError				writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzError				writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts ) const;
	virtual DzAssetJsonObject*	startDefinitionRead( DzAssetJsonItem* parentItem );
	virtual DzAssetJsonObject*	startInstanceRead( DzAssetJsonItem* parentItem );
	virtual DzError				applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts ) const;
	virtual DzError				applyInstanceToObject( QObject* object, const DzFileIOSettings* opts ) const;

	virtual void				getOwnedAssets( QObject* object, QList< IDzSceneAsset* >& assets );

	// for reading
	struct Context;
	Context* m_context;
};

struct MyCustomNodeIO::Context {
	Context(DzAssetFile& file)
		: m_file(file), m_myInt(-1)
	{}

	DzAssetFile& m_file;
	int          m_myInt;
};


class ReadMyCustomNode : public DzAssetJsonObject {
public:
	ReadMyCustomNode(MyCustomNodeIO::Context* context)
		: DzAssetJsonObject(context->m_file), m_context(context)
	{}

	virtual bool addMember( const QString &name, double val );

	MyCustomNodeIO::Context* m_context;
};
