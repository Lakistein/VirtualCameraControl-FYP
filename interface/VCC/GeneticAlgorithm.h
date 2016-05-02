#ifndef VCC_GeneticAlgorithm_H
#define VCC_GeneticAlgorithm_H

#include "dzvec3.h"
#include "Chromosome.h"
#include <random>

#include <QtCore/QList>
#include <QtCore/QPointer>
#include "dztsharedpointer.h"
#include "dzscene.h"
#include <dznode.h>
#include "dzline3.h"
class GeneticAlgorithm
{
public:
	int _gBest, _numOfChromo;
	DzVec3 _gBestPos, _point, _dirPOI, _min, _max, _bestPoint;
	float _gBestFitness, _fitness, _gBestAngle, _preferableAngle, _maxAngle, _preferableDistance, _maxDistance, _minDistance, _gBestDistance, _preferedDistance;
	double c0, c1, c2, r1, r2;
	std::list<DzBox3> _nodes;
	GeneticAlgorithm(const DzVec3 bestPoint, const DzVec3 dirPOI, const DzVec3 p, const DzVec3 min, const DzVec3 max, const DzVec3 points[9], std::list<DzBox3> nodes);
	~GeneticAlgorithm();
	float Fitness(const DzVec3 origin, const DzVec3 p, const DzVec3 points[9], std::list<DzBox3> nodes);
	std::vector<Chromosome> _chromosomes;
	DzVec3 _points[9];
	DzVec3 GetBestPoint();
	DzVec3 vecPoints[100];
	Chromosome MakeChild(Chromosome father, Chromosome mother);
	DzVec3* GeneticAlgorithm::Run();
	int Breed(Chromosome f, Chromosome m, std::vector<Chromosome> *c, DzVec3 * vecPoints, int num);
	DzVec3* GeneticAlgorithm::Run2();
	//DzVec3 *GetDirectionVector(const DzVec3 *a, const DzVec3 *b);
	//float GetAngle(const DzVec3 *A, const DzVec3 *B);
	//void SetNumberOfChromosomes(int num);
	//void SetDirectionVectorOfPOI(const DzVec3 *vec);
	//void SetNumberOfPoints(int num);
	//void SetCenterPoint(const DzVec3 *point);
	//DzVec3* Run2();
};
#endif
