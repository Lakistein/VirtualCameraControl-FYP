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

#include "mycustomshape.h"
#include <dzvertexmesh.h>
#include <idzjsonio.h>
#include <dzobject.h>
#include <dzdrawstyle.h>
#include <dzfloatcolor.h>

struct MyNurbShape::Data
{
	MyNurbGeometry*      m_geom;
	mutable DzFacetMesh* m_mesh;
};

MyNurbShape::MyNurbShape(MyNurbGeometry* geom)
	: m_data(new Data)
{
	static int handle = 0;
	setObjectName(QString("MyNurbShape%1").arg(++handle));
	if (!geom)
		geom = new MyNurbGeometry();
	m_data->m_geom = geom;
}

MyNurbShape::~MyNurbShape()
{
	delete m_data;
}

DzGeometry* MyNurbShape::getGeometry() const
{
	return m_data->m_geom;
}

DzVertexMesh* MyNurbShape::getModifiableGeom( bool isRender, DzVertexMesh *currentCache ) const
{
	m_data->m_mesh = new DzFacetMesh();
	m_data->m_mesh->setVertexArray(m_data->m_geom->getNumVertices());
	m_data->m_mesh->removeAllFacets();

	DzPnt3* a = m_data->m_geom->getVerticesPtr();
	DzPnt3* b = m_data->m_mesh->getVerticesPtr();

	for(int i = 0, n = m_data->m_geom->getNumVertices(); i < n; i++)
	{
		b[i][0] = a[i][0];
		b[i][1] = a[i][1];
		b[i][2] = a[i][2];
	}

	return m_data->m_mesh;
}

void MyNurbShape::draw( const DzDrawStyle &style, const DzGeometry *wsGeom, const DzNode *node ) const
{
	if( style.shadeStyle() == DzDrawStyle::PICKING ) {
		return;
	}

	switch( style.shadeStyle() ) {
	case DzDrawStyle::SmoothShaded:
	case DzDrawStyle::Textured:
		{
			if (!m_data->m_mesh->getNumFacets())
				m_data->m_geom->tesselate(m_data->m_mesh);

			DzFloatColor diffuse(1, 1, 1);
			DzFloatColor specular(1, 1, 1);
			DzFloatColor ambient(0, 0, 0);

			glColor4fv( diffuse.getValue() );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient.getValue() );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular.getValue() );

			glBegin(GL_QUADS);
			DzPnt3* vertices = m_data->m_mesh->getVerticesPtr();
			DzPnt3* normals = m_data->m_mesh->getNormalsPtr();
			DzFacet* facets = m_data->m_mesh->getFacetsPtr();
			for (int i = 0, n = m_data->m_mesh->getNumFacets(); i < n; i++)
			{
				glNormal3fv(normals[facets[i].m_normIdx[0]]);
				glVertex3fv(vertices[facets[i].m_vertIdx[0]]);

				glNormal3fv(normals[facets[i].m_normIdx[1]]);
				glVertex3fv(vertices[facets[i].m_vertIdx[1]]);

				glNormal3fv(normals[facets[i].m_normIdx[2]]);
				glVertex3fv(vertices[facets[i].m_vertIdx[2]]);

				glNormal3fv(normals[facets[i].m_normIdx[3]]);
				glVertex3fv(vertices[facets[i].m_vertIdx[3]]);
			}
			glEnd();
		}
		break;
	default:
		{
			glColor4f(1, 1, 0, 1);

			glBegin(GL_POINTS);
			DzPnt3* vertices = wsGeom->getVerticesPtr();
			for (int i = 0, n = wsGeom->getNumVertices(); i < n; i++)
			{
				glVertex3fv(vertices[i]);
			}
			glEnd();
		}
		break;
	}

}

void MyNurbShape::render( const DzRenderSettings &settings, const DzGeometry *wsGeom, const DzNode *node ) const
{
}


void MyNurbShape::invalidateWorkingMesh()
{
}



