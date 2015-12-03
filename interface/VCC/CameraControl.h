#include "dzvec3.h"
#include "dzbasiccamera.h"

class CameraControl
{
public:
	CameraControl();
	CameraControl(DzVec3);
	~CameraControl();
	void SetPosition(DzBasicCamera*, const DzVec3);
	void SetRotation(DzBasicCamera*, const DzQuat);
	void AlterPosition(DzBasicCamera*, float, float, float);
	void AlterRotation(DzBasicCamera*, float, float, float);
	void SetDepthOfField();
	void SetFocalDistance();
	void SetFocalLength();
	void LookAt(DzBasicCamera*, const DzVec3);
	DzVec3 GetGlobalPositoin(const DzBasicCamera *);

	int GetCameraCount();
	void DeleteCamera(const DzBasicCamera*);
	void CreateNewCamera(DzVec3, DzQuat);
	bool IsInFOV(const DzBox3);
	void Reset(DzBasicCamera);

	void GetRegularFaceCloseUp(DzBasicCamera *, const DzNode);
	void GetMediumFaceCloseUp(DzBasicCamera *, const DzNode);
	void GetExtremeFaceCloseUp(DzBasicCamera *, const DzNode);
	void GetRegularBodyShot(DzBasicCamera *, const DzNode);
	void GetLongBodyShot(DzBasicCamera *, const DzNode);
	void GetExtraLongBodyShot(DzBasicCamera *, const DzNode);
	void GetBirdsEyeView(DzBasicCamera *, const DzNode);
	void GetWormView(DzBasicCamera *, const DzNode);

};