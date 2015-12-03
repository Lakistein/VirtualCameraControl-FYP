#ifndef VCC_ParticleSwarmOptimization_H
#define VCC_ParticleSwarmOptimization_H

#include "dzvec3.h"
#include "Particle.h"
#include <random>

#include <QtCore/QList>
#include <QtCore/QPointer>
#include "dztsharedpointer.h"
#include "dzscene.h"
#include <dznode.h>
#include "dzline3.h"
#include <list>
class ParticleSwarmOptimization
{
public:
	int _gBest, _numOfParticles;
	DzVec3 _gBestPos, _point, _dirPOI, _min, _max;
	float _gBestAngle, _preferableAngle, _maxAngle, _preferableDistance, _maxDistance, _minDistance;
	double c0, c1, c2, r1, r2;
	std::list<DzBox3> _nodes;
	ParticleSwarmOptimization(const DzVec3 dirPOI, const DzVec3 p, const DzVec3 min, const DzVec3 max, const DzVec3 points[4], std::list<DzBox3> nodes);
	~ParticleSwarmOptimization();
	int ParticleSwarmOptimization::Error(const DzVec3 * origin, const DzVec3 * points, std::list<DzBox3> nodes);
	Particle _particles[100];
	DzVec3 _points[4];
	DzVec3 GetBestPoint();
	DzVec3 vecPoints[100];

	DzVec3* Run();

	DzVec3 *GetDirectionVector(const DzVec3 *a, const DzVec3 *b);
	float GetAngle(const DzVec3 *A, const DzVec3 *B);
	void SetNumberOfParticles(int num);
	void SetDirectionVectorOfPOI(const DzVec3 *vec);
	void SetNumberOfPoints(int num);
	void SetCenterPoint(const DzVec3 *point);
	DzVec3* Run2();
private:
	//int numOfPoints;
};
#endif