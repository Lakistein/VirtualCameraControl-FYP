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

float Chromosome::Distance(DzVec3 vec)
{
	return sqrt(pow(vec.m_x - currPos.m_x, 2) + pow(vec.m_y - currPos.m_y, 2) + pow(vec.m_z - currPos.m_z, 2));
}