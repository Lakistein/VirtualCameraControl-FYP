#include "CameraControl.h"

CameraControl::CameraControl()
{
	cam = new DzBasicCamera();
	cam->setName("Camera");
}

CameraControl::CameraControl(const DzVec3 pos)
{
	cam = new DzBasicCamera();
	cam->setName("Camera");
	SetPosition(pos);
}

CameraControl::~CameraControl()
{
}

void CameraControl::SetPosition(const DzVec3 pos)
{
	cam->setWSPos(pos);
}

void CameraControl::SetRotation(const DzQuat rot)
{
	cam->setWSRot(rot);
}

void CameraControl::AlterPosition(const DzVec3 *vec)
{
	cam->setWSPos(cam->getWSPos() + *vec);
}

void CameraControl::AlterPosition(float x, float y, float z)
{
	cam->setWSPos(DzVec3(cam->getWSPos().m_x + x, cam->getWSPos().m_y + y, cam->getWSPos().m_z + z));
}

void CameraControl::AlterRotation(float x, float y, float z)
{
	cam->setWSRot(DzQuat(cam->getWSRot().m_x + x, cam->getWSRot().m_y + y, cam->getWSRot().m_z + z, cam->getWSRot().m_w));

}

void CameraControl::SetDepthOfField(bool useDOF)
{
	cam->setDepthOfField(useDOF);
}

void CameraControl::SetFocalDistance(float d)
{
	cam->setFocalDistance(d);
}

void CameraControl::SetFocalLength(float l)
{
	cam->setFocalLength(l);
}

void CameraControl::LookAt(const DzVec3 target)
{
	cam->aimAt(target);
}

DzVec3 CameraControl::GetGlobalPosition(const DzBasicCamera *cam)
{
	return cam->getWSPos();
}

int CameraControl::GetCameraCount()
{
	return 0;
}

void CameraControl::DeleteCamera(const DzBasicCamera *cam)
{

}

void CameraControl::CreateNewCamera(DzVec3, DzQuat)
{
}

void CameraControl::Reset(DzBasicCamera)
{
}

void CameraControl::GetShot(DzScene * scene, ShotType s, DzNode * node, Algorithm a)
{
	DzVec3 point = GeneratePerfectPoint(s, node, scene, a);
	scene->addNode(cam);
	al = a;
}

void CameraControl::GetImportantPoints(DzVec3 point, DzVec3 newPoints[9], DzVec3 allPoints[8])
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

std::list<DzBox3> CameraControl::GetImportantNodes(ShotType sht, DzNode & obj, DzNodeListIterator * nodes)
{
	std::list<DzBox3> importantNodes;
	std::list<std::string> importantNode2s2s;

	nodes->toFront();
	DzNode *node;

	while (nodes->hasNext())
	{
		node = nodes->next();

		if (node->getAssetId().contains("sphere", false) || node->getAssetId().contains("camera", false))
			continue;

		if (node->getAssetId() == obj.getAssetId() || IsChildOf(node, obj.getAssetId()))
			continue;

		importantNode2s2s.push_back(node->name());

		importantNodes.push_back(node->getWSBoundingBox());
	}
	return importantNodes;
}

bool IsChildOf(DzNode *n, QString name)
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