struct MyNurbGeometry::Data
{
	int dimensionU;
	int dimensionV;
	QVector<float> m_weights;
	QVector<float> m_knotVectorU;
	QVector<float> m_knotVectorV;
};


static double s_vertices[] = {
	-1.92061738543705,-1.22071424910243e-016,1.9935776583948,1,-0.640205795145684,-1.22071424910243e-016,1.9935776583948,1,0.640205795145684,-1.22071424910243e-016,1.9935776583948,1,1.92061738543705,-1.22071424910243e-016,1.9935776583948,1,-1.92061738543705,-4.0690474970081e-017,0.664525886131598,1,-0.640205795145684,-4.0690474970081e-017,0.664525886131598,1,0.640205795145684,-4.0690474970081e-017,0.664525886131598,1,1.92061738543705,-4.0690474970081e-017,0.664525886131598,1,-1.92061738543705,4.0690474970081e-017,-0.664525886131598,1,-0.640205795145684,4.0690474970081e-017,-0.664525886131598,1,0.640205795145684,4.0690474970081e-017,-0.664525886131598,1,1.92061738543705,4.0690474970081e-017,-0.664525886131598,1,-1.92061738543705,1.22071424910243e-016,-1.9935776583948,1,-0.640205795145684,1.22071424910243e-016,-1.9935776583948,1,0.640205795145684,1.22071424910243e-016,-1.9935776583948,1,1.92061738543705,1.22071424910243e-016,-1.9935776583948,1
};

MyNurbGeometry::MyNurbGeometry()
	: m_data(new Data)
{
	static int handle = 0;
	setObjectName(QString("MyNurbGeometry%1").arg(++handle));

	m_data->dimensionU = 0;
	m_data->dimensionV = 0;

}

MyNurbGeometry::~MyNurbGeometry()
{
	delete m_data;
}

void MyNurbGeometry::setupExample()
{
	m_data->dimensionU = 4;
	m_data->dimensionV = 4;

	m_data->m_knotVectorU.push_back(0);
	m_data->m_knotVectorU.push_back(0);
	m_data->m_knotVectorU.push_back(0);
	m_data->m_knotVectorU.push_back(0);
	m_data->m_knotVectorU.push_back(1);
	m_data->m_knotVectorU.push_back(1);
	m_data->m_knotVectorU.push_back(1);
	m_data->m_knotVectorU.push_back(1);

	m_data->m_knotVectorV.push_back(0);
	m_data->m_knotVectorV.push_back(0);
	m_data->m_knotVectorV.push_back(0);
	m_data->m_knotVectorV.push_back(0);
	m_data->m_knotVectorV.push_back(1);
	m_data->m_knotVectorV.push_back(1);
	m_data->m_knotVectorV.push_back(1);
	m_data->m_knotVectorV.push_back(1);
	
	beginEdit(false);
	for (int i = 0; i < m_data->dimensionU; i++)
	{
		for( int j = 0 ; j < m_data->dimensionV; j++)
		{
			int index = (i*m_data->dimensionV+j)*4;
			addVertex(
				s_vertices[index+0],
				s_vertices[index+1],
				s_vertices[index+2]
			); 
			m_data->m_weights.push_back(
				s_vertices[index+3]
			);
		}
	}
	finishEdit();
}

const QObject* MyNurbGeometry::toQObject() const
{
	return this;
}

QObject* MyNurbGeometry::toQObject()
{
	return this;
}

IDzSceneAsset::AssetType MyNurbGeometry::getAssetType() const
{
	return GeometryAsset;
}




DzVertexMesh* MyNurbGeometry::makeCopy( bool shareVerts, bool shareFacets ) const
{
	return 0;
}


bool MyNurbGeometry::copyFrom( DzVertexMesh *mesh, bool shareVerts /*= false*/, bool shareFacets /*= false */ )
{
	return false;
}


DzBox3 MyNurbGeometry::getGroupBoundingBox( const QString &groupName ) const
{
	return getBoundingBox();
}

