#include "dzvec3.h"
#include "dznode.h"
#include "dzscene.h"
#include "dzbasiccamera.h"
#include "ParticleSwarmOptimization.h"
#include "GeneticAlgorithm.h"
#include <list>

class CameraControl
{
public:
	CameraControl();
	CameraControl(QString text);
	CameraControl(DzVec3);
	~CameraControl();
	
	DzBasicCamera *cam;

	enum ShotType { ExtremeCloseUp = 0, FaceCloseUp = 1, MidBody = 2, FullBodyShot = 3, BirdsEyeView = 4, WormsEyeView = 5, HighAngle = 6 };
	enum Algorithm {PSO = 0, GA = 1};

	void SetPosition(const DzVec3);
	void SetRotation(const DzQuat);
	void AlterPosition(const DzVec3 *vec);
	void AlterPosition(float, float, float);
	void AlterRotation(float, float, float);
	void SetDepthOfField(bool useDOF);
	void SetFocalDistance(float d);
	void SetFocalLength(float l);
	void LookAt(const DzVec3);
	DzVec3 GetGlobalPosition(const DzBasicCamera *);

	int GetCameraCount();
	void DeleteCamera(const DzBasicCamera*);
	void CreateNewCamera(DzVec3, DzQuat);
	void Reset(DzBasicCamera);

	void GetShot(DzScene *scene, ShotType s, DzNode * node, QString text, Algorithm a = PSO);
	ParticleSwarmOptimization *p;
	Algorithm al;
private:
	void GetImportantPoints(DzVec3, DzVec3[9], DzVec3[8]);
	bool IsChildOf(DzNode *n, QString name);
	GeneticAlgorithm g;
	DzVec3 CameraControl::GeneratePerfectPoint(ShotType s, DzNode *node, DzScene * scene, Algorithm a);
	DzVec3 CameraControl::ExtremeCloseUpPoint(DzNode *head, float &dist, DzVec3 & camAimAt);
	DzVec3 CameraControl::FaceCloseUpPoint(DzNode * head, float &dist, DzVec3 & camAimAt);
	DzVec3 CameraControl::MidBodyPoint(DzNode *body, float &dist, DzVec3 & camAimAt);
	DzVec3 CameraControl::FullBodyShotPoint(DzNode * node, float &dist, DzVec3 & camAimAt);
	DzVec3 CameraControl::HighAngleViewPoint(DzNode * node, float &dist, DzVec3 & camAimAt);
	DzVec3 CameraControl::BirdsEyeViewPoint(DzNode * node, float &dist, DzVec3 & camAimAt);
	DzVec3 CameraControl::WormsEyeViewPoint(DzNode * node, float &dist, DzVec3 & camAimAt);

	std::list<DzBox3> GetImportantNodes(ShotType sht, DzNode &obj, DzNodeListIterator * nodes);
};