DzVec3 CameraControl::GeneratePerfectPoint(ShotType s, DzNode *node, DzScene * scene, Algorithm a)
{
	DzVec3 vec;
	DzVec3 points[8];
	DzVec3 newPoints[9];
	DzVec3 dir;
	DzVec3 camAim;
	DzVec3 boxMin, boxMax;

	DzNode *n;

	switch (s)
	{
	case CameraControl::ExtremeCloseUp: {
		n = node->findNodeChild("head", true);
		float dist = 0;
		camAim = DzVec3();
		vec = ExtremeCloseUpPoint(n, dist, camAim);
		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		dir = DzVec3(vec - camAim).normalized();
		DzBox3 box = n->getWSBoundingBox();
		box.setMax(box.getMax() + dir  * dist);
		box.setMin(box.getMin() + dir  * dist);
		boxMin = box.getMin();
		boxMax = box.getMax();
		break;
	}
	case CameraControl::FaceCloseUp: {
		n = node->findNodeChild("head", true);
		float dist = 0;
		vec = FaceCloseUpPoint(n, dist, camAim);
		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();
		DzBox3 box = DzBox3(vec.m_x - 20, vec.m_y - 10, vec.m_z - 10, vec.m_x + 20, vec.m_y + 10, vec.m_z + 20);
		boxMin = box.getMin();
		boxMax = box.getMax();
		camAim = n->getWSBoundingBox().getCenter();
		break;
	}
	case CameraControl::MidBody: {
		float dist = 0;
		camAim = DzVec3();
		n = node;
		vec = MidBodyPoint(n, dist, camAim);
		dir = DzVec3(vec - camAim).normalized();

		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		DzBox3 box = n->getWSBoundingBox();
		box.setMax(box.getMax() + dir  * dist);
		box.setMin(box.getMin() + dir  * dist);
		float max = box.getMax().m_y / 3;
		DzVec3 m = box.getMin();
		m.m_y += max;
		box.setMin(m);

		m = box.getMax();
		m.m_y -= max;
		box.setMax(m);

		boxMin = box.getMin();
		boxMax = box.getMax();
		break;
	}
	case CameraControl::FullBodyShot: {
		float dist = 0;
		camAim = DzVec3();
		n = node;
		vec = FullBodyShotPoint(n, dist, camAim);
		dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();
		camAim = n->getWSBoundingBox().getCenter();

		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		DzBox3 box = n->getWSBoundingBox();
		box.setMax(box.getMax() + dir  * dist);
		box.setMin(box.getMin() + dir  * dist);
		float max = box.getMax().m_y / 3;
		DzVec3 m = box.getMin();
		m.m_y += max;
		box.setMin(m);

		m = box.getMax();
		m.m_y -= max;
		box.setMax(m);

		boxMin = box.getMin();
		boxMax = box.getMax();
		break;
	}
	case CameraControl::HighAngle: {
		float dist = 0;
		camAim = DzVec3();
		n = node;
		vec = HighAngleViewPoint(n, dist, camAim);
		dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();
		camAim = n->getWSBoundingBox().getCenter();

		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		DzBox3 box = DzBox3();
		box.setMax(DzVec3(vec.m_x + dist / 3, vec.m_y, vec.m_z + dist / 3));
		box.setMin(DzVec3(vec.m_x - dist / 3, vec.m_y, vec.m_z - dist / 3));
		boxMin = box.getMin();
		boxMax = box.getMax();
		break;
	}
	case CameraControl::BirdsEyeView: {
		float dist = 0;
		camAim = DzVec3();
		n = node;
		vec = BirdsEyeViewPoint(n, dist, camAim);
		dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();
		camAim = n->getWSBoundingBox().getCenter();

		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		DzBox3 box = DzBox3();
		box.setMax(DzVec3(vec.m_x + dist / 3, vec.m_y, vec.m_z + dist / 3));
		box.setMin(DzVec3(vec.m_x - dist / 3, vec.m_y, vec.m_z - dist / 3));
		boxMin = box.getMin();
		boxMax = box.getMax();
		break;
	}
	case CameraControl::WormsEyeView: {
		float dist = 0;
		camAim = DzVec3();
		n = node;
		vec = WormsEyeViewPoint(n, dist, camAim);
		dir = DzVec3(vec - n->getWSBoundingBox().getCenter()).normalized();
		camAim = (node->getWSPos() + n->getWSBoundingBox().getCenter()) / 1.2;


		n->getWSOrientedBox().getPoints(points);
		GetImportantPoints(vec, newPoints, points);
		DzBox3 box = DzBox3();
		box.setMax(DzVec3(vec.m_x + dist / 3, 5, vec.m_z + dist / 3));
		box.setMin(DzVec3(vec.m_x - dist / 3, 5, vec.m_z));
		boxMin = box.getMin();
		boxMax = box.getMax();
		break;
	}
	default: break;
	}

	DzVec3 bestPoint;
	switch (a)
	{
	case CameraControl::PSO:
		p = new ParticleSwarmOptimization(vec, dir, camAim, boxMin, boxMax, newPoints, GetImportantNodes(s, *node, &scene->nodeListIterator()));
		bestPoint = p->GetBestPoint();
		if (p->_fitness >= 9)
			bestPoint = vec;
		break;
	case CameraControl::GA:
		g = GeneticAlgorithm(vec, dir, camAim, boxMin, boxMax, newPoints, GetImportantNodes(s, *node, &scene->nodeListIterator()));
		bestPoint = g.GetBestPoint();
		if (g._gBestFitness >= 9)
			bestPoint = vec;
		break;
	default:
		break;
	}

	cam->setWSPos(bestPoint);
	cam->aimAt(camAim);
	SetFocalLength(28);
	return bestPoint;
}

