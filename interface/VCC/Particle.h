#include <dzvec3.h>

class Particle
{
public:
	Particle();
	Particle(DzVec3 pos, DzVec3 vel, int num, float angle);
	~Particle();

	DzVec3 currPos;
	DzVec3 lBestPos;

	DzVec3 velocity;

	float bestAngle;
	int localBest;
};

