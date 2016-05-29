#include <dzvec3.h>

class Chromosome
{
public:
	Chromosome();
	Chromosome(DzVec3 pos, float fitness);
	~Chromosome();

	DzVec3 currPos;

	float fitness;
};

