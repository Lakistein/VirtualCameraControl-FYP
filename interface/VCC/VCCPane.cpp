#include "VCCPane.h"

#include <QtCore/QObject>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>

#include "dzapp.h"
#include "dzscene.h"
#include <dznodebtn.h>
#include "dzline3.h"
#include <dzbasiccamera.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include "ParticleSwarmOptimization.h"
#include <dzorientedbox3.h>
namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}
// TODO: popravi search space, i doradjuj algoritam pomalo
static const int c_minWidth = 200;
static const int c_minHeight = 150;

VCCPane::VCCPane() : DzPane("Virtual Camera Control")
{
	// Declarations
	int margin = style()->pixelMetric(DZ_PM_GeneralMargin);

	// Define the layout for the pane
	QVBoxLayout *mainLyt = new QVBoxLayout();
	mainLyt->setMargin(margin);
	mainLyt->setSpacing(margin);

	m_output = new QTextBrowser();
	m_output->setObjectName("SceneInfoTxtBrwsr");
	m_output->setMinimumSize(c_minWidth, c_minHeight);
	mainLyt->addWidget(m_output);

	dropbtn = new QComboBox(this);
	dropbtn->addItem("Close Face Closeup");
	dropbtn->addItem("Medium Face Closeup");
	dropbtn->addItem("Long Face Closeup");
	mainLyt->addWidget(dropbtn);

	QPushButton *btnGenerate = new QPushButton("Generate Camera", this, "Test Name");
	mainLyt->addWidget(btnGenerate);

	QPushButton *run = new QPushButton("Run step by step", this, "Test Name");
	mainLyt->addWidget(run);

	QPushButton *btnBest = new QPushButton("Get best point", this, "Test Name");
	mainLyt->addWidget(btnBest);
	// Set the layout for the pane
	//DzNodeBtn *b = new DzNodeBtn();
	//mainLyt->addWidget(b);

	setLayout(mainLyt);

	connect(btnGenerate, SIGNAL(pressed()), this, SLOT(GenerateCamera()));
	connect(run, SIGNAL(pressed()), this, SLOT(Run()));
	connect(btnBest, SIGNAL(pressed()), this, SLOT(Run2()));


	setMinimumSize(c_minWidth, c_minHeight);
}

VCCPane::~VCCPane()
{
}
//..\..\..\..\..\exec\$(Configuration)\$(Platform)\plugins\

void VCCPane::GetFourPoints(DzVec3 point, DzVec3 newPoints[4], DzVec3 allPoints[8])
{
	float distances[8];
	for (int i = 0; i < 8; i++)
		distances[i] = sqrt(pow(allPoints[i].m_x - point.m_x, 2) + pow(allPoints[i].m_y - point.m_y, 2) + pow(allPoints[i].m_z - point.m_z, 2));

	for (int write = 0; write < 8; write++) {
		for (int sort = 0; sort < 8 - 1; sort++) {
			if (distances[sort] > distances[sort + 1]) {
				float temp = distances[sort + 1];
				distances[sort + 1] = distances[sort];
				distances[sort] = temp;
				DzVec3 temp2 = allPoints[sort + 1];
				allPoints[sort + 1] = allPoints[sort];
				allPoints[sort] = temp2;
			}
		}
	}

	for (int j = 0; j < 4; j++)
		newPoints[j] = allPoints[j];
}
//void VCCPane::GetFourPoints(DzVec3 point, DzVec3 newPoints[4], DzVec3 allPoints[8])
//{
//	DzVec3 min;
//	int index[4];
//	index[0] = 0;
//	index[1] = index[2] = index[3] = -1;
//	int start = 0;
//	for (int j = 0; j < 4; j++)
//	{
//		int in = 0;
//		if (j != 0)
//			while (in < 4)
//			{
//				if (index[in] == start)
//				{
//					start++;
//					in = 0;
//					continue;
//				}
//				in++;
//			}
//		min = allPoints[start];
//		index[j] = start;
//		for (int i = 0; i < 8; i++)
//		{
//			if (index[0] == i || index[1] == i || index[2] == i || index[3] == i) continue;
//			if (sqrt(pow(min.m_x - point.m_x, 2) + pow(min.m_y - point.m_y, 2) + pow(min.m_z - point.m_z, 2)) >
//				sqrt(pow(allPoints[i].m_x - point.m_x, 2) + pow(allPoints[i].m_y - point.m_y, 2) + pow(allPoints[i].m_z - point.m_z, 2)))
//			{
//				min = allPoints[i];
//				index[j] = i;
//			}
//		}
//		newPoints[j] = min;
//	}
//}
float t = 0;
DzVec3 VCCPane::GeneratePerfectPoint(ShotType s)
{
	switch (s)
	{
	case VCCPane::CloseUp: // FIX
		DzNode *head = dzScene->findNode("head");
		DzQuat temp = head->getWSRot();
		head->setWSRot(DzQuat(0, 0, 0, 0));
		float w = abs(head->getLocalBoundingBox().getMax().m_x - head->getLocalBoundingBox().getMin().m_x);
		float h = abs(head->getLocalBoundingBox().getMax().m_y - head->getLocalBoundingBox().getMin().m_y);
		head->setWSRot(temp);
		DzNode *newNode = new DzNode();
		newNode->setWSTransform(head->getWSBoundingBox().getCenter(), head->getWSRot(), head->getWSScale());
		head->addNodeChild(newNode);
		t = (h + w) * 5;
		newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
		DzVec3 point = newNode->getWSPos();
		head->removeNodeChild(newNode);
		delete(newNode);
		return point;
	}
	return DzVec3();
}

