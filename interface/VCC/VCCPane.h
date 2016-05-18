#ifndef VCC_PANE_H
#define VCC_PANE_H

#include "dzpane.h"
#include "dzaction.h"
#include "dzvec3.h"
#include "dznode.h"
#include <list>
#include <dzdropbutton.h>
#include "ParticleSwarmOptimization.h"
#include "GeneticAlgorithm.h"
#include <dzbasiccamera.h>
#include <dznodebtn.h>
#include "CameraControl.h"

class QTextBrowser;
class DzNode;


class VCCPaneAction : public DzPaneAction {
	Q_OBJECT
public:
	VCCPaneAction() : DzPaneAction("VCCPane") { }
};

class VCCPane : public DzPane {
	Q_OBJECT
public:

	VCCPane();
	~VCCPane();

private:
	QTextBrowser *m_output;
	QComboBox *dropbtn, *algBtn;
	DzNodeBtn *nodeBtn;
	CameraControl *cc;

	private slots:
	void GenerateCamera();
};
#endif