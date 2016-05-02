#include <dzvec3.h>

class Chromosome
{
public:
	Chromosome();
	Chromosome(DzVec3 pos, float fitness);
	~Chromosome();

	float Distance(DzVec3);
	DzVec3 currPos;
	DzVec3 lBestPos;

	float fitness;
	float bestAngle;
	float bestDistance;
	int localBest;
};

