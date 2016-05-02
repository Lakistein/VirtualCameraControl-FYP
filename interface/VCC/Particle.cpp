#include "Particle.h"
#include <cmath>

Particle::Particle()
{
}

Particle::Particle(DzVec3 pos, DzVec3 vec, float fit)
{
	currPos = pos;
	lBestPos = pos;
	fitness = fit;
	velocity = vec;
	bestDistance = 99999999;
}

Particle::~Particle()
{
}

float Particle::Distance(DzVec3 vec)
{
	return sqrt(pow(vec.m_x - currPos.m_x, 2) + pow(vec.m_y - currPos.m_y, 2) + pow(vec.m_z - currPos.m_z, 2));
}