DzOrientedBox3	MyNurbGeometry::getGroupOrientedBox( const QString &groupName, const DzVec3 &pos, const DzQuat &rot, const DzVec3 &scale ) const
{
	return getOrientedBox(pos, rot, scale);
}

DzBox3 MyNurbGeometry::getGroupBoundingBox( const QString &groupName, const DzMatrix3 &transform ) const
{
	return getBoundingBox();
}

DzOrientedBox3 MyNurbGeometry::getGroupOrientedBox( const QString &groupName, const DzMatrix3 &transform, const DzVec3 &pos, const DzQuat &rot, const DzVec3 &scale ) const
{
	return getOrientedBox(pos, rot, scale);
}

bool MyNurbGeometry::hasMultipleLOD() const
{
	return false;
}

int MyNurbGeometry::getNumLODs() const
{
	return 0;
}

QString MyNurbGeometry::getLODName( int which ) const
{
	return "";
}

DzMap* MyNurbGeometry::getUVs() const
{
	return 0;
}

int MyNurbGeometry::getNumUVSets() const
{
	return 0;
}

DzUVSet* MyNurbGeometry::getUVSet( int which ) const
{
	return 0;
}

void MyNurbGeometry::tesselate(DzFacetMesh* mesh)
{
	mesh->beginEdit(false);
	for (int i = 0; i < m_data->dimensionU-1; i++)
	{
		for( int j = 0 ; j < m_data->dimensionV-1; j++)
		{
			int vertIdx[4];
			int uvwIdx[4];
			vertIdx[0] = ((i+0)*m_data->dimensionV+j+0);
			vertIdx[1] = ((i+0)*m_data->dimensionV+j+1);
			vertIdx[2] = ((i+1)*m_data->dimensionV+j+1);
			vertIdx[3] = ((i+1)*m_data->dimensionV+j+0);
			uvwIdx[0] = 0;
			uvwIdx[1] = 0;
			uvwIdx[2] = 0;
			uvwIdx[3] = 0;
			mesh->addFacet(vertIdx, uvwIdx);
		}
	}
	mesh->finishEdit();
}

MyNurbGeometryIO::MyNurbGeometryIO()
{
}

MyNurbGeometryIO::~MyNurbGeometryIO()
{
}

DzGeometry* MyNurbGeometryIO::createGeometry()const
{
	return new MyNurbGeometry();
}

DzShape* MyNurbGeometryIO::createShape(DzGeometry* geometry)const
{
	MyNurbShape* shape = new MyNurbShape(qobject_cast<MyNurbGeometry*>(geometry));
	return shape;
}

DzError MyNurbGeometryIO::writeExtraDefinition( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	MyNurbGeometry* geom = qobject_cast<MyNurbGeometry*>(object);

	io->addMember("dimension_u", geom->m_data->dimensionU);
	io->addMember("dimension_v", geom->m_data->dimensionV);
	io->startMemberArray("points");
	DzPnt3* vertices = geom->getVerticesPtr();

	for (int i = 0; i < geom->m_data->m_weights.size(); i++)
	{
		io->startArray(false);
		io->addItem(vertices[i][0]);
		io->addItem(vertices[i][1]);
		io->addItem(vertices[i][2]);
		io->addItem(geom->m_data->m_weights[i]);
		io->finishArray();
	}
	io->finishArray();

	io->startMemberArray("knot_vectors");
	for (int i = 0; i < geom->m_data->m_knotVectorU.size(); i++)
	{
		io->startArray(false);
		io->addItem(geom->m_data->m_knotVectorU[i]);
		io->addItem(geom->m_data->m_knotVectorV[i]);
		io->finishArray();
	}
	io->finishArray();

	return DZ_NO_ERROR;
}

DzError MyNurbGeometryIO::writeExtraInstance( QObject* object, IDzJsonIO* io, const DzFileIOSettings* opts )const
{
	return DZ_NO_ERROR;
}

