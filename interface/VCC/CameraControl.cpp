#include "CameraControl.h"
#include "dzscene.h"


CameraControl::CameraControl()
{
	dzScene->addNode(new DzBasicCamera());
}

CameraControl::CameraControl(const DzVec3 pos)
{
	DzBasicCamera *c = new DzBasicCamera();
	SetPosition(c, pos);
	dzScene->addNode(c);
}


CameraControl::~CameraControl()
{
}

void CameraControl::SetPosition(DzBasicCamera *cam, const DzVec3 pos)
{
	cam->setWSPos(pos);
}

void CameraControl::SetRotation(DzBasicCamera *cam, const DzQuat rot)
{
	cam->setWSRot(rot);
}

void CameraControl::AlterPosition(DzBasicCamera *cam, float x, float y, float z)
{
	cam->setWSPos(DzVec3(cam->getWSPos().m_x + x, cam->getWSPos().m_y + y, cam->getWSPos().m_z + z));
}

void CameraControl::AlterRotation(DzBasicCamera *cam, float x, float y, float z)
{
	cam->setWSRot(DzQuat(cam->getWSRot().m_x + x, cam->getWSRot().m_y + y, cam->getWSRot().m_z + z, cam->getWSRot().m_w));

}

void CameraControl::SetDepthOfField()
{
}

void CameraControl::SetFocalDistance()
{
}

void CameraControl::SetFocalLength()
{
}

void CameraControl::LookAt(DzBasicCamera *cam, const DzVec3 target)
{
	cam->aimAt(target);
}

DzVec3 CameraControl::GetGlobalPositoin(const DzBasicCamera *cam)
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

bool CameraControl::IsInFOV(DzBox3)
{
	return false;
}

void CameraControl::Reset(DzBasicCamera)
{
}

void CameraControl::GetRegularFaceCloseUp(DzBasicCamera *cam, const DzNode node)
{

}
