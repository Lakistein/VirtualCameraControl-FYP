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
}

Particle::~Particle()
{
}