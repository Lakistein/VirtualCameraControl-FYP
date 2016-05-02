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
#include "GeneticAlgorithm.h"
#include <dzorientedbox3.h>
#include <dznodebtn.h>

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

	nodeBtn = new DzNodeBtn(this);

	m_output = new QTextBrowser();
	m_output->setObjectName("SceneInfoTxtBrwsr");
	m_output->setMinimumSize(c_minWidth, c_minHeight);
	mainLyt->addWidget(m_output);


	dropbtn = new QComboBox(this);
	dropbtn->addItem("Eye Level");
	dropbtn->addItem("Face Close-Up");
	dropbtn->addItem("Mid Body");
	dropbtn->addItem("Long Range");
	dropbtn->addItem("Bird Eye");
	dropbtn->addItem("Worm Eye");
	dropbtn->addItem("High Angle");
	mainLyt->addWidget(dropbtn);

	mainLyt->addWidget(nodeBtn);

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

void VCCPane::GetFourPoints(DzVec3 point, DzVec3 newPoints[9], DzVec3 allPoints[8])
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
	for (int j = 0; j < 4; j++) {
		newPoints[j] = allPoints[j];
	}

	newPoints[4] = (newPoints[0] + newPoints[1]) / 2;
	newPoints[5] = (newPoints[0] + newPoints[2]) / 2;
	newPoints[6] = (newPoints[0] + newPoints[3]) / 2;
	newPoints[7] = (newPoints[1] + newPoints[3]) / 2;
	newPoints[8] = (newPoints[2] + newPoints[3]) / 2;
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
DzVec3 VCCPane::GeneratePerfectPoint(ShotType s, DzBasicCamera *c)
{
	DzVec3 vec;
	DzVec3 points[8];
	DzVec3 newPoints[9];

	switch (s)
	{
	case VCCPane::CloseUp: {
		DzNode *n = nodeBtn->getNode()->findNodeChild("head", true);
		DzVec3 vec = FaceCloseUp(n);
		DzVec3 pnt = n->getWSPos();
		n->getWSOrientedBox().getPoints(points);
		GetFourPoints(vec, newPoints, points);
		/*	DzVec3 min = c->getWSBoundingBox().getMin();
			DzVec3 max = c->getWSBoundingBox().getMax() * 2;*/
			//min.m_x *= 10;
			//max.m_x *= 10;

			// popravi bug
		DzVec3 dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();

		float angle = dir.getAngleTo(dir);
		DzBox3 box = DzBox3(vec.m_x - 20, vec.m_y - 10, vec.m_z - 10, vec.m_x + 20, vec.m_y + 10, vec.m_z + 20);

		p = new ParticleSwarmOptimization(vec, dir, n->getWSBoundingBox().getCenter(), box.getMin(), box.getMax(), newPoints, GetImportantNodes(s, *n, &dzScene->nodeListIterator()));
		return p->GetBestPoint();
		break;
	}
	case VCCPane::LongRange: {
		float dist = 0;
		DzVec3 camAim = DzVec3();
		vec = LongRangePoint(nodeBtn->getNode(), dist, camAim);
		DzNode *n = nodeBtn->getNode();
		DzVec3 pnt = n->getWSPos();

		n->getWSOrientedBox().getPoints(points);
		GetFourPoints(vec, newPoints, points);
		/*	DzVec3 min = c->getWSBoundingBox().getMin();
		DzVec3 max = c->getWSBoundingBox().getMax() * 2;*/
		//min.m_x *= 10;
		//max.m_x *= 10;

		DzVec3 dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();

		float angle = dir.getAngleTo(dir);
		DzBox3 box = n->getWSBoundingBox();
		box.setMax(box.getMax() + dir  * dist);
		box.setMin(box.getMin() + dir  * dist);
		//DzBox3 box = DzBox3(vec.m_x - 100, vec.m_y - 100, vec.m_z - 100, vec.m_x + 100, vec.m_y + 100, vec.m_z + 100);

		p = new ParticleSwarmOptimization(vec, dir, n->getWSBoundingBox().getCenter(), box.getMin(), box.getMax(), newPoints, GetImportantNodes(s, *n, &dzScene->nodeListIterator()));
		//g = new GeneticAlgorithm(vec, dir, n->getWSBoundingBox().getCenter(), box.getMin(), box.getMax(), newPoints, GetImportantNodes(s, *n, &dzScene->nodeListIterator()));
		c->setWSPos(p->GetBestPoint());
		//if (p->_gBest == 8)
		//	c->setWSPos(vec);
		c->aimAt(camAim);
		return p->GetBestPoint();
		break;
	}
	case VCCPane::EyeLevel: {
		DzNode *n = nodeBtn->getNode()->findNodeChild("head", true);
		float dist = 0;
		DzVec3 camAim = DzVec3();
		vec = EyeLevelPoint(n, dist, camAim);
		DzVec3 pnt = n->getWSPos();
		n->getWSOrientedBox().getPoints(points);
		GetFourPoints(vec, newPoints, points);
		DzVec3 dir = DzVec3(vec - camAim).normalized();
		DzBox3 box = n->getWSBoundingBox();
		box.setMax(box.getMax() + dir  * dist);
		box.setMin(box.getMin() + dir  * dist);

		float angle = dir.getAngleTo(dir);

		//DzBox3 box = DzBox3(b.getMin().m_x + vec.m_x + dir.m_x, b.getMin().m_y + vec.m_y + dir.m_y, b.getMin().m_z + vec.m_z + dir.m_z, b.getMax().m_x + vec.m_x + dir.m_x, b.getMax().m_y + vec.m_y + dir.m_y, b.getMax().m_z + vec.m_z + dir.m_z);

		p = new ParticleSwarmOptimization(vec, dir, camAim, box.getMin(), box.getMax(), newPoints, GetImportantNodes(s, *n, &dzScene->nodeListIterator()));
		c->setWSPos(p->GetBestPoint());
		c->setWSRot(n->getWSRot());
		if (p->_gBest == 4)
			c->setWSPos(vec);
		c->aimAt(camAim);
		return p->GetBestPoint();
		break;
	}
	case VCCPane::MidBody: {
		float dist = 0;
		DzVec3 camAim = DzVec3();
		vec = LongRangePoint(nodeBtn->getNode(), dist, camAim);
		DzNode *n = nodeBtn->getNode();
		DzVec3 pnt = n->getWSPos();

		n->getWSOrientedBox().getPoints(points);
		GetFourPoints(vec, newPoints, points);
		/*	DzVec3 min = c->getWSBoundingBox().getMin();
		DzVec3 max = c->getWSBoundingBox().getMax() * 2;*/
		//min.m_x *= 10;
		//max.m_x *= 10;

		DzVec3 dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();

		float angle = dir.getAngleTo(dir);
		DzBox3 box = n->getWSBoundingBox();
		box.setMax(box.getMax() + dir  * dist);
		box.setMin(box.getMin() + dir  * dist);
		//DzBox3 box = DzBox3(vec.m_x - 100, vec.m_y - 100, vec.m_z - 100, vec.m_x + 100, vec.m_y + 100, vec.m_z + 100);

		p = new ParticleSwarmOptimization(vec, dir, n->getWSBoundingBox().getCenter(), box.getMin(), box.getMax(), newPoints, GetImportantNodes(s, *n, &dzScene->nodeListIterator()));

		c->setWSPos(p->GetBestPoint());
		c->aimAt(camAim);
		return p->GetBestPoint();
		break;
	}
	default: break;
	}
	return vec;
}

