#include "GeneticAlgorithm.h"
#include <list>
#include <vector>

GeneticAlgorithm::GeneticAlgorithm()
{

}

GeneticAlgorithm::GeneticAlgorithm(const DzVec3 bestPoint, const DzVec3 dirPOI, const DzVec3 p, const DzVec3 min, const DzVec3 max, const DzVec3 points[9], std::list<DzBox3> nodes)
{
	_gBestFitness = 99999;
	_gBestPos = DzVec3();
	_nodes = nodes;
	_point = p;
	_dirPOI = dirPOI;
	_min = min;
	_max = max;
	_numOfChromo = 100;
	_bestPoint = bestPoint;
	_preferableDistance = sqrt(pow(_point.m_x - bestPoint.m_x, 2) + pow(_point.m_y - bestPoint.m_y, 2) + pow(_point.m_z - bestPoint.m_z, 2));

	for (int i = 0; i < 9; i++)
	{
		_points[i] = points[i];
	}

	float fit = Fitness(_bestPoint, _point, _points, _nodes);

	if (fit == 0) 
	{
		_fitness = 0;
		_gBestPos = _bestPoint;
	}
	else 
	{
		// Random generator initializator
		std::random_device rd;
		std::default_random_engine gen(rd());
		std::uniform_real<> x(min.m_x, max.m_x);
		std::uniform_real<> y(min.m_y, max.m_y);
		std::uniform_real<> z(min.m_z, max.m_z);

		// Initialize particles
		for (int i = 0; i < _numOfChromo; i++)
		{
			DzVec3 vec = DzVec3();
			vec.m_x = x(gen);
			vec.m_y = y(gen);
			vec.m_z = z(gen);
			if (_max.m_x <= vec.m_x && vec.m_x <= _min.m_x && _max.m_y <= vec.m_y && vec.m_y <= _min.m_y && _max.m_z <= vec.m_z && vec.m_z <= _min.m_z) 
			{
				i--;
				continue;
			}
			float fit = Fitness(vec, _point, points, nodes);

			if (fit < _gBestFitness)
			{
				_gBestFitness = fit;
				_gBestPos = vec;
			}
			Chromosome chromo = Chromosome(vec, fit);
			_chromosomes.push_back(chromo);
		}

		Search();
	}
}

float GeneticAlgorithm::Fitness(const DzVec3 origin, const DzVec3 p, const DzVec3 points[9], std::list<DzBox3> nodes)
{
	int intersetcionts = 0;
	float angle = (_dirPOI.getAngleTo(DzVec3(origin - p)) * 180.0 / 3.14159265358979323846) / 45;
	bool intersect[9] = { false };
	DzLine3 line = DzLine3(origin, origin);
	std::list<DzBox3> ::iterator node;
	for (node = nodes.begin(); node != nodes.end(); ++node)
	{
		for (int i = 0; i < 9; i++)
		{
			if (intersect[i]) continue;

			line.setEnd(points[i]);
			if (node->testIntersection(line, 0))
			{
				intersect[i] = true;
				intersetcionts++;

				if (intersetcionts == 9)
					break;
			}
		}
	}
	float dist = abs(_preferableDistance - sqrt(pow(origin.m_x - p.m_x, 2) + pow(origin.m_y - p.m_y, 2) + pow(origin.m_z - p.m_z, 2))) / _preferableDistance;

	return angle + intersetcionts + dist;
}

DzVec3 GeneticAlgorithm::GetBestPoint()
{
	return _gBestPos;
}

Chromosome GeneticAlgorithm::MakeChild(Chromosome father, Chromosome mother)
{
	float wFit = father.fitness < mother.fitness ? father.fitness : mother.fitness;
	std::random_device rd;
	std::uniform_real<> x, y, z;
	std::default_random_engine gen(rd());

	DzVec3 dir = (mother.currPos - father.currPos).normalized();
	std::uniform_real<> m(0, sqrt(pow(father.currPos.m_x - mother.currPos.m_x, 2) + pow(father.currPos.m_y - mother.currPos.m_y, 2) + pow(father.currPos.m_z - mother.currPos.m_z, 2)));

	float gg = m(gen);
	DzVec3 vec = father.currPos + dir * gg;

	if (vec.m_x > _max.m_x) vec.m_x = _max.m_x;
	else if (vec.m_x < _min.m_x) vec.m_x = _min.m_x;

	if (vec.m_y > _max.m_y) vec.m_y = _max.m_y;
	else if (vec.m_y < _min.m_y) vec.m_y = _min.m_y;

	if (vec.m_z > _max.m_z) vec.m_z = _max.m_z;
	else if (vec.m_z < _min.m_z) vec.m_z = _min.m_z;

	float fit = Fitness(vec, _point, _points, _nodes);

	if (fit < wFit + 1.9)
	{
		if (fit < _gBestFitness)
		{
			_gBestFitness = fit;
			_gBestPos = vec;
		}

		return Chromosome(vec, fit);
	}

	return Chromosome();
}

struct CompareChromosomes
{
	inline bool operator() (const Chromosome first, const Chromosome second)
	{
		return first.fitness < second.fitness;
	}
};

bool compareChromo(Chromosome lhs, Chromosome rhs)
{
	return lhs.currPos == rhs.currPos;
}

bool  operator<(const Chromosome  &c1, const Chromosome &c2)
{
	return c1.fitness < c2.fitness;
}

DzVec3* GeneticAlgorithm::Search()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<> rnd(0, 10);
	Chromosome p;

	std::vector<Chromosome> newChromosomes;

	// selection
	int generation = 0;
	while (generation < 100)
	{
		std::sort(_chromosomes.begin(), _chromosomes.end(), CompareChromosomes());

		int numOfNewChildren = 0;
		newChromosomes.clear();
		bool done = false;
		while (numOfNewChildren < 100) {
			for (int i = 0; i < 25; i++)
			{
				if (done) break;
				for (int j = i + 1; j < 5; j++)
				{
					numOfNewChildren = Breed(_chromosomes.at(i), _chromosomes.at(j), &newChromosomes, numOfNewChildren);
					if (numOfNewChildren == 100) {
						done = true;
						break;
					}
				}
			}
		}
		_chromosomes = std::vector<Chromosome>(newChromosomes.begin(), newChromosomes.end());

		// Random generator initializator
		std::uniform_real<> x(_min.m_x, _max.m_x);
		std::uniform_real<> y(_min.m_y, _max.m_y);
		std::uniform_real<> z(_min.m_z, _max.m_z);
		std::vector<Chromosome> s;

		newChromosomes.clear();

		for (int i = 0; i < 20; i++)
		{
			_chromosomes.pop_back();
		}

		for (int i = 0; i < 20; i++)
		{
			DzVec3 vec = DzVec3();
			vec.m_x = x(gen);
			vec.m_y = y(gen);
			vec.m_z = z(gen);
			float fit = Fitness(vec, _point, _points, _nodes);

			if (fit < _gBestFitness)
			{
				_gBestFitness = fit;
				_gBestPos = vec;
			}
			Chromosome chromo = Chromosome(vec, fit);
			_chromosomes.push_back(chromo);
		}

		generation++;
	}
	return &_gBestPos;
}

int GeneticAlgorithm::Breed(Chromosome f, Chromosome m, std::vector<Chromosome> *c, int num)
{
	Chromosome child = MakeChild(f, m);
	if (child.fitness == 1000) return num;
	c->push_back(child);
	return ++num;
}

bool IsFitnessSatisfied(float c) {
	return c < 1;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

