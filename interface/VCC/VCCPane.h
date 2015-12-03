#ifndef VCC_PANE_H
#define VCC_PANE_H

#include "dzpane.h"
#include "dzaction.h"
#include "dzvec3.h"
#include "dznode.h"
#include <list>
#include <dzdropbutton.h>
#include "ParticleSwarmOptimization.h"

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

private:
	QTextBrowser	*m_output;
	enum ShotType { CloseUp };
	QComboBox *dropbtn;
	void GetFourPoints(DzVec3, DzVec3[4], DzVec3[8]);
	DzVec3 GeneratePerfectPoint(ShotType);
	std::list<DzBox3>  GetImportantNodes(DzNodeListIterator *nodes);
	bool IsChildOf(DzNode *node, QString name);

private slots:
	void GenerateCamera();
	void Run();
	void Run2();
};

#endif