// pokusaj da popravis kad je glava okrenuta
DzVec3 VCCPane::EyeLevelPoint(DzNode *head, float &dist, DzVec3 & camAimAt) {
	DzNode *lEye = head->findNodeChild("lEye", true);
	DzNode *rEye = head->findNodeChild("rEye", true);
	DzQuat temp = head->getWSRot();
	DzVec3 p = (lEye->getWSPos() + rEye->getWSPos()) / 2;
	camAimAt = p;

	head->setWSRot(DzQuat(0, 0, 0, 0));
	float w = abs(head->getLocalBoundingBox().getMax().m_x - head->getLocalBoundingBox().getMin().m_x);
	float h = abs(head->getLocalBoundingBox().getMax().m_y - head->getLocalBoundingBox().getMin().m_y);
	head->setWSRot(temp);
	DzNode *newNode = new DzNode();
	DzQuat r = head->getLocalRot();
	DzQuat rr = head->getWSRot();
	newNode->setWSTransform(p, head->getWSRot(), head->getWSScale());

	head->addNodeChild(newNode, true);
	float t = (h + w) / 2;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	//dzScene->findNodeByLabel("pp")->setWSTransform(newNode->getWSPos(), newNode->getWSRot(), newNode->getLocalScale());

	dist = t;
	DzVec3 point = newNode->getWSPos();
	head->removeNodeChild(newNode);
	delete(newNode);
	return point;
}
DzVec3 VCCPane::FaceCloseUp(DzNode *head) {
	DzQuat temp = head->getWSRot();
	head->setWSRot(DzQuat(0, 0, 0, 0));
	float w = abs(head->getLocalBoundingBox().getMax().m_x - head->getLocalBoundingBox().getMin().m_x);
	float h = abs(head->getLocalBoundingBox().getMax().m_y - head->getLocalBoundingBox().getMin().m_y);
	head->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(head->getWSBoundingBox().getCenter(), head->getWSRot(), head->getWSScale());
	head->addNodeChild(newNode);
	float t = (h + w);
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	head->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 VCCPane::MidBodyPoint(DzNode *body, float &dist, DzVec3 & camAimAt) {
	DzQuat temp = body->getWSRot();
	body->setWSRot(DzQuat(0, 0, 0, 0));
	camAimAt = body->getWSBoundingBox().getCenter();
	float w = abs(body->getLocalBoundingBox().getMax().m_x - body->getLocalBoundingBox().getMin().m_x);
	float h = abs(body->getLocalBoundingBox().getMax().m_y - body->getLocalBoundingBox().getMin().m_y);
	body->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(body->getWSBoundingBox().getCenter(), body->getWSRot(), body->getWSScale());
	body->addNodeChild(newNode);
	float t = (h + w) * 1.1;
	dist = t;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	body->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 VCCPane::LongRangePoint(DzNode * node, float &dist, DzVec3 & camAimAt)
{
	DzQuat temp = node->getWSRot();
	node->setWSRot(DzQuat(0, 0, 0, 0));
	camAimAt = node->getWSBoundingBox().getCenter();
	float w = abs(node->getLocalBoundingBox().getMax().m_x - node->getLocalBoundingBox().getMin().m_x);
	float h = abs(node->getLocalBoundingBox().getMax().m_y - node->getLocalBoundingBox().getMin().m_y);
	node->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(node->getWSBoundingBox().getCenter(), node->getWSRot(), node->getWSScale());
	node->addNodeChild(newNode, true);
	float t = (h + w) * 1.5;
	dist = t;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	node->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

std::list<DzBox3> VCCPane::GetImportantNodes(VCCPane::ShotType sht, DzNode &obj, DzNodeListIterator * nodes)
{
	std::list<DzBox3> importantNodes;
	std::list<QString> importantNode22s;
	std::list<std::string> importantNode2s2s;

	nodes->toFront();
	DzNode *node;
	DzNode *root = &obj;
	while (root->getNodeParent() != NULL) {
		root = root->getNodeParent();
	}
	int t = 0;
	while (nodes->hasNext())
	{
		node = nodes->next();

		t++;
		if (node->getAssetId() == root->getAssetId() || node->getAssetId() == obj.getAssetId())
			continue;

		switch (sht)
		{
		case VCCPane::EyeLevel:
			if (IsChildOf(node, obj.getAssetId()))
				continue;
			if (!node->getAssetId().contains("arm", false) || !node->getAssetId().contains("hand", false))
				continue;
			break;
		case VCCPane::CloseUp:
			break;
		case VCCPane::MidBody:
			break;
		case VCCPane::LongRange:
			if (IsChildOf(node, obj.getAssetId()))
				continue;
			break;
		case VCCPane::BirdEye:
			break;
		case VCCPane::WormEye:
			break;
		case VCCPane::HighAngle:
			break;
		default:
			break;
		}
		if (node->getAssetId().contains("sphere") || node->getAssetId().contains("camera"))
			continue;
		importantNode22s.push_back(node->getAssetId());
		importantNode2s2s.push_back(node->name());

		importantNodes.push_back(node->getWSBoundingBox());

	}
	return importantNodes;
}
// kad se vratis radi ovo obavezno!!! zavrsi POVs i pocni da radis genetic algorithm
bool VCCPane::IsChildOf(DzNode *n, QString name)
{
	DzNode *node = n;

	while (!node->isRootNode())
	{
		node = node->getNodeParent();
		if (node->getAssetId().contains(name))
			return true;
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

		if (!node->getAssetId().contains("sphere"))
			continue;

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
	int index = dropbtn->currentIndex();

	DzBasicCamera *c = new DzBasicCamera();
	c->setName("camera");

	DzNodeListIterator iter = dzScene->nodeListIterator();

	int indexx = 0;

	while (indexx < 100)
	{
		DzNode *nodee = iter.next();

		if (!nodee->getAssetId().contains("sphere"))  continue;

		nodee->setWSPos(p->vecPoints[indexx]);

		indexx++;
	}
	c->setWSPos(p->GetBestPoint());
	c->aimAt(p->_point);
	dzScene->addNode(c);
}

void VCCPane::GenerateCamera()
{
	int index = dropbtn->currentIndex();

	DzBasicCamera *c = new DzBasicCamera();
	c->setName("camera");
	unsigned int start = clock();

	DzVec3 vec = GeneratePerfectPoint((ShotType)index, c);


	//c->setWSPos(vec);
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

	dzScene->addNode(c);
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