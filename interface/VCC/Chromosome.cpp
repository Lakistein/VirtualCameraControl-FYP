#include "Chromosome.h"
#include <cmath>

Chromosome::Chromosome()
{
	fitness = 1000;
}

Chromosome::Chromosome(DzVec3 pos, float fit)
{
    fitness = fit;
	currPos = pos;
}

Chromosome::~Chromosome()
{
}