#include "Particle.h"

Particle::Particle()
{
}

Particle::Particle(DzVec3 pos, DzVec3 vec, int num, float angle)
{
	currPos = pos;
	lBestPos = pos;
	localBest = num;
	bestAngle = angle;
	velocity = vec;
}

Particle::~Particle()
{
}