DzVec3 CameraControl::ExtremeCloseUpPoint(DzNode *head, float &dist, DzVec3 & camAimAt) {
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
	float t = h / 2;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	//dzScene->findNodeByLabel("pp")->setWSTransform(newNode->getWSPos(), newNode->getWSRot(), newNode->getLocalScale());

	dist = t;
	DzVec3 point = newNode->getWSPos();
	head->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 CameraControl::FaceCloseUpPoint(DzNode * head, float &dist, DzVec3 & camAimAt) {
	DzQuat temp = head->getWSRot();
	head->setWSRot(DzQuat(0, 0, 0, 0));
	float w = abs(head->getLocalBoundingBox().getMax().m_x - head->getLocalBoundingBox().getMin().m_x);
	float h = abs(head->getLocalBoundingBox().getMax().m_y - head->getLocalBoundingBox().getMin().m_y);
	head->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(head->getWSBoundingBox().getCenter(), head->getWSRot(), head->getWSScale());
	head->addNodeChild(newNode);
	float t = h * 2.5;
	dist = t;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	head->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 CameraControl::MidBodyPoint(DzNode *node, float &dist, DzVec3 & camAimAt)
{
	DzQuat temp = node->getWSRot();
	node->setWSRot(DzQuat(0, 0, 0, 0));
	camAimAt = node->getWSBoundingBox().getCenter();
	float h = abs(node->getLocalBoundingBox().getMax().m_y - node->getLocalBoundingBox().getMin().m_y);
	node->setWSRot(temp);
	DzNode *newNode = new DzNode();
	camAimAt = (node->getWSBoundingBox().getCenter() + node->findNodeChild("head", true)->getWSBoundingBox().getCenter()) / 1.85;
	newNode->setWSTransform(camAimAt, node->getWSRot(), node->getWSScale());
	node->addNodeChild(newNode, true);
	float t = h / 2.5;
	dist = t;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	node->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 CameraControl::FullBodyShotPoint(DzNode * node, float &dist, DzVec3 & camAimAt)
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
	float t = h * 2;
	dist = t;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	node->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 CameraControl::HighAngleViewPoint(DzNode * node, float & dist, DzVec3 & camAimAt)
{
	DzQuat temp = node->getWSRot();
	node->setWSRot(DzQuat(0, 0, 0, 0));
	camAimAt = node->getWSBoundingBox().getCenter();
	float h = abs(node->getLocalBoundingBox().getMax().m_y - node->getLocalBoundingBox().getMin().m_y);
	node->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(node->getWSBoundingBox().getCenter(), node->getWSRot(), node->getWSScale());
	node->addNodeChild(newNode, true);
	float t = h * 1.5;
	dist = t;
	newNode->setLocalPos(DzVec3(newNode->getLocalPos().m_x, newNode->getLocalPos().m_y + t, newNode->getLocalPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	node->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 CameraControl::BirdsEyeViewPoint(DzNode * node, float &dist, DzVec3 & camAimAt)
{
	DzQuat temp = node->getWSRot();
	node->setWSRot(DzQuat(0, 0, 0, 0));
	camAimAt = node->getWSBoundingBox().getCenter();
	float h = abs(node->getLocalBoundingBox().getMax().m_y - node->getLocalBoundingBox().getMin().m_y);
	node->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(node->getWSBoundingBox().getCenter(), node->getWSRot(), node->getWSScale());
	node->addNodeChild(newNode, true);
	float t = h * 3;
	dist = t;
	newNode->setWSPos(DzVec3(newNode->getWSPos().m_x, newNode->getWSPos().m_y + t, newNode->getWSPos().m_z));
	DzVec3 point = newNode->getWSPos();
	node->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

DzVec3 CameraControl::WormsEyeViewPoint(DzNode * node, float & dist, DzVec3 & camAimAt)
{
	DzQuat temp = node->getWSRot();
	node->setWSRot(DzQuat(0, 0, 0, 0));
	camAimAt = node->getWSBoundingBox().getCenter();
	float h = abs(node->getLocalBoundingBox().getMax().m_y - node->getLocalBoundingBox().getMin().m_y);
	node->setWSRot(temp);
	DzNode *newNode = new DzNode();
	newNode->setWSTransform(node->getWSBoundingBox().getCenter(), node->getWSRot(), node->getWSScale());
	node->addNodeChild(newNode, true);
	float t = h / 1.5;
	dist = t;
	newNode->setWSPos(DzVec3(newNode->getWSPos().m_x, 5, newNode->getWSPos().m_z + t));
	DzVec3 point = newNode->getWSPos();
	node->removeNodeChild(newNode);
	delete(newNode);
	return point;
}

bool CameraControl::IsChildOf(DzNode * n, QString name)
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

DzVec3 * CameraControl::GetPoints()
{
	switch (al)
	{
	case CameraControl::PSO:
		return p->vecPoints;
	case CameraControl::GA:
		return g.vecPoints;
	default:
		break;
	}
}