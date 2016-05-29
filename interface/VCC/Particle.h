#include <dzvec3.h>

class Particle
{
public:
	Particle();
	Particle(DzVec3 pos, DzVec3 vel, float fit);
	~Particle();

	DzVec3 currPos;
	DzVec3 lBestPos;

	DzVec3 velocity;

	int fitness;
};

