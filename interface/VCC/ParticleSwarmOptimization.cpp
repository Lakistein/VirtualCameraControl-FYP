#include "ParticleSwarmOptimization.h"

ParticleSwarmOptimization::ParticleSwarmOptimization(const DzVec3 bestPoint, const DzVec3 dirPOI, const DzVec3 p, const DzVec3 min, const DzVec3 max, const DzVec3 points[9], std::list<DzBox3> nodes)
{
	_gBestPos = DzVec3();
	_nodes = nodes;
	_point = p;
	_dirPOI = dirPOI;
	_min = min;
	_max = max;
	_numOfParticles = 100;
	_preferableDistance = sqrt(pow(_point.m_x - bestPoint.m_x, 2) + pow(_point.m_y - bestPoint.m_y, 2) + pow(_point.m_z - bestPoint.m_z, 2));
	_bestPoint = bestPoint;
	_fitness = FLT_MAX;
	c0 = 0.8;
	c1 = 1.6;
	c2 = 2.4;
	for (int i = 0; i < 9; i++)
	{
		_points[i] = points[i];
	}
	float fit = Fitness(&_bestPoint, &p, _points, _nodes);
	if (fit == 0) {
		_gBestPos = _bestPoint;
		_fitness = fit;
	}
	else {
		// Random generator initializator
		std::random_device rd;
		std::default_random_engine gen(rd());
		std::uniform_real<> x(min.m_x, max.m_x);
		std::uniform_real<> y(min.m_y, max.m_y);
		std::uniform_real<> z(min.m_z, max.m_z);
		std::uniform_real<> rnd(-10, 10);

		// Initialize particles
		for (int i = 0; i < _numOfParticles; i++)
		{
			DzVec3 vec = DzVec3();
			vec.m_x = x(gen);
			vec.m_y = y(gen);
			vec.m_z = z(gen);
			if (_max.m_x <= vec.m_x && vec.m_x <= _min.m_x && _max.m_y <= vec.m_y && vec.m_y <= _min.m_y && _max.m_z <= vec.m_z && vec.m_z <= _min.m_z) {
				i--;
				continue;
			}
			float fit = Fitness(&vec, &_point, points, nodes);
			DzVec3 vel = DzVec3(rnd(gen), rnd(gen), rnd(gen));

			if (fit < _fitness)
			{
				_fitness = fit;
				_gBestPos = vec;
			}
			Particle particle = Particle(vec, vel, fit);
			_particles[i] = particle;
		}
		Search();
	}
}

ParticleSwarmOptimization::~ParticleSwarmOptimization()
{
}

float ParticleSwarmOptimization::Fitness(const DzVec3* origin, const DzVec3* p, const DzVec3 points[9], std::list<DzBox3> nodes)
{
	int intersetcionts = 0;
	float angle = _dirPOI.getAngleTo(DzVec3(*origin - *p)) * 180.0 / 3.14159265358979323846;
	angle = angle / 45;
	bool intersect[9] = { false };
	DzLine3 line = DzLine3(*origin, *origin);
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

				if (intersetcionts == 8)
					break;
			}
		}
	}
	float dist = abs(_preferableDistance - sqrt(pow(origin->m_x - p->m_x, 2) + pow(origin->m_y - p->m_y, 2) + pow(origin->m_z - p->m_z, 2))) / _preferableDistance;

	return angle + intersetcionts + dist;
}

DzVec3 ParticleSwarmOptimization::GetBestPoint()
{
	return _gBestPos;
}

DzVec3* ParticleSwarmOptimization::Search()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<> rnd(0.1, 1);
	Particle *p;
	for (int j = 0; j < 100; j++)
	{
		for (int i = 0; i < _numOfParticles; i++)
		{
			p = &_particles[i];
			DzVec3 vel = c0 * p->velocity
				+ (c1 * rnd(gen) * (p->lBestPos - p->currPos))
				+ (c2 * rnd(gen) * (_gBestPos - p->currPos));

			p->velocity = vel;
			p->currPos = p->currPos + p->velocity;

			if (p->currPos.m_x > _max.m_x) p->currPos.m_x = _max.m_x;
			else if (p->currPos.m_x < _min.m_x) p->currPos.m_x = _min.m_x;

			if (p->currPos.m_y > _max.m_y) p->currPos.m_y = _max.m_y;
			else if (p->currPos.m_y < _min.m_y) p->currPos.m_y = _min.m_y;

			if (p->currPos.m_z > _max.m_z) p->currPos.m_z = _max.m_z;
			else if (p->currPos.m_z < _min.m_z) p->currPos.m_z = _min.m_z;

			int fit = Fitness(&p->currPos, &_point, _points, _nodes);

			// LOCAL

			if (fit < p->fitness)
			{
				p->fitness = fit;
				p->lBestPos = p->currPos;
			}
			else
			{
				continue;
			}

			// GLOBAL

			if (fit < _fitness)
			{
				_fitness = fit;
				_gBestPos = p->lBestPos = p->currPos;

				if (fit < 0.1)
				{
					return &_gBestPos;
				}

				continue;
			}
		}
	}

	return &_gBestPos;
}

float ParticleSwarmOptimization::GetAngle(const DzVec3 * A, const DzVec3 * B)
{
	return acos(A->dot(*B) / (A->length() * B->length())) * 180.0 / 3.14159265358979323846;
}

void ParticleSwarmOptimization::SetNumberOfParticles(int num)
{
	_numOfParticles = num;
}