std::list<DzBox3> VCCPane::GetImportantNodes(DzNodeListIterator * nodes)
{
	std::list<DzBox3> importantNodes;
	nodes->toFront();
	DzNode *node;
	while (nodes->hasNext())
	{
		node = nodes->next();
		if (node->getAssetId().contains("sphere") || node->getAssetId() == "Genesis3Female" || IsChildOf(node, "Genesis3Female")) continue;


		importantNodes.push_back(node->getWSBoundingBox());
	}
	return importantNodes;
}

bool VCCPane::IsChildOf(DzNode *n, QString name)
{
	DzNode *node = n;
	while (!node->isRootNode())
	{
		node = node->getNodeParent();
		if (node->getAssetId() == name) return true;
	}
	return false;
}

void VCCPane::Run()
{
	DzVec3 *v = p->Run2();
	DzNodeListIterator iter = dzScene->nodeListIterator();

	int index = 0;

	while (index < 100)
	{
		DzNode *node = iter.next();

		if (!node->getAssetId().contains("sphere"))  continue;

		node->setWSPos(v[index]);

		index++;
	}
	DzBasicCamera *cam = new DzBasicCamera();
	cam->setWSPos(p->GetBestPoint());
	cam->aimAt(p->_point);
	//dzScene->addNode(cam);
}

void VCCPane::Run2()
{
	DzVec3 *v = p->Run();
	DzNodeListIterator iter = dzScene->nodeListIterator();

	int index = 0;

	while (index < 100)
	{
		DzNode *node = iter.next();

		if (!node->getAssetId().contains("sphere"))  continue;

		node->setWSPos(v[index]);

		index++;
	}
}

void VCCPane::GenerateCamera()
{
	int index = dropbtn->currentIndex();
	switch (index)
	{
	case 0:
		break;
	case 1:
		break;
	default:
		break;
	}

	unsigned int start = clock();
	DzVec3 points[8];

	DzNode *n = dzScene->findNode("head");
	n->getWSOrientedBox().getPoints(points);

	DzBasicCamera *c = new DzBasicCamera();
	c->setName("nova kamera");
	c->setWSPos(GeneratePerfectPoint(ShotType::CloseUp));
	c->aimAt(n->getWSPos());
	//n->addNodeChild(c);


	DzVec3 newPoints[4];
	GetFourPoints(c->getWSPos(), newPoints, points);
	DzVec3 min = c->getWSBoundingBox().getMin();
	DzVec3 max = c->getWSBoundingBox().getMax() * 2;
	//min.m_x *= 10;
	//max.m_x *= 10;

	DzVec3 dir = DzVec3(c->getWSBoundingBox().getCenter() - n->getWSBoundingBox().getCenter()).normalized();


	DzBox3 box = DzBox3(c->getWSPos().m_x - 200, c->getWSPos().m_y - 200, c->getWSPos().m_z - 200, c->getWSPos().m_x + 200, c->getWSPos().m_y + 200, c->getWSPos().m_z + 200);

	p = new ParticleSwarmOptimization(DzVec3(c->getWSBoundingBox().getCenter() - n->getWSBoundingBox().getCenter()), n->getWSBoundingBox().getCenter(), box.getMin(), box.getMax(), newPoints, GetImportantNodes(&dzScene->nodeListIterator()));

	DzVec3 vec = p->GetBestPoint();
	c->setWSPos(vec);
	c->aimAt(n->getWSPos());
	//dzScene->addNode(c);
	unsigned int a = clock() - start;
	DzNodeListIterator iter = dzScene->nodeListIterator();

	int indexx = 0;

	while (indexx < 100)
	{
		DzNode *nodee = iter.next();

		if (!nodee->getAssetId().contains("sphere"))  continue;

		nodee->setWSPos(p->vecPoints[indexx]);

		indexx++;
	}
	m_output->append(QString::number(a));

	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_real<> x(min.m_x, max.m_x);
	//std::uniform_real<> y(min.m_y, max.m_y);
	//std::uniform_real<> z(min.m_z, max.m_z);

	//// Initialize particles
	//for (int i = 0; i < 100; i++)
	//{
	//	DzVec3 *vec = new DzVec3();
	//	vec->m_x = x(gen);
	//	vec->m_y = y(gen);
	//	vec->m_z = z(gen);

	//	DzBasicCamera *cc = new DzBasicCamera();
	//	cc->setName("nova kamera");
	//	cc->setWSPos(*vec);
	//	cc->aimAt(n->getWSPos());
	//	dzScene->addNode(cc);
	//}
}

DzNodeListIterator *RemoveItemFromList(DzNodeListIterator *lst, QList<QString> items)
{
	QList<DzNode*> *list = new QList<DzNode*>();
	while (lst->hasNext())
	{
		if (items.count(lst->next()->getAssetId()) == 0)
		{
			list->append(lst->peekPrevious());
		}
	}
	return new DzNodeListIterator(*list);
}