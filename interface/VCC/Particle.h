#include <dzvec3.h>

class Particle
{
public:
	Particle();
	Particle(DzVec3 pos, DzVec3 vel, float fit);
	~Particle();

	float Distance(DzVec3);
	DzVec3 currPos;
	DzVec3 lBestPos;

	DzVec3 velocity;

	float bestAngle;
	float bestDistance;
	int fitness;
};

