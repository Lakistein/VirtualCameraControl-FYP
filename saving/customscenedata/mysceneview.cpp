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

/*****************************
   Include files
*****************************/
#include <QtCore/QObject>
#include <QtGui/QLayout>

#include <dzscene.h>

#include "myscenemodel.h"
#include "mysceneview.h"

static const int c_minWidth = 200;
static const int c_minHeight = 150;


MySceneView::MySceneView(QWidget* parent)
	: QWidget(parent)
{
	QBoxLayout* layout = new QHBoxLayout();

	m_label0 = new QLabel(this);
	m_label0->setText("Cool Text:");
	layout->addWidget(m_label0);

	m_lineEdit0 = new QLineEdit(this);
	layout->addWidget(m_lineEdit0);
	
	setLayout(layout);

}

MySceneView::~MySceneView()
{
}

void MySceneView::setModel(MySceneModel* model)
{
	if (m_model)
	{
		disconnect (m_lineEdit0, SIGNAL(changed(const QString&)), model, SLOT(setCoolTest(const QString&)));
		disconnect (m_model, SIGNAL(coolTextChanged(const QString&)), m_lineEdit0, SLOT(setText(const QString&)));
	}

	m_model = model;

	if (m_model)
	{
		m_lineEdit0->setText(m_model->coolText());
		connect (m_lineEdit0, SIGNAL(textChanged(const QString&)), m_model, SLOT(setCoolText(const QString&)));
		connect (m_model, SIGNAL(coolTextChanged(const QString&)), m_lineEdit0, SLOT(setText(const QString&)));
	}

}



/**
**/
MyScenePane::MyScenePane() 
	: DzPane( "Scene Data (SDK Example)" ), m_view(0)
{
	QVBoxLayout* layout = new QVBoxLayout();
	m_view = new MySceneView(this);
	layout->addWidget(m_view);
	layout->addStretch();
	setLayout(layout);

	connect( dzScene, SIGNAL(sceneCleared()), this, SLOT(createModel()) );
	createModel();
}

/**
**/
MyScenePane::~MyScenePane()
{
}


void MyScenePane::createModel()
{
	// make the model
	MySceneModel* model = new MySceneModel();
	dzScene->addDataItem(model);

	// connect to the view
	m_view->setModel(model);
}
