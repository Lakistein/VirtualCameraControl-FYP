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
	GeneticAlgorithm();
	GeneticAlgorithm(const DzVec3 bestPoint, const DzVec3 dirPOI, const DzVec3 p, const DzVec3 min, const DzVec3 max, const DzVec3 points[9], std::list<DzBox3> nodes);
	~GeneticAlgorithm();

	int _numOfChromo;
	DzVec3 _gBestPos, _point, _dirPOI, _min, _max, _bestPoint;
	float _gBestFitness, _fitness, _preferableDistance;
	std::list<DzBox3> _nodes;

	float Fitness(const DzVec3 origin, const DzVec3 p, const DzVec3 points[9], std::list<DzBox3> nodes);
	std::vector<Chromosome> _chromosomes;
	DzVec3 _points[9];
	DzVec3 GetBestPoint();
	Chromosome MakeChild(Chromosome father, Chromosome mother);
	DzVec3* GeneticAlgorithm::Search();
	int Breed(Chromosome f, Chromosome m, std::vector<Chromosome> *c, int num);
};
#endif
