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
	ParticleSwarmOptimization *p;
	GeneticAlgorithm *g;

private:
	QTextBrowser *m_output;
	enum ShotType { EyeLevel = 0, CloseUp = 1, MidBody = 2, LongRange = 3, BirdEye = 4, WormEye = 5, HighAngle = 6 };
	QComboBox *dropbtn;
	DzNodeBtn *nodeBtn;
	void GetFourPoints(DzVec3, DzVec3[9], DzVec3[8]);
	DzVec3 GeneratePerfectPoint(ShotType, DzBasicCamera*);
	DzVec3 EyeLevelPoint(DzNode *head, float &dist, DzVec3 & camAimAt);
	std::list<DzBox3>  GetImportantNodes(VCCPane::ShotType sht, DzNode &obj, DzNodeListIterator *nodes);
	bool IsChildOf(DzNode *node, QString name);
	DzVec3 VCCPane::FaceCloseUp(DzNode *head);
	DzVec3 MidBodyPoint(DzNode *body, float &dist, DzVec3 & camAimAt);
	DzVec3 VCCPane::LongRangePoint(DzNode* node, float &dist, DzVec3 & camAimAt);

	private slots:
	void GenerateCamera();
	void Run();
	void Run2();
};

#endif