DzAssetJsonObject* MyNurbGeometryIO::startDefinitionRead( DzAssetJsonItem* parentItem )
{
	m_context = new Context(parentItem->getFile());
	return new ReadMyNurbGeometry(m_context);
}

DzAssetJsonObject* MyNurbGeometryIO::startInstanceRead( DzAssetJsonItem* parentItem )
{
	m_context = new Context(parentItem->getFile());
	return new ReadMyNurbGeometry(m_context);
}

DzError MyNurbGeometryIO::applyDefinitionToObject( QObject* object, const DzFileIOSettings* opts )const
{
	MyNurbGeometry* geom = qobject_cast<MyNurbGeometry*>(object);

	geom->m_data->dimensionU = m_context->m_dimensionU;
	geom->m_data->dimensionV = m_context->m_dimensionV;

	if (m_context->m_points.size() != m_context->m_weights.size()
		|| geom->m_data->dimensionU*geom->m_data->dimensionV != m_context->m_points.size())
		return DZ_INDEX_OUT_OF_RANGE_ERROR;
	if (m_context->m_knotVectorUs.size() != m_context->m_knotVectorVs.size())
		return DZ_INDEX_OUT_OF_RANGE_ERROR;


	for (int i = 0; i < m_context->m_points.size(); i++)
	{
		geom->addVertex(
			m_context->m_points[i].m_x,
			m_context->m_points[i].m_y,
			m_context->m_points[i].m_z
			);
		geom->m_data->m_weights.push_back(m_context->m_weights[i]);
	}

	geom->m_data->m_knotVectorU = m_context->m_knotVectorUs;
	geom->m_data->m_knotVectorV = m_context->m_knotVectorUs;

	return DZ_NO_ERROR;
}

DzError MyNurbGeometryIO::applyInstanceToObject( QObject* object, const DzFileIOSettings* opts )const
{
	MyNurbShape* shape = qobject_cast<MyNurbShape*>(object);
	MyNurbGeometry* geom = qobject_cast<MyNurbGeometry*>(shape->getGeometry());
	return DZ_NO_ERROR;
}



bool ReadMyNurbGeometry::addMember( const QString &name, double val )
{
	if (name == "dimension_u") {
		m_context->m_dimensionU = val;
	}
	else if (name == "dimension_v") {
		m_context->m_dimensionV = val;
	}
	else {
		return DzAssetJsonObject::addMember(name, val);
	}
	return true;
}

DzAssetJsonItem* ReadMyNurbGeometry::startMemberArray( const QString &name )
{
	if (name == "points") {
		return new ReadMyNurbPoints(m_context);
	}
	else if (name == "knot_vectors") {
		return new ReadMyNurbKnots(m_context);
	}
	else {
		return DzAssetJsonObject::startMemberArray(name);
	}
}

DzAssetJsonItem* ReadMyNurbPoints::startArray()
{
	m_context->m_points.push_back(DzVec3(0, 0, 0));
	m_context->m_weights.push_back(0);
	return new ReadMyNurbPoint(m_context);
}

bool ReadMyNurbPoint::addItem(double val )
{
	int index = m_context->m_points.size() - 1;
	switch(m_i)
	{
	case 0:
	case 1:
	case 2:
		m_context->m_points[index][m_i++] = val; 
		return true;
	case 3:
		m_context->m_weights[index] = val; 
		return true;
	default:
		return DzAssetJsonArray::addItem(val);
	}
}


DzAssetJsonItem* ReadMyNurbKnots::startArray()
{
	m_context->m_knotVectorUs.push_back(0);
	m_context->m_knotVectorVs.push_back(0);
	return new ReadMyNurbKnot(m_context);
}

bool ReadMyNurbKnot::addItem(double val )
{
	int index = m_context->m_knotVectorUs.size() - 1;
	switch(m_i)
	{
	case 0: 
		m_context->m_knotVectorUs[index] = val; 
		return true;
	case 1: 
		m_context->m_knotVectorVs[index] = val; 
		return true;
	default:
		return DzAssetJsonArray::addItem(val);
	}
}
