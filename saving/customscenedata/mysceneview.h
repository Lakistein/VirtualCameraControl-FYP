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
#pragma once

/*****************************
   Include files
*****************************/
#include "myscenemodel.h"
#include <dzpane.h>
#include <dzaction.h>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>

/*****************************
  Forwards
*****************************/


/*****************************
   Class definitions
*****************************/
class MySceneView : public QWidget {
	Q_OBJECT
public:
    MySceneView(QWidget* parent);
    ~MySceneView();

	void setModel(MySceneModel* model);

private:
	QPointer<MySceneModel> m_model;
	QLabel*    m_label0;
	QLineEdit* m_lineEdit0;
};



/**
	This class is the menu action that will toggle the pane on/off in the interface
**/
class MyScenePaneAction : public DzPaneAction {
	Q_OBJECT
public:
	MyScenePaneAction() : DzPaneAction( "MyScenePane" ) { }
};

/**
	This class is the interface pane
**/
class MyScenePane : public DzPane {
	Q_OBJECT
public:
    MyScenePane();
    ~MyScenePane();

private slots:
	void createModel();

private:
	MySceneView* m_view;
};



